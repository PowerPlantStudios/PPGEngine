#include "editor.h"

namespace PPGE
{
constexpr float PI = 3.14159265359f;

static size_t entity_count = 0;

EditorWidget::EditorWidget() : Widget("TestSubsystem"), m_camera_controller{}
{
    resource_mgr.RegisterLoaderMultiple<TextResource, TextLoader>({".hlsl", ".glsl"});
    resource_mgr.RegisterLoaderMultiple<ByteResource, ByteLoader>({".bin"});
    resource_mgr.RegisterLoaderMultiple<LazyResource, LazyLoader>(
        {".gltf", ".obj", ".mtl", ".bmp", ".png", ".jpg", ".dds"});
}

void EditorWidget::OnAttach()
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
        auto light = m_scene.CreateEntity("Directional Light");
        auto &transform = light.GetComponents<TransformComponent>();
        transform.position = Math::Vector3(1.0f, 1.0f, 1.0f);
        transform.rotation = Math::Quaternion::CreateFromYawPitchRoll(-3 * PI / 4, 2 * PI / 5, 0.0f);
        auto &light_component = light.AddComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL);
        light_component.color = Math::Color(1.00f, 0.96f, 0.84f);
        light_component.intensity = 1.5f;
    }

    // Load resources
    resource_mgr.WalkRoot("../../Sandbox/assets");
    if (auto model = resource_mgr.GetCachedResource("cube/cube.obj"))
    {
        auto lazy_model = std::static_pointer_cast<LazyResource>(model);
        auto entity = m_scene.CreateEntity("Entity " + std::to_string(++entity_count));
        ImportHelper::ImportModel(*lazy_model, entity);
    }

    auto entity = m_scene.CreateEntity("Entity " + std::to_string(++entity_count));
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
            PBRMaterial::MaterialDesc desc{.albedo_color = Math::Color(1.0f, 1.0f, 1.0f),
                                           .emissive_color = Math::Color(1.0f, 1.0f, 1.0f),
                                           .roughness_factor = 1.0f,
                                           .metalic_factor = 1.0f,
                                           .alpha_cutoff = 1.0f};
            auto material = MaterialHelper::CreateMaterial<PBRMaterial>(desc);

            if (auto resource = resource_mgr.GetCachedResource("textures/no_texture.png"))
            {
                auto lazy_resource = std::static_pointer_cast<LazyResource>(resource);
                if (auto map = MaterialHelper::LoadTexture(lazy_resource->data))
                    material->SetEmissionMap(std::move(map));
            }
            mesh_renderer.material = std::move(material);
        }

        auto &transform = entity.GetComponents<TransformComponent>();
        transform.position = Math::Vector3(0.0f, -0.1f, 0.0f);
        transform.scale = Math::Vector3(5.0f, 1.0f, 5.0f);
    }

    auto color_buffer = m_renderer.GetActiveSceneRGH().GetResource<PPGETexture>("PresentBuffer");
    {
        TextureViewDesc desc;
        desc.texture_view_type = ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
        desc.resource_dimension = color_buffer->GetDesc().resource_dimension;
        desc.format = color_buffer->GetDesc().format_type;
        desc.most_detailed_mip = 0;
        desc.mip_levels_num = 1;
        desc.array_slices_num = color_buffer->GetDesc().array_size;
        desc.first_array_slice = 0;
        m_viewport_texture = color_buffer->CreateView(desc);
        m_viewport_texture_handle = m_viewport_texture->GetGenericHandle();
    }
}

void EditorWidget::OnDetach()
{
}

void EditorWidget::OnUpdate(float delta_time)
{
    m_camera_controller.Update(delta_time);
}

void EditorWidget::OnRender()
{
    m_renderer.BeginScene(m_scene, m_camera_controller).Submit();
}

void EditorWidget::OnImGui()
{
    ImGui::DockSpaceOverViewport();

    ShowMainMenuBar();

    ShowSceneHierarchyPanel();

    ShowPropertiesPanel();

    ShowContentBrowser();

    ShowViewport();
}

