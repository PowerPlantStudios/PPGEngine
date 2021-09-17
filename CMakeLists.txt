cmake_minimum_required(VERSION 3.20)
project(PPGE)

set(CMAKE_CXX_STANDARD 23)

set(GRAPHICS_API "OpenGL" CACHE STRING "Selected Rendering API")
set(GRAPHICS_APIS "OpenGL;DX11;DX12;Vulkan;Metal")
set_property(CACHE GRAPHICS_API PROPERTY STRINGS ${GRAPHICS_APIS})

if(WIN32)
    # Check selected API is supported for the platform
    if(${GRAPHICS_API} STREQUAL "Metal")
    message(FATAL_ERROR "Windows build only supports OpenGL, Vulkan, DX11 and DX12.")
    endif()
    
    if(${GRAPHICS_API} STREQUAL "OpenGL")
        find_package(OpenGL REQUIRED)
    endif()
    
    # Set the correct Window API on the current platform for the selected rendering API
    if(${GRAPHICS_API} STREQUAL "OpenGL" OR ${GRAPHICS_API} STREQUAL "Vulkan")
        set(DISPLAY_API "GLFW" CACHE STRING "Selected Window API")
    elseif(${GRAPHICS_API} STREQUAL "DX11" OR ${GRAPHICS_API} STREQUAL "DX12")
        set(DISPLAY_API "Win32" CACHE STRING "Selected Window API")
    endif()
elseif(APPLE)
    if(${GRAPHICS_API} STREQUAL "DX11" OR ${GRAPHICS_API} STREQUAL "DX12")
        message(FATAL_ERROR "MacOS build only supports OpenGL, Vulkan and Metal.")
    endif()
    # TODO : Check if Metal API supports GLFW as the Window API
    set(DISPLAY_API "GLFW" CACHE STRING "Selected Window API")
elseif(UNIX)
    if(${GRAPHICS_API} STREQUAL "DX11" OR ${GRAPHICS_API} STREQUAL "DX12" OR ${GRAPHICS_API} STREQUAL "Metal")
        message(FATAL_ERROR "UNIX build only supports OpenGL and Vulkan.")
    endif()
    set(DISPLAY_API "GLFW" CACHE STRING "Selected Window API")
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/Thirdparty)
set(PPGE_CORE_DIR ${PROJECT_SOURCE_DIR}/PPGE)
set(PPGE_EDITOR_DIR ${PROJECT_SOURCE_DIR}/Editor)
set(GAME_DIR ${PROJECT_SOURCE_DIR}/Sandbox)

add_subdirectory(PPGE)
add_subdirectory(Sandbox)
add_subdirectory(Thirdparty)
