#pragma once
#include <PPGE.h>

namespace PPGE
{
class EditorWidget : public Widget
{
  public:
    EditorWidget();

    void OnAttach() override;

    void OnDetach() override;

    void OnUpdate(float delta_time) override;

    void OnRender() override;

    virtual void OnImGui();

    void OnInputEvent(InputEvent &inputEvent) override
    {
        DispatchInputEvent<KeyPressedEvent>(inputEvent,
                                            PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(EditorWidget::HandleKeyPressedEvenet));
    }

    bool HandleKeyPressedEvenet(KeyPressedEvent &e)
    {
        if (e.Keycode() == PPGE_KEY_J)
        {
            Destroy();
            return true;
        }
        return false;
    }

  private:
    void ShowMainMenuBar();

    void ShowSceneHierarchyPanel();

    void ShowPropertiesPanel();

    void ShowContentBrowser();

    void ShowViewport();

    template <typename ComponentType, typename DrawCallback>
    static void DrawComponent(const std::string &name, Entity entity, DrawCallback &&callback)
    {
        const ImGuiTreeNodeFlags tree_node_flags =
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        if (entity.Has<ComponentType>())
        {
            ImVec2 available_content_region = ImGui::GetContentRegionAvail();

            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
            ImGui::Separator();
            bool is_open = ImGui::TreeNodeEx((void *)typeid(ComponentType).hash_code(), tree_node_flags, name.c_str());
            ImGui::PopStyleVar();

            float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::SameLine(available_content_region.x - line_height * 0.5f);
            if (ImGui::Button("+", ImVec2{line_height, line_height}))
            {
                ImGui::OpenPopup("ComponentFunctions");
            }

            bool is_component_removed = false;
            if (ImGui::BeginPopup("ComponentFunctions"))
            {
                if (ImGui::MenuItem("Remove component"))
                    is_component_removed = true;
                ImGui::EndPopup();
            }

            if (is_open)
            {
                auto &component = entity.GetComponents<ComponentType>();
                callback(component);
                ImGui::TreePop();
            }

            if (is_component_removed)
                entity.RemoveComponents<ComponentType>();
        }
    }

    static void DrawVectorController(const std::string &label, Math::Vector3 &vec3, float reset_value = 0.0f,
                                     float column_width = 100.0f);

    ResourceManager resource_mgr;

    FreeLookCamera m_camera_controller;

    Scene m_scene;

    Renderer m_renderer;

    std::shared_ptr<PPGETextureView> m_viewport_texture;

    size_t m_viewport_texture_handle = 0;

    Entity m_selected_entity;
};
} // namespace PPGE
