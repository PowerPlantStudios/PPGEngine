#include <PPGE.h>

constexpr float pi = 3.14159265359f;

template<typename VertexBufferType>
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

void CreateMeshRenderer(PPGE::Entity &entity, const std::filesystem::path &path_to_diffuse_tex)
{

    std::shared_ptr<PPGE::PPGETexture> diffuse_texture;
    {
        std::string path_to_res = path_to_diffuse_tex.string();
        PPGE::TextureCreateDesc tex_cd;
        tex_cd.resource_path = path_to_res.c_str();
        tex_cd.file_format = PPGE::TextureFileFormat::PNG;
        tex_cd.desc.bind_flags = PPGE::BindFlags::BIND_SHADER_RESOURCE;
        PPGE::RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, diffuse_texture);
    }

    auto &mesh_renderer = entity.AddComponent<PPGE::MeshRendererComponent>();
    mesh_renderer.albedo_map = diffuse_texture->GetDefaultView();
}

class SceneLoader
{
  public:
    static void LoadScene(const std::filesystem::path &path_to_scene, std::vector<PPGE::FullVertex> &vertices,
                          std::vector<unsigned int> &indices)
    {
        const aiScene *scene = aiImportFile(path_to_scene.string().c_str(), aiProcess_Triangulate);

        if (!scene || !scene->HasMeshes())
        {
            APP_ERROR("Unable to load {0}\n", path_to_scene.string().c_str());
            return;
        }

        const aiMesh *mesh = scene->mMeshes[0];
        for (unsigned i = 0; i != mesh->mNumVertices; i++)
        {
            aiVector3D v = mesh->mVertices[i];
            aiVector3D n(0.0f, 0.0f, 0.0f);
            if (mesh->HasNormals())
                n = mesh->mNormals[i];
            aiVector3D t(0.0f, 0.0f, 0.0f);
            if (mesh->HasTextureCoords(0))
                t = mesh->mTextureCoords[0][i];
            vertices.push_back(
                {.px = v.x, .py = v.z, .pz = v.y, .nx = n.x, .ny = n.y, .nz = n.z, .color = 0xffffffff});
        }
        for (unsigned i = 0; i != mesh->mNumFaces; i++)
        {
            for (unsigned j = 0; j != 3; j++)
                indices.push_back(mesh->mFaces[i].mIndices[j]);
        }

        aiReleaseImport(scene);
    }

    static void LoadScene(const std::filesystem::path &path_to_scene, const std::filesystem::path &path_to_texture,
                          PPGE::Scene &ppge_scene)
    {
        std::vector<PPGE::VertexPosNorColor> vertices;
        std::vector<unsigned int> indices;

        const aiScene *scene = aiImportFile(path_to_scene.string().c_str(), aiProcess_Triangulate);

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
            CreateMeshFilter(entity, vertices, indices);

            const aiMaterial *mat = scene->mMaterials[mesh->mMaterialIndex];
            aiColor4D Kd, Ks;
            aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &Kd);
            aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &Ks);

            auto &mesh_renderer = entity.AddComponent<PPGE::MeshRendererComponent>();
            mesh_renderer.albedo_color = PPGE::Math::Color(Kd.r, Kd.g, Kd.b);
            mesh_renderer.specular_color = PPGE::Math::Color(Ks.r, Ks.g, Ks.b);

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
        {
            auto entity = m_scene.CreateEntity();
            {
                std::vector<PPGE::VertexPosColor> vertices{
                    {.px =  8.0f, .py = 0.0f, .pz =  8.0f, .color = 0xffffffff},
                    {.px =  8.0f, .py = 0.0f, .pz = -8.0f, .color = 0xffffffff},
                    {.px = -8.0f, .py = 0.0f, .pz = -8.0f, .color = 0xffffffff},
                    {.px = -8.0f, .py = 0.0f, .pz =  8.0f, .color = 0xffffffff}};
                std::vector<unsigned int> indices{0, 3, 2, 2, 1, 0};
                CreateMeshFilter(entity, vertices, indices);
            }
            auto &mesh_renderer = entity.AddComponent<PPGE::MeshRendererComponent>();
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
