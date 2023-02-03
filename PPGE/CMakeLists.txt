cmake_minimum_required(VERSION 3.20)
project(Engine VERSION 0.0.1 LANGUAGES CXX)

include_directories(
    ${DEPENDENCIES_SRC_DIR}/glfw/include
    ${DEPENDENCIES_SRC_DIR}/glad/include
    ${DEPENDENCIES_SRC_DIR}/spdlog/include
    ${DEPENDENCIES_SRC_DIR}/imgui/src
    ${DEPENDENCIES_SRC_DIR}/math/simplemath/include
    ${DEPENDENCIES_SRC_DIR}/assimp/include
    ${DEPENDENCIES_BIN_DIR}/assimp/include
    ${CMAKE_CURRENT_SOURCE_DIR})
if(WIN32)
    include_directories(
        ${DEPENDENCIES_SRC_DIR}/directxtk/src/Inc)
endif()

file(GLOB PPGE_HDRS 
    ${PPGE_CORE_DIR}/core/*.h
    ${PPGE_CORE_DIR}/ecs/*.h
    ${PPGE_CORE_DIR}/ecs/components/*.h
    ${PPGE_CORE_DIR}/event/*.h
    ${PPGE_CORE_DIR}/io/*.h
    ${PPGE_CORE_DIR}/renderer/*.h
    ${PPGE_CORE_DIR}/renderer/passes/*.h
    ${PPGE_CORE_DIR}/rhi/*.h
    ${PPGE_CORE_DIR}/system/*.h
    ${PPGE_CORE_DIR}/system/animation/*.h
    ${PPGE_CORE_DIR}/system/animation/*.hpp
    ${PPGE_CORE_DIR}/ui/*.h)
set(PPGE_HDRS ${PPGE_HDRS} 
    ${PPGE_CORE_DIR}/PPGE.h)

file(GLOB PPGE_SRCS 
    ${PPGE_CORE_DIR}/core/*.cpp
    ${PPGE_CORE_DIR}/ecs/*.cpp
    ${PPGE_CORE_DIR}/ecs/components/*.cpp
    ${PPGE_CORE_DIR}/event/*.cpp
    ${PPGE_CORE_DIR}/io/*.cpp
    ${PPGE_CORE_DIR}/renderer/*.cpp
    ${PPGE_CORE_DIR}/renderer/passes/*.cpp
    ${PPGE_CORE_DIR}/rhi/*.cpp
    ${PPGE_CORE_DIR}/system/*.cpp
    ${PPGE_CORE_DIR}/system/animation/*.cpp
    ${PPGE_CORE_DIR}/ui/*.cpp)
set(PPGE_SRCS ${PPGE_SRCS} 
    ${PPGE_CORE_DIR}/PPGEpch.h)

# Add platform glfw related files for every PC platform
file(GLOB PLATFORM_GLFW_HDRS
${PPGE_CORE_DIR}/platform/glfw/*.h)
file(GLOB PLATFOMR_GLFW_SRCS
${PPGE_CORE_DIR}/platform/glfw/*.cpp)
set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_GLFW_HDRS})
set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_GLFW_SRCS})
# Add platform opengl related files for every PC platform
file(GLOB PLATFORM_GL_HDRS
    ${PPGE_CORE_DIR}/platform/gl/*.h)
file(GLOB PLATFOMR_GL_SRCS
    ${PPGE_CORE_DIR}/platform/gl/*.cpp)
set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_GL_HDRS})
set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_GL_SRCS})
# Add platform vulkan related files for every PC platform
file(GLOB PLATFORM_VK_HDRS
    ${PPGE_CORE_DIR}/platform/vk/*.h)
file(GLOB PLATFOMR_VK_SRCS
    ${PPGE_CORE_DIR}/platform/vk/*.cpp)
set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_VK_HDRS})
set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_VK_SRCS})

if (WIN32)
    # Add platform windows related files to the project
    file(GLOB PLATFORM_WIN_HDRS
        ${PPGE_CORE_DIR}/platform/windows/*.h)
    file(GLOB PLATFOMR_WIN_SRCS
        ${PPGE_CORE_DIR}/platform/windows/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_WIN_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_WIN_SRCS})
    # Add platform win32 related files to the project
    file(GLOB PLATFORM_WIN32_HDRS
        ${PPGE_CORE_DIR}/platform/win32/*.h)
    file(GLOB PLATFOMR_WIN32_SRCS
        ${PPGE_CORE_DIR}/platform/win32/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_WIN32_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_WIN32_SRCS})
    # Add platform D3D11 related files to the project
    file(GLOB PLATFORM_D3D11_HDRS
        ${PPGE_CORE_DIR}/platform/d3d11/*.h)
    file(GLOB PLATFOMR_D3D11_SRCS
        ${PPGE_CORE_DIR}/platform/d3d11/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_D3D11_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_D3D11_SRCS})
    # Add platform dx12 related files to the project
    file(GLOB PLATFORM_DX12_HDRS
        ${PPGE_CORE_DIR}/platform/dx12/*.h)
    file(GLOB PLATFOMR_DX12_SRCS
        ${PPGE_CORE_DIR}/platform/dx12/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_DX12_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_DX12_SRCS})
elseif(APPLE)
    # Add platform mac related files to the project
    file(GLOB PLATFORM_OSX_HDRS
        ${PPGE_CORE_DIR}/platform/osx/*.h)
    file(GLOB PLATFOMR_OSX_SRCS
        ${PPGE_CORE_DIR}/platform/osx/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_OSX_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_OSX_SRCS})
    # Add platform metal related files to the project
    file(GLOB PLATFORM_MTL_HDRS
        ${PPGE_CORE_DIR}/platform/metal/*.h)
    file(GLOB PLATFOMR_MTL_SRCS
        ${PPGE_CORE_DIR}/platform/metal/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_MTL_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_MTL_SRCS})
elseif(UNIX)
    # Add platform linux related files to the project
    file(GLOB PLATFORM_LNX_HDRS
        ${PPGE_CORE_DIR}/platform/linuxbsd/*.h)
    file(GLOB PLATFOMR_LNX_SRCS
        ${PPGE_CORE_DIR}/platform/linuxbsd/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_LNX_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_LNX_SRCS})
else()
    message(FATAL_ERROR "Current platform is not supported.")
endif()

add_library(${PROJECT_NAME} STATIC 
    ${PPGE_SRCS} 
    ${PPGE_HDRS})

target_precompile_headers(${PROJECT_NAME} PRIVATE 
    ${PPGE_CORE_DIR}/PPGEpch.h)

target_compile_definitions(${PROJECT_NAME} PRIVATE 
    $<$<CONFIG:Debug>:PPGE_DEBUG> 
    $<$<CONFIG:Release>:PPGE_RELEASE>)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    imgui 
    simplemath
    assimp
    EnTT)

target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PPGE_PLATFORM_WIN NOMINMAX)
    target_link_libraries(${PROJECT_NAME} PRIVATE simpletexture d3dcompiler.lib dxgi.lib d3d11.lib d3d12.lib)
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PPGE_PLATFORM_APPLE)
elseif(UNIX)
    target_compile_definitions(${PROJECT_NAME} PUBLIC PPGE_PLATFORM_UNIX)
else()
    message(FATAL_ERROR "Current platform is not supported.")
endif()

file(GLOB PPGE_RSCS
    ${PPGE_CORE_DIR}/renderer/shaders/*.hlsl)
if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set_source_files_properties(${PPGE_RSCS} PROPERTIES VS_TOOL_OVERRIDE "None")
endif()
target_sources(${PROJECT_NAME} PRIVATE ${PPGE_RSCS})

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PPGE_HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PPGE_SRCS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PPGE_RSCS})
