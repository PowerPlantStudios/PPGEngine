#include <PPGE.h>

static std::string_view vs_code =
    R"(
cbuffer g_PerFrame : register(b0)
{
	float4x4 view;
	float4x4 proj;
	float4x4 viewProj;
};

cbuffer g_PerObject : register(b1)
{
	float4x4 world;
};

struct VertexIn
{
	float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	vout.PosH = float4(vin.PosL, 1.0f);
    vout.PosH = mul(vout.PosH, world);
    vout.PosH = mul(vout.PosH, viewProj);
	vout.Color = vin.Color; 
    return vout;
}
)";

static std::string_view ps_code_1 =
    R"(
struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

float4 main(VertexOut pin) : SV_Target
{
    return pin.Color;
}
)";

static std::string_view ps_code_2 =
    R"(
struct VertexOut
{
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
};

float4 main(VertexOut pin) : SV_Target
{
    return float4(0.2, 0.3, 0.1, 1.0);
}
)";

constexpr float pi = 3.14159265359f;

class CameraController
{
  public:
    CameraController()
        : m_camera(), m_move_speed(5.0f), m_rotation_speed(10.f), m_position(0.0f, 0.0f, 5.0f),
          m_forward(PPGE::Math::Vector3::Forward), m_up(PPGE::Math::Vector3::Up), m_right(PPGE::Math::Vector3::Right),
          m_yaw(0.0f / 180.f * pi), m_pitch(0.0f / 180.f * pi)
    {
        m_camera.SetPerpectiveFOV(0.4f * 3.14f, 1280.0f / 720.0f, 0.1f, 1000.0f);
        UpdateView();
    }

    void UpdateView()
    {
        auto rotCamera = PPGE::Math::Matrix::CreateFromYawPitchRoll(m_yaw, m_pitch, 0.0f);
        auto target = PPGE::Math::Vector3::Transform(PPGE::Math::Vector3::Forward, rotCamera);
        target.Normalize();

        m_right = PPGE::Math::Vector3::Transform(PPGE::Math::Vector3::Right, rotCamera);
        m_forward = PPGE::Math::Vector3::Transform(PPGE::Math::Vector3::Forward, rotCamera);
        m_up = m_right.Cross(m_forward);

        m_view = PPGE::Math::Matrix::CreateLookAt(m_position, m_position + target, m_up);
    }

    void MoveForward(float delta_time)
    {
        m_position += m_forward * delta_time * m_move_speed;
        UpdateView();
    }

    void MoveBackward(float delta_time)
    {
        m_position -= m_forward * delta_time * m_move_speed;
        UpdateView();
    }

    void MoveRight(float delta_time)
    {
        m_position += m_right * delta_time * m_move_speed;
        UpdateView();
    }

    void MoveLeft(float delta_time)
    {
        m_position -= m_right * delta_time * m_move_speed;
        UpdateView();
    }

    void RotatePitch(float delta)
    {
        m_pitch += (m_rotation_speed * delta / 180.0f * pi);
        UpdateView();
    }

    void RotateYaw(float delta)
    {
        m_yaw += (m_rotation_speed * delta / 180.0f * pi);
        UpdateView();
    }

    inline PPGE::Math::Matrix GetPorojection() const
    {
        return m_camera.GetProjection();
    }

    inline PPGE::Math::Matrix GetView() const
    {
        return m_view;
    }

  private:
    PPGE::Camera m_camera;
    PPGE::Math::Matrix m_view;

    float m_move_speed;
    float m_rotation_speed;

    PPGE::Math::Vector3 m_position;
    PPGE::Math::Vector3 m_up;
    PPGE::Math::Vector3 m_forward;
    PPGE::Math::Vector3 m_right;

    float m_yaw;
    float m_pitch;
};

