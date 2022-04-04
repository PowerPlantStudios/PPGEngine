#include <PPGE.h>

constexpr float pi = 3.14159265359f;

class SceneLoader
{
  public:
    struct Vertex
    {
        float px, py, pz;
        float nx, ny, nz;
        float tu, tv;
    };

    static void LoadScene(const std::filesystem::path &path_to_scene, std::vector<Vertex> &vertices,
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
            const aiVector3D v = mesh->mVertices[i];
            const aiVector3D n = mesh->mNormals[i];
            const aiVector3D t = mesh->mTextureCoords[0][i];
            vertices.push_back(
                {.px = v.x, .py = v.z, .pz = v.y, .nx = n.x, .ny = n.y, .nz = n.z, .tu = t.x, .tv = t.y});
        }
        for (unsigned i = 0; i != mesh->mNumFaces; i++)
        {
            for (unsigned j = 0; j != 3; j++)
                indices.push_back(mesh->mFaces[i].mIndices[j]);
        }

        aiReleaseImport(scene);
    }
};

class TestLayer : public PPGE::Widget
{
    PPGE::ResourceManager resource_mgr;

    std::shared_ptr<PPGE::PPGEBuffer> m_vb_model;
    std::shared_ptr<PPGE::PPGEBuffer> m_vb_ground;

    std::shared_ptr<PPGE::PPGEBuffer> m_ib_model;
    std::shared_ptr<PPGE::PPGEBuffer> m_ib_ground;

    std::shared_ptr<PPGE::PPGEBuffer> m_cb_per_frame;

    std::shared_ptr<PPGE::PPGEBuffer> m_cb_per_object;

    std::shared_ptr<PPGE::PPGEPipelineState> m_PSO;

    std::shared_ptr<PPGE::PPGEShaderResourceBinding> m_SRB_model;
    std::shared_ptr<PPGE::PPGEShaderResourceBinding> m_SRB_ground;

    std::shared_ptr<PPGE::PPGETextureView> m_diffuse_srv_model;
    std::shared_ptr<PPGE::PPGETextureView> m_diffuse_srv_ground;

    std::shared_ptr<PPGE::PPGESampler> m_sampler_state;

    size_t m_num_indicies_model = 0;

    PPGE::FreeLookCamera m_camera_controller;

    PPGE::Scene m_scene;

    struct cbPerFrame
    {
        PPGE::Math::Matrix view;
        PPGE::Math::Matrix proj;
        PPGE::Math::Matrix viewProj;
        PPGE::Math::Vector3 cameraPosition;
        float padding;
    };

    struct cbPerObject
    {
        PPGE::Math::Matrix model;
        PPGE::Math::Matrix modelInvTran;
    };

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

        auto light_entity = m_scene.CreateEntity();
        auto &dir_light_component = light_entity.AddComponent<PPGE::DirectionalLightComponent>();

        // Load resources
        resource_mgr.WalkRoot("../../Sandbox/assets");

        // Create vertex buffer and index buffer
        {
            std::vector<SceneLoader::Vertex> vertices;
            std::vector<unsigned int> indices;

            if (auto resource = resource_mgr.GetCachedResource("models/rubber_duck/rubber_duck.gltf"))
            {
                auto lazy_resource = std::static_pointer_cast<PPGE::LazyResource>(std::move(resource));
                SceneLoader::LoadScene(lazy_resource->data, vertices,
                                       indices);
            }


            PPGE::BufferDesc vb_desc;
            vb_desc.byte_width = vertices.size() * sizeof(SceneLoader::Vertex);
            vb_desc.bind_flags = PPGE::BindFlags::BIND_VERTEX_BUFFER;
            vb_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
            vb_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
            PPGE::BufferData vb_init_data;
            vb_init_data.data_ptr = &vertices[0];
            vb_init_data.data_size = vertices.size() * sizeof(SceneLoader::Vertex);
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(vb_desc, &vb_init_data, m_vb_model);

            PPGE::BufferDesc ib_desc;
            ib_desc.byte_width = indices.size() * sizeof(unsigned int);
            ib_desc.bind_flags = PPGE::BindFlags::BIND_INDEX_BUFFER;
            ib_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
            ib_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
            PPGE::BufferData ib_init_data;
            ib_init_data.data_ptr = &indices[0];
            ib_init_data.data_size = indices.size() * sizeof(unsigned int);
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(ib_desc, &ib_init_data, m_ib_model);

            m_num_indicies_model = indices.size();
        }
        {
            std::vector<SceneLoader::Vertex> vertices{
                {.px = 8.0f, .py = 0.0f, .pz = 8.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .tu = 0.0f, .tv = 0.0f},
                {.px = 8.0f, .py = 0.0f, .pz = -8.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .tu = 0.0f, .tv = 1.0f},
                {.px = -8.0f, .py = 0.0f, .pz = -8.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .tu = 1.0f, .tv = 1.0f},
                {.px = -8.0f, .py = 0.0f, .pz = 8.0f, .nx = 0.0f, .ny = 1.0f, .nz = 0.0f, .tu = 1.0f, .tv = 0.0f}};
            std::vector<unsigned int> indices{0, 3, 2, 2, 1, 0};

            PPGE::BufferDesc vb_desc;
            vb_desc.byte_width = vertices.size() * sizeof(SceneLoader::Vertex);
            vb_desc.bind_flags = PPGE::BindFlags::BIND_VERTEX_BUFFER;
            vb_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
            vb_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
            PPGE::BufferData vb_init_data;
            vb_init_data.data_ptr = &vertices[0];
            vb_init_data.data_size = vertices.size() * sizeof(SceneLoader::Vertex);
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(vb_desc, &vb_init_data, m_vb_ground);

            PPGE::BufferDesc ib_desc;
            ib_desc.byte_width = indices.size() * sizeof(unsigned int);
            ib_desc.bind_flags = PPGE::BindFlags::BIND_INDEX_BUFFER;
            ib_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
            ib_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
            PPGE::BufferData ib_init_data;
            ib_init_data.data_ptr = &indices[0];
            ib_init_data.data_size = indices.size() * sizeof(unsigned int);
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(ib_desc, &ib_init_data, m_ib_ground);
        }

