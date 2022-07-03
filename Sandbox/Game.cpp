#define PPGE_APP_ENTRY_POINT
#include <PPGE.h>

constexpr float PI = 3.14159265359f;

static size_t ENTITY_COUNT = 0;

namespace PPGE
{
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
        RendererOptions options = m_renderer.GetRendererOptions();
        options |= RendererOptions::ENABLE_NORMAL_MAP;
        m_renderer.SetRendererOptions(options);

        // Create camera
        auto camera_entity = m_scene.CreateEntity("Camera");
        auto &camera_component = camera_entity.AddComponent<CameraComponent>();
        {
            float fov = PI * 0.333f;
            float aspect_ratio = (float)DisplaySystem::Get().GetWidth() / (float)DisplaySystem::Get().GetHeight();
            camera_component.projection = Math::Matrix::CreatePerspectiveFieldOfView(fov, aspect_ratio, 0.1f, 1000.0f);
        }
        m_camera_controller.Possess(camera_entity);
        m_camera_controller.Move(Math::Vector3(-2.0f, 2.0f, -2.0f));
        m_camera_controller.LookAt(Math::Vector3::Zero, Math::Vector3::Up);

        // Create lights
        {
            m_light = m_scene.CreateEntity("Sun Light");
            auto &transform = m_light.GetComponents<TransformComponent>();
            transform.position = Math::Vector3(2.0f, 2.0f, 2.0f);
            transform.rotation = Math::Quaternion::CreateFromYawPitchRoll(-3 * PI / 4, 2 * PI / 5, 0.0f);
            auto &point_light = m_light.AddComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL, false);
            point_light.color = Math::Color(1.00f, 0.96f, 0.84f);
            point_light.intensity = 1.5f;
        }

        {
            m_light = m_scene.CreateEntity("Light");
            auto &transform = m_light.GetComponents<TransformComponent>();
            transform.position = Math::Vector3(-.75f, 2.0f, -.75f);
            transform.rotation = Math::Quaternion::CreateFromYawPitchRoll(1 * PI / 4, 2 * PI / 5, 0.0f);
            auto &point_light = m_light.AddComponent<LightComponent>(LightComponent::LightType::SPOT);
            point_light.color = Math::Color(1.0f, 1.0f, 1.0f);
            point_light.intensity = 20.0f;
        }

        resource_mgr.WalkRoot("../../Sandbox/assets");
        if (auto model = resource_mgr.GetCachedResource("flight_helmet/flight_helmet_joint.gltf"))
        {
            auto lazy_model = std::static_pointer_cast<LazyResource>(model);
            auto entity = m_scene.CreateEntity("Damaged Helmet");
            ImportHelper::ImportModel(*lazy_model, entity);
        }

        auto entity = m_scene.CreateEntity("Ground");
        {
            auto &mesh_filter = entity.AddComponent<MeshFilterComponent>();
            {
                std::vector<std::vector<StandardVertex>> vertices;
                std::vector<unsigned int> indices;
                auto &v = vertices.emplace_back();
                GeometryPrimitives::CreateQuad(v, indices);
                mesh_filter.mesh = MeshHelper::CreateMesh(vertices, indices);
            }

            auto &mesh_renderer = entity.AddComponent<MeshRendererComponent>();
            {
                PBRMaterial::MaterialDesc desc{.albedo_color = Math::Color(.15f, .20f, .40f),
                                               .emissive_color = Math::Color(0.0f, 0.0f, 0.0f),
                                               .roughness_factor = 1.0f,
                                               .metalic_factor = 0.0f,
                                               .alpha_cutoff = 1.0f};
                auto material = MaterialHelper::CreateMaterial<PBRMaterial>(desc);

                //if (auto resource = resource_mgr.GetCachedResource("textures/landscape1_albedo.dds"))
                //{
                //    auto lazy_resource = std::static_pointer_cast<LazyResource>(resource);
                //    if (auto map = MaterialHelper::LoadTexture(lazy_resource->data))
                //        material->SetAlbedoMap(std::move(map));
                //}

                //if (auto resource = resource_mgr.GetCachedResource("textures/landscape1_normal.dds"))
                //{
                //    auto lazy_resource = std::static_pointer_cast<LazyResource>(resource);
                //    if (auto map = MaterialHelper::LoadTexture(lazy_resource->data))
                //        material->SetNormalMap(std::move(map));
                //}

                mesh_renderer.material = std::move(material);
            }

            auto &transform = entity.GetComponents<TransformComponent>();
            transform.position = Math::Vector3(0.0f, -1.5f, 0.0f);
            transform.scale = Math::Vector3(5.0f, 1.0f, 5.0f);
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
        // ImGui::Begin("Light Debug");

        // auto &transform = m_light.GetComponents<TransformComponent>();
        // DrawVectorController("Position", transform.position, 0.0f, 100.0f);

        // if (ImGui::Checkbox("Enable Normal Map", &is_normal_map_enabled))
        //{
        //     if (is_normal_map_enabled)
        //     {
        //         RendererOptions options = m_renderer.GetRendererOptions();
        //         options |= RendererOptions::ENABLE_NORMAL_MAP;
        //         m_renderer.SetRendererOptions(options);
        //     }
        //     else
        //     {
        //         RendererOptions options = m_renderer.GetRendererOptions();
        //         options &= ~RendererOptions::ENABLE_NORMAL_MAP;
        //         m_renderer.SetRendererOptions(options);
        //     }
        // }

        // ImGui::End();
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
