cmake_minimum_required(VERSION 3.20)
project(Engine VERSION 0.0.1 LANGUAGES CXX)

include_directories(
    ${DEPENDENCIES_DIR}/glfw/include
    ${DEPENDENCIES_DIR}/glad/include
    ${DEPENDENCIES_DIR}/spdlog/include
    ${DEPENDENCIES_DIR}/imgui
    ${DEPENDENCIES_DIR}/math/simplemath/include
    ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB PPGE_HDRS 
    ${PPGE_CORE_DIR}/core/*.h
    ${PPGE_CORE_DIR}/event/*.h
    ${PPGE_CORE_DIR}/system/*.h
    ${PPGE_CORE_DIR}/ui/*.h)
set(PPGE_HDRS ${PPGE_HDRS} 
    ${PPGE_CORE_DIR}/PPGE.h
    ${PPGE_CORE_DIR}/PPGEpch.h)

file(GLOB PPGE_SRCS 
    ${PPGE_CORE_DIR}/core/*.cpp
    ${PPGE_CORE_DIR}/event/*.cpp
    ${PPGE_CORE_DIR}/system/*.cpp
    ${PPGE_CORE_DIR}/ui/*.cpp)
set(PPGE_SRCS ${PPGE_SRCS} 
    ${PPGE_CORE_DIR}/PPGEpch.cpp)

if (WIN32)
    file(GLOB PLATFORM_WIN_HDRS
        ${PPGE_CORE_DIR}/platform/windows/*.h)
    file(GLOB PLATFOMR_WIN_SRCS
        ${PPGE_CORE_DIR}/platform/windows/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_WIN_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_WIN_SRCS})
elseif(APPLE)
    file(GLOB PLATFORM_OSX_HDRS
        ${PPGE_CORE_DIR}/platform/osx/*.h)
    file(GLOB PLATFOMR_OSX_SRCS
        ${PPGE_CORE_DIR}/platform/osx/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_OSX_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_OSX_SRCS})
elseif(UNIX)
    file(GLOB PLATFORM_LNX_HDRS
        ${PPGE_CORE_DIR}/platform/linuxbsd/*.h)
    file(GLOB PLATFOMR_LNX_SRCS
        ${PPGE_CORE_DIR}/platform/linuxbsd/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_LNX_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_LNX_SRCS})
else()
    message(FATAL_ERROR "Current platform is not supported.")
endif()

if(${DISPLAY_API} STREQUAL "GLFW")
    file(GLOB PLATFORM_GLFW_HDRS
        ${PPGE_CORE_DIR}/platform/glfw/*.h)
    file(GLOB PLATFOMR_GLFW_SRCS
        ${PPGE_CORE_DIR}/platform/glfw/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_GLFW_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_GLFW_SRCS})
elseif(${DISPLAY_API} STREQUAL "Win32")
    file(GLOB PLATFORM_WIN32_HDRS
        ${PPGE_CORE_DIR}/platform/win32/*.h)
    file(GLOB PLATFOMR_WIN32_SRCS
        ${PPGE_CORE_DIR}/platform/win32/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_WIN32_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_WIN32_SRCS})
else()
    message(FATAL_ERROR "Display API '${DISPLAY_API}' is not supported.")
endif()

if(${RENDERING_API} STREQUAL "OpenGL")
    file(GLOB PLATFORM_GL_HDRS
        ${PPGE_CORE_DIR}/platform/gl/*.h)
    file(GLOB PLATFOMR_GL_SRCS
        ${PPGE_CORE_DIR}/platform/gl/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_GL_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_GL_SRCS})
elseif(${RENDERING_API} STREQUAL "Vulkan")
    file(GLOB PLATFORM_VK_HDRS
        ${PPGE_CORE_DIR}/platform/vk/*.h)
    file(GLOB PLATFOMR_VK_SRCS
        ${PPGE_CORE_DIR}/platform/vk/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_VK_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_VK_SRCS})
elseif(${RENDERING_API} STREQUAL "DX11")
    file(GLOB PLATFORM_DX11_HDRS
        ${PPGE_CORE_DIR}/platform/dx11/*.h)
    file(GLOB PLATFOMR_DX11_SRCS
        ${PPGE_CORE_DIR}/platform/dx11/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_DX11_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_DX11_SRCS})
elseif(${RENDERING_API} STREQUAL "DX12")
    file(GLOB PLATFORM_DX12_HDRS
        ${PPGE_CORE_DIR}/platform/dx12/*.h)
    file(GLOB PLATFOMR_DX12_SRCS
        ${PPGE_CORE_DIR}/platform/dx12/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_DX12_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_DX12_SRCS})
elseif(${RENDERING_API} STREQUAL "Metal")
    file(GLOB PLATFORM_MTL_HDRS
        ${PPGE_CORE_DIR}/platform/metal/*.h)
    file(GLOB PLATFOMR_MTL_SRCS
        ${PPGE_CORE_DIR}/platform/metal/*.cpp)
    set(PPGE_HDRS ${PPGE_HDRS} ${PLATFORM_MTL_HDRS})
    set(PPGE_SRCS ${PPGE_SRCS} ${PLATFOMR_MTL_SRCS})
else()
    message(FATAL_ERROR "Rendering API '${RENDERING_API}' is not supported.")
endif()

add_library(Engine STATIC 
    ${PPGE_SRCS} 
    ${PPGE_HDRS})

target_compile_definitions(Engine PRIVATE 
    $<$<CONFIG:Debug>:PPGE_DEBUG> 
    $<$<CONFIG:Release>:PPGE_RELEASE>)

target_include_directories(Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Engine PRIVATE
    imgui 
    simplemath)

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
    target_link_libraries(Engine PRIVATE glfw)
elseif(${DISPLAY_API} STREQUAL "Win32")
    target_compile_definitions(Engine PUBLIC DISPLAY_API_WIN32)
else()
    message(FATAL_ERROR "Display API '${DISPLAY_API}' is not supported.")
endif()

if(${RENDERING_API} STREQUAL "OpenGL")
    target_compile_definitions(Engine PUBLIC RENDERING_API_OPENGL)
    target_link_libraries(Engine PRIVATE glad)
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
