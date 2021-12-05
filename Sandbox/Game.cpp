#include <PPGE.h>

class TestLayer : public PPGE::UILayer
{
    PPGE::Math::Vector2 vector2;
    PPGE::VertexBufferHandle m_vertexbuffer;
    PPGE::IndexBufferHandle m_indexbuffer;
    PPGE::ProgramHandle m_program;
    PPGE::SceneData m_scenedata;
    PPGE::RenderStates m_rendererstates;

  public:
    TestLayer() : UILayer("TestSubsystem"), vector2(0, 0), m_vertexbuffer(), m_indexbuffer()
    {
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

        PosColorVertex cube_vertices[] = {
            {-1.0f, 1.0f, 1.0f, 0xff000000},   // v0
            {1.0f, 1.0f, 1.0f, 0xff0000ff},    // v1
            {-1.0f, -1.0f, 1.0f, 0xff00ff00},  // v2
            {1.0f, -1.0f, 1.0f, 0xff00ffff},   // v3
            {-1.0f, 1.0f, -1.0f, 0xffff0000},  // v4
            {1.0f, 1.0f, -1.0f, 0xffff00ff},   // v5
            {-1.0f, -1.0f, -1.0f, 0xffffff00}, // v6
            {1.0f, -1.0f, -1.0f, 0xffffffff},  // v7
        };

        PPGE::VertexBufferDesc vb_desc;
        vb_desc.m_data = &cube_vertices;
        vb_desc.m_size = sizeof(cube_vertices);
        vb_desc.m_layout =
            PPGE::VertexLayout{{PPGE::VertexLayout::Attribute::Position, PPGE::VertexLayout::Type::Float, 3},
                               {PPGE::VertexLayout::Attribute::Color0, PPGE::VertexLayout::Type::Uint8, 4}};
        m_vertexbuffer = PPGE::Renderer::CreateVertexBuffer(vb_desc);

        uint16_t tri_indices[] = {
            0, 1, 2, // 0
            1, 3, 2, // 1
            4, 6, 5, // 2
            5, 6, 7, // 3
            0, 2, 4, // 4
            4, 2, 6, // 5
            1, 5, 3, // 6
            5, 7, 3, // 7
            0, 4, 1, // 8
            4, 5, 1, // 9
            2, 3, 6, // 10
            6, 3, 7, // 11
        };

        PPGE::IndexBufferDesc ib_desc;
        ib_desc.m_data = &tri_indices;
        ib_desc.m_size = sizeof(tri_indices);
        m_indexbuffer = PPGE::Renderer::CreateIndexBuffer(ib_desc);
    }

    void OnUpdate(float timestamp) override
    {
        if (PPGE::Input::IsKeyPressed(PPGE_KEY_E))
        {
            APP_TRACE("Key E is pressed. Recorded mouse positions x-{0} y-{1}", vector2.x, vector2.y);
        }
        if (PPGE::Input::IsMouseButtonPressed(PPGE_MOUSE_BUTTON_0))
        {
            auto x_pose = PPGE::Input::GetMouseX();
            APP_TRACE("Mouse x - {0} ", x_pose);
            vector2.x = x_pose;
        }
        if (PPGE::Input::IsMouseButtonPressed(PPGE_MOUSE_BUTTON_1))
        {
            auto y_pose = PPGE::Input::GetMouseY();
            APP_TRACE("Mouse y - {0} ", y_pose);
            vector2.y = y_pose;
        }

        PPGE::Renderer::BeginScene(m_scenedata);
        {
            PPGE::Renderer::SetVertexBuffer(m_vertexbuffer);
            PPGE::Renderer::SetIndexBuffer(m_indexbuffer);
            PPGE::Renderer::SetRenderStates(m_rendererstates);
            PPGE::Renderer::Submit(m_program);
        }
        PPGE::Renderer::EndScene();

        // Then continue rest of the rendering ...
    }

    void OnInputEvent(PPGE::InputEvent &inputEvent) override
    {
        // APP_INFO("Input event is received by the test system.");
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