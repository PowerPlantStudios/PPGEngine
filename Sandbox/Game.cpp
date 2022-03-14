#include <PPGE.h>

static std::string_view vs_code =
    R"(
cbuffer g_PerFrame : register(b0)
{
	float4x4 modelViewProj;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float3 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float3 Color : COLOR;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
    vout.PosH  = mul(float4(vin.PosL, 1.0), modelViewProj);
	vout.Color = vin.Color; 
    return vout;
}
)";

static std::string_view ps_code =
    R"(
struct FragmentIn
{
	float4 PosH  : SV_POSITION;
    float3 Color : COLOR;
};

float4 main(FragmentIn pin) : SV_Target
{
   return float4(pin.Color, 1.0);
}
)";

constexpr float pi = 3.14159265359f;

class Camera2D
{
  public:
    Camera2D()
    {
        m_view = PPGE::Math::Matrix::Identity;
        m_proj = PPGE::Math::Matrix::CreateOrthographic(PPGE::DisplaySystem::Get().GetWidth(),
                                                        PPGE::DisplaySystem::Get().GetHeight(), 0.0f, 1000.0f);
    }

    void MoveLeft(float delta)
    {
        m_pos -= PPGE::Math::Vector3(speed * delta, 0, 0);
        UpdateTranslation();
    }

    void MoveRight(float delta)
    {
        m_pos += PPGE::Math::Vector3(speed * delta, 0, 0);
        UpdateTranslation();
    }

    void MoveUp(float delta)
    {
        m_pos += PPGE::Math::Vector3(0, speed * delta, 0);
        UpdateTranslation();
    }

    void MoveDown(float delta)
    {
        m_pos -= PPGE::Math::Vector3(0, speed * delta, 0);
        UpdateTranslation();
    }

    inline PPGE::Math::Matrix GetViewProj() const
    {
        return m_proj * m_view;
    }

  private:
    void UpdateTranslation()
    {
        m_view.Translation(m_pos);
    }

    PPGE::Math::Matrix m_view;
    PPGE::Math::Matrix m_proj;

    PPGE::Math::Vector3 m_pos;

    float speed = 1.0f;
};

class TestLayer : public PPGE::Widget
{
    std::shared_ptr<PPGE::PPGEBuffer> m_vb;
    std::shared_ptr<PPGE::PPGEBuffer> m_ib;
    std::shared_ptr<PPGE::PPGEBuffer> m_cb;
    std::shared_ptr<PPGE::PPGEPipelineState> m_PSO;
    std::shared_ptr<PPGE::PPGEShaderResourceBinding> m_SRB;

    Camera2D m_camera_controller;

  public:
    TestLayer() : Widget("TestSubsystem"), m_camera_controller{}
    {
    }