void EditorWidget::ShowMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Project", "Ctrl+N"))
            {
                APP_TRACE("New scene is created");
            }
            if (ImGui::MenuItem("Open Project", "Ctrl+O"))
            {
                APP_TRACE("Open existing scene");
            }
            if (ImGui::MenuItem("Save Project", "Ctrl+S"))
            {
                APP_TRACE("Scene is saved");
            }
            if (ImGui::MenuItem("Save Project As", "Ctrl+Shift+S"))
            {
                APP_TRACE("Scene is saved as");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void EditorWidget::ShowSceneHierarchyPanel()
{
    ImGui::Begin("Scene Hierarchy");

    m_scene.ForEachEntity([&](Entity entity) {
        std::string_view name;
        if (entity.Has<TagComponent>())
        {
            auto &tag_component = entity.GetComponents<TagComponent>();
            name = tag_component.tag;
        }
        else
        {
            name = "Entity";
        }

        ImGuiTreeNodeFlags tree_node_flags =
            ((m_selected_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        tree_node_flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        if (ImGui::TreeNodeEx((void *)(uint64_t)entity, tree_node_flags, name.data()))
        {
            // Iterate through child entities;
            ImGui::TreePop();
        }

        if (ImGui::IsItemClicked())
        {
            m_selected_entity = entity;
        }

        bool is_entity_deleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                is_entity_deleted = true;

            ImGui::EndPopup();
        }

        if (is_entity_deleted)
        {
            m_scene.DestroyEntity(entity);
            if (m_selected_entity == entity)
                m_selected_entity = {};
        }
    });

    if (ImGui::BeginPopupContextWindow(0, 1, false))
    {
        if (ImGui::MenuItem("Create Entity"))
            m_scene.CreateEntity("Entity " + std::to_string(++entity_count));

        ImGui::EndPopup();
    }

    ImGui::End();
}

void EditorWidget::ShowPropertiesPanel()
{
    ImGui::Begin("Properties");
    if (m_selected_entity)
    {
        if (m_selected_entity.Has<TagComponent>())
        {
            auto &tag_component = m_selected_entity.GetComponents<TagComponent>();

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            std::strncpy(buffer, tag_component.tag.c_str(), sizeof(buffer));
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
            {
                tag_component.tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent"))
        {
            if (ImGui::MenuItem("Light"))
            {
                if (!m_selected_entity.Has<LightComponent>())
                    m_selected_entity.AddComponent<LightComponent>(LightComponent::LightType::DIRECTIONAL);
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Mesh Filter"))
            {
                if (!m_selected_entity.Has<MeshFilterComponent>())
                    m_selected_entity.AddComponent<MeshFilterComponent>();
                ImGui::CloseCurrentPopup();
            }

            if (ImGui::MenuItem("Mesh Renderer"))
            {
                if (!m_selected_entity.Has<MeshRendererComponent>())
                    m_selected_entity.AddComponent<MeshRendererComponent>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", m_selected_entity, [](auto &component) {
            DrawVectorController("Position", component.position);
            // roll (x-axis rotation)
            float roll = 0.0f;
            {
                double sinr_cosp =
                    2 * (component.rotation.w * component.rotation.x + component.rotation.y * component.rotation.z);
                double cosr_cosp =
                    1 - 2 * (component.rotation.x * component.rotation.x + component.rotation.y * component.rotation.y);
                roll = std::atan2(sinr_cosp, cosr_cosp) / PI * 180.0f;
            }
            // pitch (y-axis rotation)
            float pitch = 0.0f;
            {
                double sinp =
                    2 * (component.rotation.w * component.rotation.y - component.rotation.z * component.rotation.x);
                if (std::abs(sinp) >= 1)
                    pitch = std::copysign(PI / 2, sinp) / PI * 180.0f; // use 90 degrees if out of range
                else
                    pitch = std::asin(sinp) / PI * 180.0f;
            }
            // yaw (z-axis rotation)
            float yaw = 0.0f;
            {
                double siny_cosp =
                    2 * (component.rotation.w * component.rotation.z + component.rotation.x * component.rotation.y);
                double cosy_cosp =
                    1 - 2 * (component.rotation.y * component.rotation.y + component.rotation.z * component.rotation.z);
                yaw = std::atan2(siny_cosp, cosy_cosp) / PI * 180.0f;
            }

            Math::Vector3 euler_angles(yaw, pitch, roll);
            DrawVectorController("Rotation", euler_angles);

            double cy = cos(euler_angles.x / 180.0f * PI * 0.5);
            double sy = sin(euler_angles.x / 180.0f * PI * 0.5);
            double cp = cos(euler_angles.y / 180.0f * PI * 0.5);
            double sp = sin(euler_angles.y / 180.0f * PI * 0.5);
            double cr = cos(euler_angles.z / 180.0f * PI * 0.5);
            double sr = sin(euler_angles.z / 180.0f * PI * 0.5);

            component.rotation.w = cr * cp * cy + sr * sp * sy;
            component.rotation.x = sr * cp * cy - cr * sp * sy;
            component.rotation.y = cr * sp * cy + sr * cp * sy;
            component.rotation.z = cr * cp * sy - sr * sp * cy;

            DrawVectorController("Scale", component.scale, 1.0f);
        });

        DrawComponent<LightComponent>("Light", m_selected_entity, [](LightComponent &component) {
            const char *light_types[] = {"Directional", "Point", "Spot"};
            const char *current_light_type = light_types[(uint8_t)component.GetLightType()];
            if (ImGui::BeginCombo("Light Type", current_light_type))
            {
                for (int i = 0; i < 3; i++)
                {
                    bool is_selected = current_light_type == light_types[i];
                    if (ImGui::Selectable(light_types[i], is_selected))
                    {
                        current_light_type = light_types[i];
                        component.SetLightType((LightComponent::LightType)i);
                    }

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }

                ImGui::EndCombo();
            }

            float light_color[4] = {component.color.R(), component.color.G(), component.color.B(), component.color.A()};
            ImGui::ColorEdit4("Light Color", light_color);
            component.color = Math::Color(light_color[0], light_color[1], light_color[2], light_color[3]);

            float light_intensity = component.intensity;
            if (ImGui::DragFloat("Intensity", &light_intensity))
                component.intensity = light_intensity;

            if (component.GetLightType() != LightComponent::LightType::DIRECTIONAL)
            {
                float dist_attenuation_a0 = component.dist_attenuation_a0;
                if (ImGui::DragFloat("Att (A0)", &dist_attenuation_a0))
                    component.dist_attenuation_a0 = dist_attenuation_a0;

                float dist_attenuation_a1 = component.dist_attenuation_a1;
                if (ImGui::DragFloat("Att (A1)", &dist_attenuation_a1))
                    component.dist_attenuation_a1 = dist_attenuation_a1;

                float dist_attenuation_a2 = component.dist_attenuation_a2;
                if (ImGui::DragFloat("Att (A2)", &dist_attenuation_a2))
                    component.dist_attenuation_a2 = dist_attenuation_a2;

                float light_range = component.range;
                if (ImGui::DragFloat("Range", &light_range))
                    component.range = light_range;
            }

            if (component.GetLightType() == LightComponent::LightType::SPOT)
            {
                float spot_cutoff_angle = component.spot_cutoff_angle;
                if (ImGui::DragFloat("Cutoff Angle", &spot_cutoff_angle))
                    component.spot_cutoff_angle = spot_cutoff_angle;

                float spot_inner_cone_angle = component.spot_inner_cone_angle;
                if (ImGui::DragFloat("Inner Cone Angle", &spot_inner_cone_angle))
                    component.spot_inner_cone_angle = spot_inner_cone_angle;

                float spot_decay_rate = component.spot_decay_rate;
                if (ImGui::DragFloat("Spot Decay Angle", &spot_decay_rate))
                    component.spot_decay_rate = spot_decay_rate;
            }
        });
    }

    ImGui::End();
}

void EditorWidget::ShowContentBrowser()
{
    ImGui::Begin("Content Browser");

    ImGui::End();
}

void EditorWidget::ShowViewport()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{1280, 720});
    ImGui::Begin("Viewport");
    {
        ImTextureID tex_handle = reinterpret_cast<ImTextureID>(m_viewport_texture_handle);
        ImVec2 tex_size = ImVec2(m_viewport_texture->GetTexture()->GetDesc().width,
                                 m_viewport_texture->GetTexture()->GetDesc().height);
        ImGui::Image(tex_handle, tex_size);
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void EditorWidget::DrawVectorController(const std::string &label, Math::Vector3 &vec3, float reset_value,
                                        float column_width)

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
} // namespace PPGE
