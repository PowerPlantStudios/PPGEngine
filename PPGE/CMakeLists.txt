cmake_minimum_required(VERSION 3.20)
project(Engine VERSION 0.0.1 LANGUAGES CXX)

include_directories(
    ${DEPENDENCIES_DIR}/glfw/include
    ${DEPENDENCIES_DIR}/glad/include
    ${DEPENDENCIES_DIR}/spdlog/include
    ${DEPENDENCIES_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE PPGE_HDRS *.h)
file(GLOB_RECURSE PPGE_SRCS *.cpp)

add_library(Engine 
    STATIC ${PPGE_SRCS} ${PPGE_HDRS})
target_compile_definitions(Engine 
    PRIVATE $<$<CONFIG:Debug>:PPGE_DEBUG> 
            $<$<CONFIG:Release>:PPGE_RELEASE>)
target_include_directories(Engine 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Engine 
    PRIVATE glfw glad imgui)

if (WIN32)
    target_compile_definitions(Engine PUBLIC PPGE_PLATFORM_WIN)
elseif(APPLE)
    target_compile_definitions(Engine PUBLIC PPGE_PLATFORM_APPLE)
elseif(UNIX)
    target_compile_definitions(Engine PUBLIC PPGE_PLATFORM_UNIX)
else()
    message(FATAL_ERROR "Current platform is not supported.")
endif()
    
if(${DISPLAY_API} STREQUAL "GLFW")
    target_compile_definitions(Engine PUBLIC DISPLAY_API_GLFW PRIVATE GLFW_INCLUDE_NONE)
elseif(${DISPLAY_API} STREQUAL "Win32")
    target_compile_definitions(Engine PUBLIC DISPLAY_API_WIN32)
else()
    message(FATAL_ERROR "Display API '${DISPLAY_API}' is not supported.")
endif()

if(${RENDERING_API} STREQUAL "OpenGL")
    target_compile_definitions(Engine PUBLIC RENDERING_API_OPENGL) 
elseif(${RENDERING_API} STREQUAL "Vulkan")
    target_compile_definitions(Engine PUBLIC RENDERING_API_VULKAN)
elseif(${RENDERING_API} STREQUAL "DX11")
    target_compile_definitions(Engine PUBLIC RENDERING_API_DX11)
elseif(${RENDERING_API} STREQUAL "DX12")
    target_compile_definitions(Engine PUBLIC RENDERING_API_DX12)
elseif(${RENDERING_API} STREQUAL "Metal")
    target_compile_definitions(Engine PUBLIC RENDERING_API_METAL)
else()
    message(FATAL_ERROR "Rendering API '${RENDERING_API}' is not supported.")
endif()

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PPGE_HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PPGE_SRCS})