class TestLayer : public PPGE::UILayer
{
    PPGE::VertexBufferHandle m_vertexbuffer;
    PPGE::IndexBufferHandle m_indexbuffer;
    PPGE::ProgramHandle m_program;
    PPGE::UniformHandle m_viewproj;
    PPGE::UniformHandle m_model;
    PPGE::RenderStates m_rendererstates;
    PPGE::SceneData m_scenedata;

    CameraController m_camera_controller;

    float m_mouse_x, m_mouse_y;
    float m_sping_angle;

  public:
    TestLayer()
        : UILayer("TestSubsystem"), m_vertexbuffer{}, m_indexbuffer{}, m_program{}, m_rendererstates{}, m_scenedata{},
          m_camera_controller{}, m_viewproj{}, m_mouse_x{0.0f}, m_mouse_y{0.0f}, m_sping_angle{0.0f}
    {
        m_scenedata.m_projection = m_camera_controller.GetPorojection().Transpose();
        m_scenedata.m_view = m_camera_controller.GetView().Transpose();
    }

    void OnAttach() override
    {
        struct PosColorVertex
        {
            float m_x;
            float m_y;
            float m_z;
            uint32_t m_abgr;
        };

        PosColorVertex tri_vertices[] = {{-1.0f, 1.0f, 1.0f, 0xffaaffff},   {1.0f, 1.0f, 1.0f, 0xffaaff66},
                                         {-1.0f, -1.0f, 1.0f, 0xffffaaff},  {1.0f, -1.0f, 1.0f, 0xffffffaa},
                                         {-1.0f, 1.0f, -1.0f, 0xffffffaa},  {1.0f, 1.0f, -1.0f, 0xffffaaff},
                                         {-1.0f, -1.0f, -1.0f, 0xffaaff66}, {1.0f, -1.0f, -1.0f, 0xffaaffff}};

        PPGE::VertexBufferDesc vb_desc;
        vb_desc.m_layout =
            PPGE::VertexLayout{{PPGE::VertexLayout::Attribute::Position, PPGE::VertexLayout::Type::Float, 3},
                               {PPGE::VertexLayout::Attribute::Color0, PPGE::VertexLayout::Type::Uint8, 4}};

        vb_desc.m_resource.m_pData = &tri_vertices;
        vb_desc.m_resource.m_size = sizeof(tri_vertices);
        m_vertexbuffer = PPGE::Renderer::CreateVertexBuffer(vb_desc);

        uint16_t tri_indices[] = {
            0, 1, 2,          // 0
            1, 3, 2, 4, 6, 5, // 2
            5, 6, 7, 0, 2, 4, // 4
            4, 2, 6, 1, 5, 3, // 6
            5, 7, 3, 0, 4, 1, // 8
            4, 5, 1, 2, 3, 6, // 10
            6, 3, 7,
        };

        PPGE::IndexBufferDesc ib_desc;
        ib_desc.m_resource.m_pData = &tri_indices;
        ib_desc.m_resource.m_size = sizeof(tri_indices);
        ib_desc.m_index_cout = sizeof(tri_indices) / sizeof(uint16_t);
        m_indexbuffer = PPGE::Renderer::CreateIndexBuffer(ib_desc);

        PPGE::ShaderDesc shader_desc;

        shader_desc.m_type = PPGE::ShaderDesc::ShaderType::VS;
        shader_desc.m_data = vs_code.data();
        shader_desc.m_size = vs_code.size();
        PPGE::ShaderHandle vs_handle = PPGE::Renderer::CreateShader(shader_desc);

        shader_desc.m_type = PPGE::ShaderDesc::ShaderType::PS;
        shader_desc.m_data = ps_code_1.data();
        shader_desc.m_size = ps_code_1.size();
        PPGE::ShaderHandle ps_handle = PPGE::Renderer::CreateShader(shader_desc);

        PPGE::ProgramDesc pg_desc;
        pg_desc.m_vertex_shader_handle = vs_handle;
        pg_desc.m_pixel_shader_handle = ps_handle;
        m_program = PPGE::Renderer::CreateProgram(pg_desc);

        PPGE::UniformDesc un_desc;
        un_desc.m_name = "g_viewproj";
        un_desc.m_type = PPGE::UniformDesc::UniformType::Mat4;
        un_desc.m_num = 2;
        m_viewproj = PPGE::Renderer::CreateUniform(un_desc);

        un_desc.m_name = "g_model";
        un_desc.m_type = PPGE::UniformDesc::UniformType::Mat4;
        un_desc.m_num = 1;
        m_model = PPGE::Renderer::CreateUniform(un_desc);
    }

