#pragma once

// Client Side Header File For PPGE API

// External includes

// PPGE includes
#include "core/application.h"
#include "core/defines.h"
#include "core/input.h"
#include "ecs/components/camera_component.h"
#include "ecs/components/debug_component.h"
#include "ecs/components/light_component.h"
#include "ecs/components/mesh_component.h"
#include "ecs/components/transform_components.h"
#include "ecs/entity.h"
#include "ecs/scene.h"
#include "event/application_event.h"
#include "event/input_event.h"
#include "io/resource_manager.h"
#include "renderer/camera_controller.h"
#include "renderer/render_graph.h"
#include "renderer/renderer.h"
#include "renderer/renderer_types.h"
#include "rhi/buffer.h"
#include "rhi/pipeline_state.h"
#include "rhi/rhi_types.h"
#include "rhi/sampler.h"
#include "rhi/shader.h"
#include "rhi/shader_resource_binding.h"
#include "rhi/shader_resource_variable.h"
#include "rhi/texture.h"
#include "rhi/texture_view.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

// Entry point for client side application
#include "core/main.h"