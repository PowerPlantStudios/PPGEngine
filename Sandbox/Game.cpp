#include <PPGE.h>

constexpr float PI = 3.14159265359f;

static size_t entity_count = 0;

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
    static void LoadScene(const std::filesystem::path &path_to_scene, PPGE::Scene &ppge_scene,
                          float scale_factor = 1.0f)
    {
        std::unordered_map<std::string, std::shared_ptr<PPGE::PPGETextureView>> diffuse_maps;
        std::vector<PPGE::StandardVertex> vertices;
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

            auto entity = ppge_scene.CreateEntity("Entity " + std::to_string(++entity_count));
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

    std::shared_ptr<PPGE::PPGETextureView> m_viewport_texture;

    size_t m_viewport_texture_handle = 0;

    PPGE::Entity m_selected_entity;

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
        // Create camera
        auto camera_entity = m_scene.CreateEntity("Camera");
        auto &camera_component = camera_entity.AddComponent<PPGE::CameraComponent>();
        {
            float fov = PI * 0.333f;
            float aspect_ratio =
                (float)PPGE::DisplaySystem::Get().GetWidth() / (float)PPGE::DisplaySystem::Get().GetHeight();
            camera_component.projection =
                PPGE::Math::Matrix::CreatePerspectiveFieldOfView(fov, aspect_ratio, 0.1f, 1000.0f);
        }
        m_camera_controller.Possess(camera_entity);
        m_camera_controller.Move(PPGE::Math::Vector3(-1.5f, 1.75f, -1.5f));
        m_camera_controller.LookAt(PPGE::Math::Vector3::Zero, PPGE::Math::Vector3::Up);

        // Create lights
        {
            auto light = m_scene.CreateEntity("Directional Light");
            auto &transform = light.GetComponents<PPGE::TransformComponent>();
            transform.position = PPGE::Math::Vector3(1.0f, 1.0f, 1.0f);
            transform.rotation = PPGE::Math::Quaternion::CreateFromYawPitchRoll(2.3561945, -0.7853982f, 0.0f);
            auto &dir_light = light.AddComponent<PPGE::LightComponent>(PPGE::LightComponent::LightType::DIRECTIONAL);
            dir_light.color = PPGE::Math::Color(0.8f, 0.8f, 0.7f);
            dir_light.intensity = 0.05f;
        }

        // Load resources
        resource_mgr.WalkRoot("../../Sandbox/assets");
        if (auto model = resource_mgr.GetCachedResource("cube/cube.obj"))
        {
            auto lazy_model = std::static_pointer_cast<PPGE::LazyResource>(model);
            SceneLoader::LoadScene(lazy_model->data, m_scene);
        }

        {
            auto entity = m_scene.CreateEntity("Entity " + std::to_string(++entity_count));
            {
                std::vector<PPGE::StandardVertex> vertices{{.px = 1.0f,
                                                            .py = 0.0f,
                                                            .pz = 1.0f,
                                                            .nx = 0.0f,
                                                            .ny = 1.0f,
                                                            .nz = 0.0f,
                                                            .color = 0x1199eeff,
                                                            .u1 = 0.0f,
                                                            .v1 = 0.0f},
                                                           {.px = 1.0f,
                                                            .py = 0.0f,
                                                            .pz = -1.0f,
                                                            .nx = 0.0f,
                                                            .ny = 1.0f,
                                                            .nz = 0.0f,
                                                            .color = 0x11ee99ff,
                                                            .u1 = 1.0f,
                                                            .v1 = 0.0f},
                                                           {.px = -1.0f,
                                                            .py = 0.0f,
                                                            .pz = -1.0f,
                                                            .nx = 0.0f,
                                                            .ny = 1.0f,
                                                            .nz = 0.0f,
                                                            .color = 0x1199eeff,
                                                            .u1 = 1.0f,
                                                            .v1 = 1.0f},
                                                           {.px = -1.0f,
                                                            .py = 0.0f,
                                                            .pz = 1.0f,
                                                            .nx = 0.0f,
                                                            .ny = 1.0f,
                                                            .nz = 0.0f,
                                                            .color = 0xee1199ff,
                                                            .u1 = 0.0f,
                                                            .v1 = 1.0f}};
                std::vector<unsigned int> indices{0, 3, 2, 2, 1, 0};
                CreateMeshFilter(entity, vertices, indices);
            }
            if (auto resource = resource_mgr.GetCachedResource("textures/no_texture.png"))
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

        auto color_buffer = m_renderer.GetActiveSRGH().GetResource<PPGE::PPGETexture>("ColorBuffer");
        {
            PPGE::TextureViewDesc desc;
            desc.texture_view_type = PPGE::ResourceViewType::RESOURCE_VIEW_SHADER_RESOURCE;
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
        ImGui::DockSpaceOverViewport();

        ShowMainMenuBar();

        ShowSceneHierarchyPanel();

        ShowPropertiesPanel();

        ShowContentBrowser();

        ShowViewport();
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

  private:
    void ShowMainMenuBar()
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

    void ShowSceneHierarchyPanel()
    {
        ImGui::Begin("Scene Hierarchy");

        m_scene.ForEachEntity([&](PPGE::Entity entity) {
            std::string_view name;
            if (entity.Has<PPGE::TagComponent>())
            {
                auto &tag_component = entity.GetComponents<PPGE::TagComponent>();
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

    void ShowPropertiesPanel()
    {
        ImGui::Begin("Properties");
        if (m_selected_entity)
        {
            if (m_selected_entity.Has<PPGE::TagComponent>())
            {
                auto &tag_component = m_selected_entity.GetComponents<PPGE::TagComponent>();

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
                    if (!m_selected_entity.Has<PPGE::LightComponent>())
                        m_selected_entity.AddComponent<PPGE::LightComponent>(
                            PPGE::LightComponent::LightType::DIRECTIONAL);
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Mesh Filter"))
                {
                    if (!m_selected_entity.Has<PPGE::MeshFilterComponent>())
                        m_selected_entity.AddComponent<PPGE::MeshFilterComponent>();
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("Mesh Renderer"))
                {
                    if (!m_selected_entity.Has<PPGE::MeshRendererComponent>())
                        m_selected_entity.AddComponent<PPGE::MeshRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::PopItemWidth();

            DrawComponent<PPGE::TransformComponent>("Transform", m_selected_entity, [](auto &component) {
                DrawVectorController("Position", component.position);
                // roll (x-axis rotation)
                float roll = 0.0f;
                {
                    double sinr_cosp =
                        2 * (component.rotation.w * component.rotation.x + component.rotation.y * component.rotation.z);
                    double cosr_cosp = 1 - 2 * (component.rotation.x * component.rotation.x +
                                                component.rotation.y * component.rotation.y);
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
                    double cosy_cosp = 1 - 2 * (component.rotation.y * component.rotation.y +
                                                component.rotation.z * component.rotation.z);
                    yaw = std::atan2(siny_cosp, cosy_cosp) / PI * 180.0f;
                }

                PPGE::Math::Vector3 euler_angles(yaw, pitch, roll);
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

            DrawComponent<PPGE::LightComponent>("Light", m_selected_entity, [](auto &component) {
                const char *light_types[] = {"Directional", "Point", "Spot"};
                const char *current_light_type = light_types[(uint8_t)component.type];
                if (ImGui::BeginCombo("Light Type", current_light_type))
                {
                    for (int i = 0; i < 3; i++)
                    {
                        bool is_selected = current_light_type == light_types[i];
                        if (ImGui::Selectable(light_types[i], is_selected))
                        {
                            current_light_type = light_types[i];
                            component.type = (PPGE::LightComponent::LightType)i;
                        }

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                float light_color[4] = {component.color.R(), component.color.G(), component.color.B(),
                                        component.color.A()};
                ImGui::ColorEdit4("Light Color", light_color);
                component.color = PPGE::Math::Color(light_color[0], light_color[1], light_color[2], light_color[3]);

                float light_intensity = component.intensity;
                if (ImGui::DragFloat("Intensity", &light_intensity))
                    component.intensity = light_intensity;

                if (component.type != PPGE::LightComponent::LightType::DIRECTIONAL)
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

                if (component.type == PPGE::LightComponent::LightType::SPOT)
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

    void ShowContentBrowser()
    {
        ImGui::Begin("Content Browser");

        ImGui::End();
    }

    void ShowViewport()
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

    template <typename ComponentType, typename DrawCallback>
    static void DrawComponent(const std::string &name, PPGE::Entity entity, DrawCallback &&callback)
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

    static void DrawVectorController(const std::string &label, PPGE::Math::Vector3 &vec3, float reset_value = 0.0f,
                                     float column_width = 100.0f)
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