        // Create constant buffer (uniform to bind vertex shader)
        {
            PPGE::BufferDesc cb_desc;
            cb_desc.byte_width = sizeof(cbPerFrame);
            cb_desc.bind_flags = PPGE::BindFlags::BIND_CONSTANT_BUFFER;
            cb_desc.usage = PPGE::UsageType::USAGE_DYNAMIC;
            cb_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_WRITE;
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_frame);
            cb_desc.byte_width = sizeof(cbPerObject);
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb_per_object);
        }

        // Create Texture View
        if (auto resource = resource_mgr.GetCachedResource("models/rubber_duck/textures/Duck_baseColor.png"))
        {
            auto lazy_resource = std::static_pointer_cast<PPGE::LazyResource>(std::move(resource));
            std::string path_to_resource = lazy_resource->data.string();

            std::shared_ptr<PPGE::PPGETexture> texture;
            PPGE::TextureCreateDesc tex_cd;
            tex_cd.resource_path = path_to_resource.c_str();
            tex_cd.file_format = PPGE::TextureFileFormat::PNG;
            tex_cd.desc.bind_flags = PPGE::BindFlags::BIND_SHADER_RESOURCE;
            PPGE::RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, texture);
            m_diffuse_srv_model = texture->GetDefaultView();
        }
        if (auto resource = resource_mgr.GetCachedResource("textures/grass.jpg"))
        {
            auto lazy_resource = std::static_pointer_cast<PPGE::LazyResource>(std::move(resource));
            std::string path_to_resource = lazy_resource->data.string();

            std::shared_ptr<PPGE::PPGETexture> texture;
            PPGE::TextureCreateDesc tex_cd;
            tex_cd.resource_path = path_to_resource.c_str();
            tex_cd.file_format = PPGE::TextureFileFormat::JPEG;
            tex_cd.desc.bind_flags = PPGE::BindFlags::BIND_SHADER_RESOURCE;
            PPGE::RendererSystem::Get().GetDevice()->CreateTexture(tex_cd, texture);
            m_diffuse_srv_ground = texture->GetDefaultView();
        }

        // Create Sampler
        {
            PPGE::SamplerDesc sampler_desc;
            PPGE::RendererSystem::Get().GetDevice()->CreateSampler(sampler_desc, m_sampler_state);
        }

        {
            std::shared_ptr<PPGE::PPGEShader> vs;
            if (auto resource = resource_mgr.GetCachedResource("shaders/simple_vs.hlsl"))
            {
                auto text_resource = std::static_pointer_cast<PPGE::TextResource>(std::move(resource));
                PPGE::ShaderCreateDesc cd;
                cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX;
                cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
                cd.source_code = text_resource->data.data();
                cd.source_code_size = text_resource->data.size();
                PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
            }

            std::shared_ptr<PPGE::PPGEShader> ps;
            if (auto resource = resource_mgr.GetCachedResource("shaders/simple_ps.hlsl"))
            {
                auto text_resource = std::static_pointer_cast<PPGE::TextResource>(std::move(resource));
                PPGE::ShaderCreateDesc cd;
                cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL;
                cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
                cd.source_code = text_resource->data.data();
                cd.source_code_size = text_resource->data.size();
                PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, ps);
            }

            PPGE::GfxPipelineStateCreateDesc ps_cd;
            {
                PPGE::InputLayoutDesc ly_desc;
                PPGE::InputElementDesc elements[] = {
                    {"POSITION", 0, PPGE::ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
                    {"NORMAL", 0, PPGE::ElementValueType::ELEMENT_VALUE_FLOAT32, 3, 0},
                    {"TEXTURE", 0, PPGE::ElementValueType::ELEMENT_VALUE_FLOAT32, 2, 0}};
                ly_desc.elements = elements;
                ly_desc.elements_num = (sizeof(elements) / sizeof(PPGE::InputElementDesc));

                ps_cd.desc.input_layout_desc = ly_desc;
            }
            ps_cd.commited_vs = vs;
            ps_cd.commited_ps = ps;

            PPGE::ShaderResourceCreateDesc SRVs[] = {
                {"cb_per_frame",
                 {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX,
                  PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
                {"cb_per_object",
                 {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX,
                  PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}},
                {"g_diffuse",
                 {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_TEXTURE_SRV}},
                {"g_sampler",
                 {PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL, PPGE::ShaderResourceType::SHADER_RESOURCE_SAMPLER}}};
            ps_cd.srv = SRVs;
            ps_cd.srv_num = (sizeof(SRVs) / sizeof(PPGE::ShaderResourceCreateDesc));

            PPGE::RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_PSO);
        }

        m_SRB_model = m_PSO->CreateShaderResourceBinding();
        m_SRB_model->GetVariableByName("cb_per_frame", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_frame);
        m_SRB_model->GetVariableByName("cb_per_object", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)
            ->Set(m_cb_per_object);
        m_SRB_model->GetVariableByName("g_diffuse", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_diffuse_srv_model);
        m_SRB_model->GetVariableByName("g_sampler", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_sampler_state);

        m_SRB_ground = m_PSO->CreateShaderResourceBinding();
        m_SRB_ground->GetVariableByName("cb_per_frame", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb_per_frame);
        m_SRB_ground->GetVariableByName("cb_per_object", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)
            ->Set(m_cb_per_object);
        m_SRB_ground->GetVariableByName("g_diffuse", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)
            ->Set(m_diffuse_srv_ground);
        m_SRB_ground->GetVariableByName("g_sampler", PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL)->Set(m_sampler_state);

        PPGE::RendererSystem::Get().GetImmediateContext()->SetPipelineStateObject(m_PSO);
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
        {
            cbPerFrame *map_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_frame.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&map_data));
            *map_data = cbPerFrame{.view = m_camera_controller.GetView().Transpose(),
                                   .proj = m_camera_controller.GetProj().Transpose(),
                                   .viewProj = m_camera_controller.GetViewProj().Transpose(),
                                   .cameraPosition = m_camera_controller.GetPosition(),
                                   .padding = 0.0f};
            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_frame.get());
        }

        {
            std::shared_ptr<PPGE::PPGEBuffer> vbs[] = {m_vb_model};
            uint64_t offsets[] = {0};
            PPGE::RendererSystem::Get().GetImmediateContext()->SetVertexBuffers(1, vbs, offsets);
        }
        PPGE::RendererSystem::Get().GetImmediateContext()->SetIndexBuffer(m_ib_model);
        PPGE::RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB_model);
        {
            cbPerObject *map_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_object.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&map_data));
            *map_data =
                cbPerObject{.model = PPGE::Math::Matrix::Identity, .modelInvTran = PPGE::Math::Matrix::Identity};
            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_object.get());
        }
        PPGE::RendererSystem::Get().GetImmediateContext()->DrawIndexed(m_num_indicies_model);

        {
            std::shared_ptr<PPGE::PPGEBuffer> vbs[] = {m_vb_ground};
            uint64_t offsets[] = {0};
            PPGE::RendererSystem::Get().GetImmediateContext()->SetVertexBuffers(1, vbs, offsets);
        }
        PPGE::RendererSystem::Get().GetImmediateContext()->SetIndexBuffer(m_ib_ground);
        PPGE::RendererSystem::Get().GetImmediateContext()->CommitShaderResources(m_SRB_ground);
        {
            cbPerObject *map_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb_per_object.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&map_data));
            *map_data =
                cbPerObject{.model = PPGE::Math::Matrix::Identity, .modelInvTran = PPGE::Math::Matrix::Identity};
            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb_per_object.get());
        }
        PPGE::RendererSystem::Get().GetImmediateContext()->DrawIndexed(6);
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