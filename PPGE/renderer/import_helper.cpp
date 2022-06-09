#include "import_helper.h"

#include "ecs/components/mesh_component.h"
#include "ecs/entity.h"
#include "io/resources.h"
#include "renderer/geometry_helper.h"
#include "renderer/renderer_types.h"
#include "rhi/texture.h"
#include "rhi/texture_view.h"
#include "system/renderer_system.h"

namespace PPGE
{
static std::shared_ptr<PPGETextureView> LoadEmbeddedTexture(const aiTexture *texture)
{
    std::shared_ptr<PPGETexture> ppge_texture;
    {
        TextureCreateDesc tex_cd;

        // Load uncompressed texture
        if (texture->mHeight != 0)
        {
            tex_cd.desc.resource_dimension = ResourceDimensionType::RESOURCE_DIMENSION_2D;
            tex_cd.desc.width = texture->mWidth;
            tex_cd.desc.height = texture->mHeight;
            tex_cd.desc.format_type = TextureFormatType::TEXTURE_FORMAT_B8G8R8A8_UNORM;
            tex_cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE;

            TextureData texture_data;
            texture_data.data_ptr = reinterpret_cast<void *>(texture->pcData);
            texture_data.pitch = texture->mWidth * 4;
            texture_data.slice_pitch = texture->mWidth * texture->mHeight * 4;
            tex_cd.subresource = &texture_data;
            tex_cd.subresource_num = 1;

            RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, ppge_texture);
            TextureViewDesc view_desc;
            view_desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
            view_desc.format = tex_cd.desc.format_type;
            view_desc.resource_dimension = tex_cd.desc.resource_dimension;

            return ppge_texture->CreateView(view_desc);
        }
        else
        {
            tex_cd.is_compressed = true;

            if (texture->CheckFormat("dds"))
            {
                tex_cd.file_format = TextureFileFormat::DDS;
            }
            else if (texture->CheckFormat("jpg"))
            {
                tex_cd.file_format = TextureFileFormat::JPEG;
            }
            else if (texture->CheckFormat("png"))
            {
                tex_cd.file_format = TextureFileFormat::PNG;
            }

            TextureData texture_data;
            texture_data.data_ptr = reinterpret_cast<void *>(texture->pcData);
            texture_data.data_size = texture->mWidth;
            tex_cd.subresource = &texture_data;
            tex_cd.subresource_num = 1;
        }
        tex_cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE;
        RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, ppge_texture);
    }

    return ppge_texture->GetDefaultView();
}

static std::shared_ptr<PPGETextureView> LoadTexture(const std::filesystem::path &path_to_texture)
{
    std::shared_ptr<PPGETexture> diffuse_texture;
    {
        std::string path_to_res = path_to_texture.string();
        TextureCreateDesc tex_cd;
        tex_cd.resource_path = path_to_res.c_str();

        auto ext = path_to_texture.extension();
        if (ext.compare(".dds") == 0)
        {
            tex_cd.file_format = TextureFileFormat::DDS;
        }
        else if (ext.compare(".jpeg") == 0 || ext.compare(".jpg") == 0)
        {
            tex_cd.file_format = TextureFileFormat::JPEG;
        }
        else if (ext.compare(".png") == 0)
        {
            tex_cd.file_format = TextureFileFormat::PNG;
        }
        tex_cd.desc.bind_flags = BindFlags::BIND_SHADER_RESOURCE;
        RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, diffuse_texture);
    }

    return diffuse_texture->GetDefaultView();
}

class MeshLoader
{
  private:
    std::vector<StandardVertex> vertices;
    std::vector<unsigned int> indices;

  public:
    MeshLoader() = default;

    void Load(const aiMesh *ai_mesh, Entity &entity);
};

class MaterialLoader
{
  private:
    using TextureCacheTable = std::unordered_map<std::string, std::shared_ptr<PPGETextureView>>;
    TextureCacheTable diffuse_maps;
    TextureCacheTable specular_maps;
    TextureCacheTable normal_maps;

    const std::filesystem::path &resource_root;

    enum class WorkflowTypes : uint8_t
    {
        Unknwon = 0,
        BlingPhong,
        Metalic,
    };

  public:
    MaterialLoader(const std::filesystem::path &_resource_root) : resource_root(_resource_root){};

    void Load(const aiScene *ai_scene, const aiMaterial *ai_material, Entity &entity);

  private:
    void LoadBlingPhongWorkflowProperties(const aiScene *ai_scene, const aiMaterial *ai_material, Entity &entity);

    void LoadMetalicWorkflowProperties(const aiScene *ai_scene, const aiMaterial *ai_material, Entity &entity);
};

void MeshLoader::Load(const aiMesh *ai_mesh, Entity &entity)
{
    vertices.clear();
    indices.clear();

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
                indices.push_back(face.mIndices[k]);
            }
        }
    }

    GeometryHelper::CreateMeshFilter(entity, vertices, indices);
}