    void OnAttach() override
    {
        // Create vertex buffer
        {
            struct PosColorVertex
            {
                float x, y, z;
                float r, g, b;
            };
            PosColorVertex tri_vertices[] = {
                {-100.0f, -100.0f, 0.0f, 0.8f, 0.5f, 0.3f},
                {+100.0f, -100.0f, 0.0f, 0.2f, 0.5f, 0.8f},
                {-100.0f, +100.0f, 0.0f, 0.4f, 0.1f, 0.7f},
                {+100.0f, +100.0f, 0.0f, 0.1f, 0.8f, 0.1f}
            };

            PPGE::BufferDesc vb_desc;
            vb_desc.byte_width = sizeof(tri_vertices);
            vb_desc.bind_flags = PPGE::BindFlags::BIND_VERTEX_BUFFER;
            vb_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
            vb_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
            PPGE::BufferData init_data;
            init_data.data_ptr = tri_vertices;
            init_data.data_size = sizeof(tri_vertices);
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(vb_desc, &init_data, m_vb);
        }

        // Create index buffer
        {
            uint16_t tri_indices[] = {0, 2, 1, 1, 2, 3};

            PPGE::BufferDesc ib_desc;
            ib_desc.byte_width = sizeof(tri_indices);
            ib_desc.bind_flags = PPGE::BindFlags::BIND_INDEX_BUFFER;
            ib_desc.usage = PPGE::UsageType::USAGE_IMMUTABLE;
            ib_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_NONE;
            PPGE::BufferData init_data;
            init_data.data_ptr = tri_indices;
            init_data.data_size = sizeof(tri_indices);
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(ib_desc, &init_data, m_ib);
        }

        // Create constant buffer (uniform to bind vertex shader)
        {
            PPGE::BufferDesc cb_desc;
            cb_desc.byte_width = 64;
            cb_desc.bind_flags = PPGE::BindFlags::BIND_CONSTANT_BUFFER;
            cb_desc.usage = PPGE::UsageType::USAGE_DYNAMIC;
            cb_desc.cpu_access_flags = PPGE::CPUAccessFlags::CPU_ACCESS_WRITE;
            PPGE::RendererSystem::Get().GetDevice()->CreateBuffer(cb_desc, nullptr, m_cb);
        }

        {
            std::shared_ptr<PPGE::PPGEShader> vs;
            {
                PPGE::ShaderCreateDesc cd;
                cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX;
                cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
                cd.source_code = vs_code.data();
                cd.source_code_size = vs_code.size();
                PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, vs);
            }

            std::shared_ptr<PPGE::PPGEShader> ps;
            {
                PPGE::ShaderCreateDesc cd;
                cd.desc.shader_type_flags = PPGE::ShaderTypeFlags::SHADER_TYPE_PIXEL;
                cd.compiler = PPGE::ShaderCompilerType::SHADER_COMPILER_FXC;
                cd.source_code = ps_code.data();
                cd.source_code_size = ps_code.size();
                PPGE::RendererSystem::Get().GetDevice()->CreateShader(cd, ps);
            }

            PPGE::GfxPipelineStateCreateDesc ps_cd;
            ps_cd.commited_vs = vs;
            ps_cd.commited_ps = ps;

            PPGE::ShaderResourceCreateDesc SRVs[] = {{"c_buffer",
                                                      {PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX,
                                                       PPGE::ShaderResourceType::SHADER_RESOURCE_CONSTANT_BUFFER}}};
            ps_cd.srv = SRVs;
            ps_cd.srv_num = (sizeof(SRVs) / sizeof(PPGE::ShaderResourceCreateDesc));

            PPGE::RendererSystem::Get().GetDevice()->CreatePipelineState(ps_cd, m_PSO);
        }

        m_PSO->CreateShaderResourceBinding(m_SRB);
        m_SRB->GetVariableByName("c_buffer", PPGE::ShaderTypeFlags::SHADER_TYPE_VERTEX)->Set(m_cb);

        PPGE::RendererSystem::Get().GetImmediateContext()->SetVertexBuffers(1, m_vb.get());
        PPGE::RendererSystem::Get().GetImmediateContext()->SetIndexBuffer(m_ib.get());
        PPGE::RendererSystem::Get().GetImmediateContext()->SetPipelineObject(m_PSO.get());
        PPGE::RendererSystem::Get().GetImmediateContext()->SetShaderResourceBinding(m_SRB.get());
    }

    void OnDetach() override
    {
    }

    void OnUpdate(float delta_time) override
    {
        if (PPGE::Input::IsKeyPressed(PPGE_KEY_W))
        {
            m_camera_controller.MoveUp(delta_time);
        }
        else if (PPGE::Input::IsKeyPressed(PPGE_KEY_S))
        {
            m_camera_controller.MoveDown(delta_time);
        }

        if (PPGE::Input::IsKeyPressed(PPGE_KEY_D))
        {
            m_camera_controller.MoveRight(delta_time);
        }
        else if (PPGE::Input::IsKeyPressed(PPGE_KEY_A))
        {
            m_camera_controller.MoveLeft(delta_time);
        }
    }

    void OnRender() override
    {
        {
            PPGE::Math::Matrix *map_data;
            PPGE::RendererSystem::Get().GetImmediateContext()->Map(m_cb.get(), PPGE::MapType::MAP_WRITE,
                                                                   PPGE::MapFlags::MAP_DISCARD,
                                                                   reinterpret_cast<void **>(&map_data));
            *map_data = m_camera_controller.GetViewProj().Transpose();
            PPGE::RendererSystem::Get().GetImmediateContext()->Unmap(m_cb.get());
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