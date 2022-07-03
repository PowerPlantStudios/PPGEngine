#include "import_helper.h"

#include "ecs/components/mesh_component.h"
#include "ecs/entity.h"
#include "io/resources.h"
#include "renderer/material.h"
#include "renderer/mesh.h"
#include "renderer/renderer_types.h"
#include "rhi/texture.h"
#include "rhi/texture_view.h"
#include "system/renderer_system.h"

namespace PPGE
{
class MeshLoader
{
  private:
    std::vector<std::vector<StandardVertex>> m_vertices;
    std::vector<unsigned int> m_indices;

    const std::filesystem::path &m_resource_root;

  public:
    MeshLoader(const std::filesystem::path &resource_root) : m_resource_root(resource_root){};

    void Load(const aiMesh *ai_mesh, Entity &entity)
    {
        auto &mesh_filter = entity.AddComponent<MeshFilterComponent>();
        {
            m_indices.clear();
            m_vertices.clear();
            auto &vertices = m_vertices.emplace_back();

            for (unsigned j = 0; j < ai_mesh->mNumVertices; ++j)
            {
                auto &vertex = vertices.emplace_back();
                if (ai_mesh->HasPositions())
                {
                    vertex.px = ai_mesh->mVertices[j].x;
                    vertex.py = ai_mesh->mVertices[j].y;
                    vertex.pz = ai_mesh->mVertices[j].z;
                }
                if (ai_mesh->HasNormals())
                {
                    vertex.nx = ai_mesh->mNormals[j].x;
                    vertex.ny = ai_mesh->mNormals[j].y;
                    vertex.nz = ai_mesh->mNormals[j].z;
                }
                if (ai_mesh->HasTextureCoords(0))
                {
                    vertex.u1 = ai_mesh->mTextureCoords[0][j].x;
                    vertex.v1 = ai_mesh->mTextureCoords[0][j].y;
                }
                if (ai_mesh->HasTangentsAndBitangents())
                {
                    vertex.tx = ai_mesh->mTangents[j].x;
                    vertex.ty = ai_mesh->mTangents[j].y;
                    vertex.tz = ai_mesh->mTangents[j].z;
                    vertex.btx = ai_mesh->mBitangents[j].x;
                    vertex.bty = ai_mesh->mBitangents[j].y;
                    vertex.btz = ai_mesh->mBitangents[j].z;
                }
            }

            if (ai_mesh->HasFaces())
            {
                for (unsigned j = 0; j < ai_mesh->mNumFaces; ++j)
                {
                    const aiFace face = ai_mesh->mFaces[j];
                    for (unsigned k = 0; k < face.mNumIndices; k++)
                    {
                        m_indices.push_back(face.mIndices[k]);
                    }
                }
            }
            mesh_filter.mesh = MeshHelper::CreateMesh(m_vertices, m_indices);
        }
    }
};

template <typename T> std::optional<T> GetMaterialProperty(const aiMaterial *, const char *, unsigned int, unsigned int)
{
    return {};
}

template <>
std::optional<Math::Color> GetMaterialProperty(const aiMaterial *ai_material, const char *p_key, unsigned int type,
                                               unsigned int index)
{
    aiColor4D ai_color;
    aiReturn result = ai_material->Get(p_key, type, index, ai_color);
    if (result == AI_SUCCESS)
        return Math::Color(ai_color.r, ai_color.g, ai_color.b, ai_color.a);
    return {};
}

template <>
std::optional<int> GetMaterialProperty(const aiMaterial *ai_material, const char *p_key, unsigned int type,
                                       unsigned int index)
{
    ai_int ai_value;
    aiReturn result = ai_material->Get(p_key, type, index, ai_value);
    if (result == AI_SUCCESS)
        return ai_value;
    return {};
}

template <>
std::optional<float> GetMaterialProperty(const aiMaterial *ai_material, const char *p_key, unsigned int type,
                                         unsigned int index)
{
    ai_real ai_value;
    aiReturn result = ai_material->Get(p_key, type, index, ai_value);
    if (result == AI_SUCCESS)
        return ai_value;
    return {};
}

template <>
std::optional<std::string> GetMaterialProperty(const aiMaterial *ai_material, const char *p_key, unsigned int type,
                                               unsigned int index)
{
    aiString ai_str;
    aiReturn result = ai_material->Get(p_key, type, index, ai_str);
    if (result == AI_SUCCESS)
        return ai_str.C_Str();
    return {};
}

class MaterialLoader
{
  private:
    using TextureCacheTable = std::unordered_map<std::string, std::shared_ptr<PPGETextureView>>;
    TextureCacheTable m_texture_cache_table;

