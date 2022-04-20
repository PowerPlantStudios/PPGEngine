#include <PPGE.h>

constexpr float pi = 3.14159265359f;

template <typename VertexBufferType>
void CreateMeshFilter(PPGE::Entity &entity, const std::vector<VertexBufferType> &vertices,
                      const std::vector<unsigned int> &indices)
{
    std::shared_ptr<PPGE::PPGEBuffer> vb;
    {
        PPGE::BufferDesc vb_desc;
        vb_desc.byte_width = vertices.size() * sizeof(VertexBufferType);
        vb_desc.bind_flags = PPGE::BindFlags::BIND_VERTEX_BUFFER;
        vb_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
        vb_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
        PPGE::BufferData vb_init_data;
        vb_init_data.data_ptr = &vertices[0];
        vb_init_data.data_size = vertices.size() * sizeof(VertexBufferType);
        PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(vb_desc, &vb_init_data, vb);
    }

    std::shared_ptr<PPGE::PPGEBuffer> ib;
    {
        PPGE::BufferDesc ib_desc;
        ib_desc.byte_width = indices.size() * sizeof(unsigned int);
        ib_desc.bind_flags = PPGE::BindFlags::BIND_INDEX_BUFFER;
        ib_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
        ib_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
        PPGE::BufferData ib_init_data;
        ib_init_data.data_ptr = &indices[0];
        ib_init_data.data_size = indices.size() * sizeof(unsigned int);
        PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(ib_desc, &ib_init_data, ib);
    }

    auto &mesh_filter = entity.AddComponent<PPGE::MeshFilterComponent>();
    mesh_filter.vertex_buffer = vb;
    mesh_filter.index_buffer = ib;
    mesh_filter.num_indices = indices.size();
}

std::shared_ptr<PPGE::PPGETextureView> LoadTexture(const std::filesystem::path &path_to_diffuse_tex)
{
    std::shared_ptr<PPGE::PPGETexture> diffuse_texture;
    {
        std::string path_to_res = path_to_diffuse_tex.string();
        PPGE::TextureCreateDesc tex_cd;
        tex_cd.resource_path = path_to_res.c_str();
        
        auto ext = path_to_diffuse_tex.extension();
        if (ext.compare(".dds") == 0)
        {
            tex_cd.file_format = PPGE::TextureFileFormat::DDS;
        }
        else if (ext.compare(".jpeg") == 0)
        {
            tex_cd.file_format = PPGE::TextureFileFormat::JPEG;
        }
        else if (ext.compare(".png") == 0)
        {
            tex_cd.file_format = PPGE::TextureFileFormat::PNG;
        }
        tex_cd.desc.bind_flags = PPGE::BindFlags::BIND_SHADER_RESOURCE;
        PPGE::RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, diffuse_texture);
    }

    return diffuse_texture->GetDefaultView();
}

class SceneLoader
{
  public:
    static void LoadScene(const std::filesystem::path &path_to_scene, PPGE::Scene &ppge_scene, float scale_factor = 1.0f)
    {
        std::unordered_map<std::string, std::shared_ptr<PPGE::PPGETextureView>> diffuse_maps;
        std::vector<PPGE::FullVertex> vertices;
        std::vector<unsigned int> indices;

        const aiScene *scene = aiImportFile(path_to_scene.string().c_str(),
                                            aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

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

            auto entity = ppge_scene.CreateEntity();
            auto &transform = entity.GetComponents<PPGE::TransformComponent>();
            transform.scale = PPGE::Math::Vector3(scale_factor, scale_factor, scale_factor);
            CreateMeshFilter(entity, vertices, indices);

            auto &mesh_renderer = entity.AddComponent<PPGE::MeshRendererComponent>();
            {
                aiReturn result;
                const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];

                aiColor4D Kd;
                result = aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &Kd);
                if (result == AI_SUCCESS)
                {
                    mesh_renderer.albedo_color = PPGE::Math::Color(Kd.r, Kd.g, Kd.b);
                }
                else
                {
                    mesh_renderer.albedo_color = PPGE::Math::Color(1.0f, 1.0f, 1.0f);
                }
                aiColor4D Ks;
                result = aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &Ks);
                if (result == AI_SUCCESS)
                {
                    mesh_renderer.specular_color = PPGE::Math::Color(Ks.r, Ks.g, Ks.b, 1.0f);
                }
                else
                {
                    mesh_renderer.specular_color = PPGE::Math::Color(1.0f, 1.0f, 1.0f);
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
            }