void MaterialLoader::Load(const aiScene *ai_scene, const aiMaterial *ai_material, Entity &entity)
{
    WorkflowTypes workflow_type = WorkflowTypes::Metalic; // TODO: Automatically detect workflow type

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

void MaterialLoader::LoadBlingPhongWorkflowProperties(const aiScene *ai_scene, const aiMaterial *ai_material,
                                                      Entity &entity)
{
    auto &mesh_renderer = entity.AddComponent<MeshRendererComponent>();
    {
        aiReturn result;

        aiColor4D Kd;
        result = aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_DIFFUSE, &Kd);
        if (result == AI_SUCCESS)
        {
            mesh_renderer.albedo_color = Math::Color(Kd.r, Kd.g, Kd.b);
        }
        else
        {
            mesh_renderer.albedo_color = Math::Color(1.0f, 1.0f, 1.0f);
        }
        aiColor4D Ks;
        result = aiGetMaterialColor(ai_material, AI_MATKEY_COLOR_SPECULAR, &Ks);
        if (result == AI_SUCCESS)
        {
            mesh_renderer.specular_color = Math::Color(Ks.r, Ks.g, Ks.b, 1.0f);
        }
        else
        {
            mesh_renderer.specular_color = Math::Color(1.0f, 1.0f, 1.0f, 1.0f);
        }
        ai_real Ns;
        result = aiGetMaterialFloat(ai_material, AI_MATKEY_SHININESS, &Ns);
        if (result == AI_SUCCESS)
        {
            mesh_renderer.specular_color.A(Ns);
        }

        aiString tex_path;
        result = aiGetMaterialString(ai_material, AI_MATKEY_TEXTURE_DIFFUSE(0), &tex_path);
        if (result == AI_SUCCESS)
        {
            auto path_to_diff_texture = resource_root.parent_path() / std::filesystem::path(tex_path.C_Str());
            auto it = diffuse_maps.find(path_to_diff_texture.string());
            if (it == diffuse_maps.end())
            {
                auto [_it, success] =
                    diffuse_maps.emplace(path_to_diff_texture.string(), LoadTexture(path_to_diff_texture));
                it = _it;
            }
            mesh_renderer.albedo_map = it->second;
        }

        result = aiGetMaterialString(ai_material, AI_MATKEY_TEXTURE_SPECULAR(0), &tex_path);
        if (result == AI_SUCCESS)
        {
            auto path_to_specular_texture = resource_root.parent_path() / std::filesystem::path(tex_path.C_Str());
            auto it = specular_maps.find(path_to_specular_texture.string());
            if (it == specular_maps.end())
            {
                auto [_it, success] =
                    specular_maps.emplace(path_to_specular_texture.string(), LoadTexture(path_to_specular_texture));
                it = _it;
            }
            mesh_renderer.specular_map = it->second;
        }

        result = aiGetMaterialString(ai_material, AI_MATKEY_TEXTURE_NORMALS(0), &tex_path);
        if (result == AI_SUCCESS)
        {
            auto path_to_normal_texture = resource_root.parent_path() / std::filesystem::path(tex_path.C_Str());
            auto it = normal_maps.find(path_to_normal_texture.string());
            if (it == normal_maps.end())
            {
                auto [_it, success] =
                    normal_maps.emplace(path_to_normal_texture.string(), LoadTexture(path_to_normal_texture));
                it = _it;
            }
            mesh_renderer.normal_map = it->second;
        }
    }
}

void MaterialLoader::LoadMetalicWorkflowProperties(const aiScene *ai_scene, const aiMaterial *ai_material,
                                                   Entity &entity)
{
    auto &mesh_renderer = entity.AddComponent<MeshRendererComponent>();
    {
        aiReturn result;

        aiColor4D Kd;
        result = ai_material->Get(AI_MATKEY_BASE_COLOR, Kd);
        if (result == AI_SUCCESS)
        {
            mesh_renderer.albedo_color = Math::Color(Kd.r, Kd.g, Kd.b);
        }

        aiString tex_path;
        result = ai_material->GetTexture(AI_MATKEY_BASE_COLOR_TEXTURE, &tex_path);
        if (result == AI_SUCCESS)
        {
            auto path_to_diff_texture = resource_root.parent_path() / std::filesystem::path(tex_path.C_Str());
            auto it = diffuse_maps.find(path_to_diff_texture.string());
            if (it == diffuse_maps.end())
            {
                if (auto texture = ai_scene->GetEmbeddedTexture(tex_path.C_Str()))
                {
                    auto [_it, success] =
                        diffuse_maps.emplace(path_to_diff_texture.string(), LoadEmbeddedTexture(texture));
                    it = _it;
                }
                else
                {
                    auto [_it, success] =
                        diffuse_maps.emplace(path_to_diff_texture.string(), LoadTexture(path_to_diff_texture));
                    it = _it;
                }
            }
            mesh_renderer.albedo_map = it->second;
        }
    }
}

void ModelLoader::ImportModel(const LazyResource &resource, Entity &entity)
{
    MeshLoader mesh_loader;
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

std::shared_ptr<PPGETextureView> ModelLoader::ImportTexture(const LazyResource &resource)
{
    return LoadTexture(resource.data.string().c_str());
}
} // namespace PPGE