    void OnUpdate(float delta_time) override
    {
        float current_mouse_x = PPGE::Input::GetMouseX();
        float delta_x = m_mouse_x - current_mouse_x;
        m_mouse_x = current_mouse_x;

        float current_mouse_y = PPGE::Input::GetMouseY();
        float delta_y = m_mouse_y - current_mouse_y;
        m_mouse_y = current_mouse_y;

        if (PPGE::Input::IsKeyPressed(PPGE_KEY_W))
        {
            m_camera_controller.MoveForward(delta_time);
        }
        else if (PPGE::Input::IsKeyPressed(PPGE_KEY_S))
        {
            m_camera_controller.MoveBackward(delta_time);
        }

        if (PPGE::Input::IsKeyPressed(PPGE_KEY_D))
        {
            m_camera_controller.MoveRight(delta_time);
        }
        else if (PPGE::Input::IsKeyPressed(PPGE_KEY_A))
        {
            m_camera_controller.MoveLeft(delta_time);
        }

        if (PPGE::Input::IsMouseButtonPressed(PPGE_MOUSE_BUTTON_0))
        {
            m_camera_controller.RotatePitch(delta_time * delta_y);
            m_camera_controller.RotateYaw(delta_time * delta_x);
        }

        struct g_ViewProj
        {
            PPGE::Math::Matrix m_view;
            PPGE::Math::Matrix m_proj;
        } viewProjData;

        struct g_Model
        {
            PPGE::Math::Matrix m_model;
        } modelData;

        m_scenedata.m_view = m_camera_controller.GetView().Transpose();
        PPGE::Renderer::BeginScene(m_scenedata);
        {
            m_sping_angle += (delta_time * 45.5f / 180.0f * pi);
            if (m_sping_angle > 360.0f)
                m_sping_angle = 0.0f;
            auto rot_anim = PPGE::Math::Matrix::CreateRotationY(m_sping_angle).Transpose();

            for (unsigned int i = 0; i < 29; i++)
            {
                for (unsigned int j = 0; j < 29; j++)
                {
                    PPGE::Renderer::SetVertexBuffer(m_vertexbuffer);
                    PPGE::Renderer::SetIndexBuffer(m_indexbuffer);

                    PPGE::Renderer::SetRenderStates(m_rendererstates);

                    auto trns_anim =
                        PPGE::Math::Matrix::CreateTranslation(i * 3.5f - 49.0f, 0.0f, j * 3.5f - 49.0f).Transpose();
                    PPGE::Subresource subresource;
                    modelData.m_model = trns_anim * rot_anim;
                    subresource.m_pData = &modelData;
                    subresource.m_size = sizeof(modelData);
                    PPGE::Renderer::UpdateUniform(m_model, subresource);
                    PPGE::Renderer::SetUniform(m_model, PPGE::UniformDesc::Target::VS, 1);

                    PPGE::Renderer::Submit(m_program);
                }
            }
        }
        PPGE::Renderer::EndScene();
    }

    void OnDetach() override
    {
        PPGE::Renderer::ReleaseVertexBuffer(m_vertexbuffer);
        PPGE::Renderer::ReleaseIndexBuffer(m_indexbuffer);
    }

    void OnRender() override
    {
    }

    void OnInputEvent(PPGE::InputEvent &inputEvent) override
    {
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
        PushLayerBack(std::make_unique<TestLayer>());
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