    const std::filesystem::path &m_resource_root;

    enum class WorkflowTypes : uint8_t
    {
        Unknwon = 0,
        BlingPhong,
        Metalic,
    };

  public:
    MaterialLoader(const std::filesystem::path &resource_root) : m_resource_root(resource_root){};

    void Load(const aiScene *ai_scene, const aiMaterial *ai_material, Entity &entity)
    {
        WorkflowTypes workflow_type = WorkflowTypes::BlingPhong;
        if (auto shading_model = GetMaterialProperty<int>(ai_material, AI_MATKEY_SHADING_MODEL))
        {
            if ((aiShadingMode)(*shading_model) == aiShadingMode::aiShadingMode_PBR_BRDF)
                workflow_type = WorkflowTypes::Metalic;
            // TODO: Check for Metallic/Roughness or Specular/Glossiness workflows
        }

        switch (workflow_type)
        {
        case WorkflowTypes::BlingPhong:
            LoadBlingPhongWorkflowProperties(ai_scene, ai_material, entity);
            break;
        case WorkflowTypes::Metalic:
            LoadMetalicWorkflowProperties(ai_scene, ai_material, entity);
            break;
        default:
            PPGE_ASSERT(false, "Unknown workflow type");
            break;
        }
    }

  private:
    std::shared_ptr<PPGETextureView> LoadTexture(const aiScene *ai_scene, const aiMaterial *ai_material,
                                                 aiTextureType tex_type, unsigned int index)
    {
        if (ai_material->GetTextureCount(tex_type) == 0)
            return nullptr;

        aiString tex_path;
        aiReturn result = ai_material->GetTexture(tex_type, index, &tex_path);
        if (result == AI_SUCCESS)
        {
            auto path_to_texture = std::filesystem::weakly_canonical(m_resource_root.parent_path() /
                                                                     std::filesystem::path(tex_path.C_Str()));
            auto it = m_texture_cache_table.find(path_to_texture.string());
            if (it == m_texture_cache_table.end())
            {
                if (auto texture = ai_scene->GetEmbeddedTexture(tex_path.C_Str()))
                {
                    auto [_it, success] = m_texture_cache_table.emplace(
                        path_to_texture.string(), MaterialHelper::LoadTexture(texture->mHeight, texture->mWidth,
                                                                              texture->pcData, texture->achFormatHint));
                    it = _it;
                }
                else
                {
                    auto [_it, success] = m_texture_cache_table.emplace(path_to_texture.string(),
                                                                        MaterialHelper::LoadTexture(path_to_texture));
                    it = _it;
                }
            }
            return it->second;
        }
        return nullptr;
    }