            vertices.clear();
            indices.clear();
        }

        aiReleaseImport(scene);
    }
};

class TestLayer : public PPGE::Widget
{
    PPGE::ResourceManager resource_mgr;

    PPGE::FreeLookCamera m_camera_controller;

    PPGE::Scene m_scene;

    PPGE::Renderer m_renderer;

  public:
    TestLayer() : Widget("TestSubsystem"), m_camera_controller{}
    {
        resource_mgr.RegisterLoaderMultiple<PPGE::TextResource, PPGE::TextLoader>({".hlsl", ".glsl"});
        resource_mgr.RegisterLoaderMultiple<PPGE::ByteResource, PPGE::ByteLoader>({".bin"});
        resource_mgr.RegisterLoaderMultiple<PPGE::LazyResource, PPGE::LazyLoader>(
            {".gltf", ".obj", ".mtl", ".bmp", ".png", ".jpg", ".dds"});
    }

    void OnAttach() override
    {
        auto camera_entity = m_scene.CreateEntity();
        auto &camera_component = camera_entity.AddComponent<PPGE::CameraComponent>();
        {
            float fov = pi * 0.333f;
            float aspect_ratio =
                (float)PPGE::DisplaySystem::Get().GetWidth() / (float)PPGE::DisplaySystem::Get().GetHeight();
            camera_component.projection =
                PPGE::Math::Matrix::CreatePerspectiveFieldOfView(fov, aspect_ratio, 0.1f, 1000.0f);
        }
        m_camera_controller.Possess(camera_entity);
        m_camera_controller.Move(PPGE::Math::Vector3(-1.5f, 1.75f, -1.5f));
        m_camera_controller.LookAt(PPGE::Math::Vector3::Zero, PPGE::Math::Vector3::Up);

        // Load resources
        resource_mgr.WalkRoot("../../Sandbox/assets");
        if (auto model = resource_mgr.GetCachedResource("cornellbox/CornellBox-Original.obj"))
        {
            auto lazy_model = std::static_pointer_cast<PPGE::LazyResource>(model);
            SceneLoader::LoadScene(lazy_model->data, m_scene);
        }

        {
            auto entity = m_scene.CreateEntity();
            {
                std::vector<PPGE::FullVertex> vertices{
                    {.px =  1.0f, .py = 0.0f, .pz =  1.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .color = 0x1199eeff, .u1 = 0.0f, .v1 = 0.0f},
                    {.px =  1.0f, .py = 0.0f, .pz = -1.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .color = 0x11ee99ff, .u1 = 1.0f, .v1 = 0.0f},
                    {.px = -1.0f, .py = 0.0f, .pz = -1.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .color = 0x1199eeff, .u1 = 1.0f, .v1 = 1.0f},
                    {.px = -1.0f, .py = 0.0f, .pz =  1.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .color = 0xee1199ff, .u1 = 0.0f, .v1 = 1.0f}};
                std::vector<unsigned int> indices{0, 3, 2, 2, 1, 0};
                CreateMeshFilter(entity, vertices, indices);
            }
            if (auto resource = resource_mgr.GetCachedResource("textures/landscape0_albedo.dds"))
            {  
                auto lazy_resource = std::static_pointer_cast<PPGE::LazyResource>(resource);
                auto &mesh_renderer = entity.AddComponent<PPGE::MeshRendererComponent>();
                mesh_renderer.albedo_map = LoadTexture(lazy_resource->data);
                mesh_renderer.specular_color = PPGE::Math::Color(.85f, .85f, .85f, 10.0f);
            }

            auto &transform = entity.GetComponents<PPGE::TransformComponent>();
            transform.position = PPGE::Math::Vector3(0.0f, -0.1f, 0.0f);
            transform.scale = PPGE::Math::Vector3(5.0f, 1.0f, 5.0f);
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
    }

    void OnInputEvent(PPGE::InputEvent &inputEvent) override
    {
        PPGE::DispatchInputEvent<PPGE::KeyPressedEvent>(
            inputEvent, PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(TestLayer::HandleKeyPressedEvenet));
    }

    bool HandleKeyPressedEvenet(PPGE::KeyPressedEvent &e)
    {
        if (e.Keycode() == PPGE_KEY_J)
        {
            Destroy();
            return true;
        }
        return false;
    }
};

class Game : public PPGE::Application
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

PPGE::Application *PPGE::CreateApplication()
{
    return new Game();
}
