#define PPGE_APP_ENTRY_POINT
#include <PPGE.h>

constexpr float PI = 3.14159265359f;

static size_t ENTITY_COUNT = 0;

namespace PPGE
{
template <typename VertexBufferType>
void CreateMeshFilter(Entity &entity, const std::vector<VertexBufferType> &vertices,
                      const std::vector<unsigned int> &indices)
{
    std::shared_ptr<PPGEBuffer> vb;
    {
        BufferDesc vb_desc;
        vb_desc.byte_width = vertices.size() * sizeof(VertexBufferType);
        vb_desc.bind_flags = BindFlags::BIND_VERTEX_BUFFER;
        vb_desc.usage = UsageType::USAGE_IMMUTABLE;
        vb_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_NONE;
        BufferData vb_init_data;
        vb_init_data.data_ptr = &vertices[0];
        vb_init_data.data_size = vertices.size() * sizeof(VertexBufferType);
        RendererSystem::Get().GetDevice()->CreateBuffer(vb_desc, &vb_init_data, vb);
    }

    std::shared_ptr<PPGEBuffer> ib;
    {
        BufferDesc ib_desc;
        ib_desc.byte_width = indices.size() * sizeof(unsigned int);
        ib_desc.bind_flags = BindFlags::BIND_INDEX_BUFFER;
        ib_desc.usage = UsageType::USAGE_IMMUTABLE;
        ib_desc.cpu_access_flags = CPUAccessFlags::CPU_ACCESS_NONE;
        BufferData ib_init_data;
        ib_init_data.data_ptr = &indices[0];
        ib_init_data.data_size = indices.size() * sizeof(unsigned int);
        RendererSystem::Get().GetDevice()->CreateBuffer(ib_desc, &ib_init_data, ib);
    }

    auto &mesh_filter = entity.AddComponent<MeshFilterComponent>();
    mesh_filter.vertex_buffer = vb;
    mesh_filter.index_buffer = ib;
    mesh_filter.num_indices = indices.size();
}

std::shared_ptr<PPGETextureView> LoadTexture(const std::filesystem::path &path_to_diffuse_tex)
{
    std::shared_ptr<PPGETexture> diffuse_texture;
    {
        std::string path_to_res = path_to_diffuse_tex.string();
        TextureCreateDesc tex_cd;
        tex_cd.resource_path = path_to_res.c_str();

        auto ext = path_to_diffuse_tex.extension();
        if (ext.compare(".dds") == 0)
        {
            tex_cd.file_format = TextureFileFormat::DDS;
        }
        else if (ext.compare(".jpeg") == 0)
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

class SceneLoader
{
  public:
    static void LoadScene(const std::filesystem::path &path_to_scene, Scene &ppge_scene, float scale_factor = 1.0f)
    {
        std::unordered_map<std::string, std::shared_ptr<PPGETextureView>> diffuse_maps;
        std::unordered_map<std::string, std::shared_ptr<PPGETextureView>> normal_maps;
        std::vector<StandardVertex> vertices;
        std::vector<unsigned int> indices;

        const aiScene *scene = aiImportFile(path_to_scene.string().c_str(),
                                            aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

        scene = aiApplyPostProcessing(scene, aiProcess_FixInfacingNormals);

        if (!scene || !scene->HasMeshes())
        {
            APP_ERROR("Unable to load {0}\n", path_to_scene.string().c_str());
            return;
        }

        for (unsigned i = 0; i < scene->mNumMeshes; ++i)
        {

            const aiMesh *mesh = scene->mMeshes[i];
            for (unsigned j = 0; j < mesh->mNumVertices; ++j)
            {
                auto &vertex = vertices.emplace_back();
                if (mesh->HasPositions())
                {
                    vertex.px = mesh->mVertices[j].x;
                    vertex.py = mesh->mVertices[j].y;
                    vertex.pz = mesh->mVertices[j].z;
                }
                if (mesh->HasNormals())
                {
                    vertex.nx = mesh->mNormals[j].x;
                    vertex.ny = mesh->mNormals[j].y;
                    vertex.nz = mesh->mNormals[j].z;
                }
                if (mesh->HasTextureCoords(0))
                {
                    vertex.u1 = mesh->mTextureCoords[0][j].x;
                    vertex.v1 = mesh->mTextureCoords[0][j].y;
                }
                if (mesh->HasTangentsAndBitangents())
                {
                    vertex.tx = mesh->mTangents[j].x;
                    vertex.ty = mesh->mTangents[j].y;
                    vertex.tz = mesh->mTangents[j].z;
                    vertex.btx = mesh->mBitangents[j].x;
                    vertex.bty = mesh->mBitangents[j].y;
                    vertex.btz = mesh->mBitangents[j].z;
                }
            }

            if (mesh->HasFaces())
            {
                for (unsigned j = 0; j < mesh->mNumFaces; ++j)
                {
                    const aiFace face = mesh->mFaces[j];
                    for (unsigned k = 0; k < face.mNumIndices; k++)
                    {
                        indices.push_back(face.mIndices[k]);
                    }
                }
            }

            auto entity = ppge_scene.CreateEntity("Entity " + std::to_string(++ENTITY_COUNT));
            auto &transform = entity.GetComponents<TransformComponent>();
            transform.scale = Math::Vector3(scale_factor, scale_factor, scale_factor);
            CreateMeshFilter(entity, vertices, indices);

            auto &mesh_renderer = entity.AddComponent<MeshRendererComponent>();
            {
                aiReturn result;
                const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

                aiColor4D Kd;
                result = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &Kd);
                if (result == AI_SUCCESS)
                {
                    mesh_renderer.albedo_color = Math::Color(Kd.r, Kd.g, Kd.b);
                }
                else
                {
                    mesh_renderer.albedo_color = Math::Color(1.0f, 1.0f, 1.0f);
                }
                aiColor4D Ks;
                result = aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &Ks);
                if (result == AI_SUCCESS)
                {
                    mesh_renderer.specular_color = Math::Color(Ks.r, Ks.g, Ks.b, 1.0f);
                }
                else
                {
                    mesh_renderer.specular_color = Math::Color(1.0f, 1.0f, 1.0f);
                }
                ai_real Ns;
                result = aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &Ns);
                if (result == AI_SUCCESS)
                {
                    mesh_renderer.specular_color.A(Ns);
                }

                aiString tex_path;
                result = aiGetMaterialString(mat, AI_MATKEY_TEXTURE_DIFFUSE(0), &tex_path);
                if (result == AI_SUCCESS)
                {
                    auto path_to_diff_texture = path_to_scene.parent_path() / std::filesystem::path(tex_path.C_Str());
                    auto it = diffuse_maps.find(path_to_diff_texture.string());
                    if (it == diffuse_maps.end())
                    {
                        auto [_it, success] =
                            diffuse_maps.emplace(path_to_diff_texture.string(), LoadTexture(path_to_diff_texture));
                        it = _it;
                    }
                    mesh_renderer.albedo_map = it->second;
                }

                result = aiGetMaterialString(mat, AI_MATKEY_TEXTURE_NORMALS(0), &tex_path);
                if (result == AI_SUCCESS)
                {
                    auto path_to_normal_texture = path_to_scene.parent_path() / std::filesystem::path(tex_path.C_Str());
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

            vertices.clear();
            indices.clear();
        }

        aiReleaseImport(scene);
    }
};

class TestLayer : public Widget
{
    ResourceManager resource_mgr;

    FreeLookCamera m_camera_controller;

    Scene m_scene;

    Renderer m_renderer;

    Entity m_light;

    bool is_normal_map_enabled = false;

  public:
    TestLayer() : Widget("TestSubsystem"), m_camera_controller{}
    {
        resource_mgr.RegisterLoaderMultiple<LazyResource, LazyLoader>(
            {".gltf", ".obj", ".mtl", ".bmp", ".png", ".jpg", ".dds"});
    }

    void OnAttach() override
    {
        // Create camera
        auto camera_entity = m_scene.CreateEntity("Camera");
        auto &camera_component = camera_entity.AddComponent<CameraComponent>();
        {
            float fov = PI * 0.333f;
            float aspect_ratio = (float)DisplaySystem::Get().GetWidth() / (float)DisplaySystem::Get().GetHeight();
            camera_component.projection = Math::Matrix::CreatePerspectiveFieldOfView(fov, aspect_ratio, 0.1f, 1000.0f);
        }
        m_camera_controller.Possess(camera_entity);
        m_camera_controller.Move(Math::Vector3(-1.5f, 1.75f, -1.5f));
        m_camera_controller.LookAt(Math::Vector3::Zero, Math::Vector3::Up);

        // Create lights
        {
            m_light = m_scene.CreateEntity("Point Light");
            auto &transform = m_light.GetComponents<TransformComponent>();
            transform.position = Math::Vector3(1.0f, 1.0f, 1.0f);
            auto &point_light = m_light.AddComponent<LightComponent>(LightComponent::LightType::POINT);
            point_light.color = Math::Color(1.0f, 1.0f, 1.0f);
            point_light.intensity = 1.5f;
        }

        resource_mgr.WalkRoot("../../Sandbox/assets");
        if (auto model = resource_mgr.GetCachedResource("cube/cube.obj"))
        {
            auto lazy_model = std::static_pointer_cast<LazyResource>(model);
            SceneLoader::LoadScene(lazy_model->data, m_scene);
        }
    }

    void OnDetach() override
    {
    }

    void OnUpdate(float delta_time) override
    {
        m_camera_controller.Update(delta_time);
    }

    void OnRender() override
    {
        m_renderer.BeginScene(m_scene, m_camera_controller).Submit();
    }

    virtual void OnImGui()
    {
        ImGui::Begin("Light Debug");

        auto &transform = m_light.GetComponents<TransformComponent>();
        DrawVectorController("Position", transform.position, 0.0f, 100.0f);

        if (ImGui::Checkbox("Enable Normal Map", &is_normal_map_enabled))
        {
            if (is_normal_map_enabled)
            {
                RendererOptions options = m_renderer.GetRendererOptions();
                options |= RendererOptions::ENABLE_NORMAL_MAP;
                m_renderer.SetRendererOptions(options);
            }
            else
            {
                RendererOptions options = m_renderer.GetRendererOptions();
                options &= ~RendererOptions::ENABLE_NORMAL_MAP;
                m_renderer.SetRendererOptions(options);
            }
        }

        ImGui::End();
    }

  private:
    void DrawVectorController(const std::string &label, Math::Vector3 &vec3, float reset_value, float column_width)

    {
        ImGuiIO &io = ImGui::GetIO();
        auto bold_font = io.Fonts->Fonts[0];

        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, column_width);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

        float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 button_size = {line_height + 3.0f, line_height};

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
        ImGui::PushFont(bold_font);
        if (ImGui::Button("X", button_size))
            vec3.x = reset_value;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &vec3.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
        ImGui::PushFont(bold_font);
        if (ImGui::Button("Y", button_size))
            vec3.y = reset_value;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &vec3.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
        ImGui::PushFont(bold_font);
        if (ImGui::Button("Z", button_size))
            vec3.z = reset_value;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &vec3.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::PopID();
    }
};

class Game : public Application
{
  public:
    Game()
    {
    }

    ~Game()
    {
    }

    void StartUp() override
    {
        Application::StartUp();
        CreateWidget<TestLayer>();
        APP_INFO("Application is started up.");
    }

    void ShutDown() override
    {
        APP_INFO("Application is shut down.");
        Application::ShutDown();
    }
};
} // namespace PPGE

PPGE::Application *PPGE::CreateApplication()
{
    return new PPGE::Game();
}
