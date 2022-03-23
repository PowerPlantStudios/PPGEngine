#pragma once

// Client Side Header File For PPGE API

// External includes

// PPGE includes
#include "core/application.h"
#include "core/defines.h"
#include "core/input.h"
#include "ecs/components/camera_component.h"
#include "ecs/components/light_component.h"
#include "ecs/components/other_components.h"
#include "ecs/entity.h"
#include "ecs/scene.h"
#include "event/application_event.h"
#include "event/input_event.h"
#include "io/resource_manager.h"
#include "renderer/buffer.h"
#include "renderer/pipeline_state.h"
#include "renderer/renderer_types.h"
#include "renderer/sampler.h"
#include "renderer/shader.h"
#include "renderer/shader_resource_binding.h"
#include "renderer/shader_resource_variable.h"
#include "renderer/texture.h"
#include "renderer/texture_view.h"
#include "system/logger_system.h"
#include "system/renderer_system.h"

// Entry point for client side application
#include "core/main.h"