    void LoadBlingPhongWorkflowProperties(const aiScene *ai_scene, const aiMaterial *ai_material, Entity &entity)
    {
        auto &mesh_renderer = entity.AddComponent<MeshRendererComponent>();
        {
            LegacyMaterial::MaterialDesc desc{.albedo_color = Math::Color(1.0f, 1.0f, 1.0f),
                                              .specular_color = Math::Color(1.0f, 1.0f, 1.0f),
                                              .emissive_color = Math::Color(1.0f, 1.0f, 1.0f),
                                              .shininess = 1.0f,
                                              .alpha_cutoff = 0.0f};

            auto name = GetMaterialProperty<std::string>(ai_material, AI_MATKEY_NAME);

            if (auto Kd = GetMaterialProperty<Math::Color>(ai_material, AI_MATKEY_COLOR_DIFFUSE))
                desc.albedo_color = *Kd;

            if (auto Ks = GetMaterialProperty<Math::Color>(ai_material, AI_MATKEY_COLOR_SPECULAR))
                desc.specular_color = *Ks;

            if (auto Ke = GetMaterialProperty<Math::Color>(ai_material, AI_MATKEY_COLOR_EMISSIVE))
                desc.emissive_color = *Ke;

            if (auto Ns = GetMaterialProperty<float>(ai_material, AI_MATKEY_SHININESS))
                desc.shininess = *Ns;

            if (auto opacity = GetMaterialProperty<float>(ai_material, AI_MATKEY_OPACITY))
                desc.alpha_cutoff = *opacity;

            auto material = MaterialHelper::CreateMaterial<LegacyMaterial>(desc);

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_DIFFUSE, 0))
                material->SetAlbedoMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_SPECULAR, 0))
                material->SetSpecularMap(std::move(map));
            else if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_SHININESS, 0))
                material->SetSpecularMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_NORMALS, 0))
                material->SetNormalMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_EMISSIVE, 0))
                material->SetEmissionMap(std::move(map));

            mesh_renderer.material = std::move(material);
        }
    }

    void LoadMetalicWorkflowProperties(const aiScene *ai_scene, const aiMaterial *ai_material, Entity &entity)
    {
        auto &mesh_renderer = entity.AddComponent<MeshRendererComponent>();
        {
            PBRMaterial::MaterialDesc desc{.albedo_color = Math::Color(1.0f, 1.0f, 1.0f),
                                           .emissive_color = Math::Color(1.0f, 1.0f, 1.0f),
                                           .roughness_factor = 1.0f,
                                           .metalic_factor = 1.0f,
                                           .alpha_cutoff = 0.0f};

            auto name = GetMaterialProperty<std::string>(ai_material, AI_MATKEY_NAME);

            if (auto Kd = GetMaterialProperty<Math::Color>(ai_material, AI_MATKEY_BASE_COLOR))
                desc.albedo_color = *Kd;

            if (auto Ke = GetMaterialProperty<Math::Color>(ai_material, AI_MATKEY_COLOR_EMISSIVE))
                desc.emissive_color = *Ke;

            if (auto roughness = GetMaterialProperty<float>(ai_material, AI_MATKEY_ROUGHNESS_FACTOR))
                desc.roughness_factor = *roughness;

            if (auto metalic = GetMaterialProperty<float>(ai_material, AI_MATKEY_METALLIC_FACTOR))
                desc.metalic_factor = *metalic;

            if (auto opacity = GetMaterialProperty<float>(ai_material, AI_MATKEY_OPACITY))
                desc.alpha_cutoff = *opacity;

            auto material = MaterialHelper::CreateMaterial<PBRMaterial>(desc);

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_BASE_COLOR, 0))
                material->SetAlbedoMap(std::move(map));
            else if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_DIFFUSE, 0))
                material->SetAlbedoMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_DIFFUSE_ROUGHNESS, 0))
            {
                material->SetRoughnessMap(std::move(map));
                if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_METALNESS, 0))
                    material->SetMetallicMap(std::move(map));
            }
            else if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_UNKNOWN, 0))
            {
                material->SetRoughnessMap(map);
                material->SetMetallicMap(std::move(map));
            }

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_NORMAL_CAMERA, 0))
                material->SetNormalMap(std::move(map));
            else if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_NORMALS, 0))
                material->SetNormalMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_AMBIENT_OCCLUSION, 0))
                material->SetOcclusionMap(std::move(map));
            else if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_LIGHTMAP, 0))
                material->SetOcclusionMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_EMISSION_COLOR, 0))
                material->SetEmissionMap(std::move(map));
            else if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_EMISSIVE, 0))
                material->SetEmissionMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_HEIGHT, 0))
                material->SetHeightMap(std::move(map));

            if (auto map = LoadTexture(ai_scene, ai_material, aiTextureType_OPACITY, 0))
                material->SetAlphaMaskMap(std::move(map));

            mesh_renderer.material = std::move(material);
        }
    }
};

void ImportHelper::ImportModel(const LazyResource &resource, Entity &entity)
{
    MeshLoader mesh_loader(resource.data);
    MaterialLoader material_loader(resource.data);

    const aiScene *ai_scene = aiImportFile(resource.data.string().c_str(),
                                           aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);
    ai_scene = aiApplyPostProcessing(ai_scene, aiProcess_FixInfacingNormals | aiProcess_MakeLeftHanded);

    if (!ai_scene)
    {
        APP_ERROR("Unable to load {0}\n", resource.data.string().c_str());
        return;
    }

    if (!ai_scene->HasMeshes())
    {
        APP_ERROR("Reource doesn't have mesh: {0}\n", resource.data.string().c_str());
        return;
    }

    for (unsigned i = 0; i < ai_scene->mNumMeshes; ++i)
    {
        auto &child_entity = entity.CreateChildEntity();

        const aiMesh *ai_mesh = ai_scene->mMeshes[i];
        mesh_loader.Load(ai_mesh, child_entity);

        const aiMaterial *ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
        material_loader.Load(ai_scene, ai_material, child_entity);
    }

    aiReleaseImport(ai_scene);
}

} // namespace PPGE
