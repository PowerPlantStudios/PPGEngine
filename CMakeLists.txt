cmake_minimum_required(VERSION 3.20)
project(PPGE)

set(CMAKE_CXX_STANDARD 23)

set(RENDERING_API "OpenGL" CACHE STRING "Selected Rendering API")
set(RENDERING_APIS "OpenGL;DX11;DX12;Vulkan;Metal")
set_property(CACHE RENDERING_API PROPERTY STRINGS ${RENDERING_APIS})

# Make sure current platform supports selected Rendering API as well as
# set the correct window/display API for the selected rendering API
if(WIN32)
    # Check selected API is supported for the platform
    if(${RENDERING_API} STREQUAL "Metal")
        message(FATAL_ERROR "Windows build only supports OpenGL, Vulkan, DX11 and DX12.")
    endif()    
    # Set the correct Window API on the current platform for the selected rendering API
    if(${RENDERING_API} STREQUAL "OpenGL" OR ${RENDERING_API} STREQUAL "Vulkan")
        set(DISPLAY_API "GLFW")
    elseif(${RENDERING_API} STREQUAL "DX11" OR ${RENDERING_API} STREQUAL "DX12")
        set(DISPLAY_API "Win32")
    endif()
elseif(APPLE)
    # TODO : Check if we can maintain anything else but Metal on Mac OS build.
    if(${RENDERING_API} STREQUAL "DX11" OR ${RENDERING_API} STREQUAL "DX12")
        message(FATAL_ERROR "MacOS build supports only OpenGL, Vulkan and Metal.")
    endif()
    # TODO : Check if Metal API supports GLFW as the Window API
    set(DISPLAY_API "GLFW")
elseif(UNIX)
    if(${RENDERING_API} STREQUAL "DX11" OR ${RENDERING_API} STREQUAL "DX12" OR ${RENDERING_API} STREQUAL "Metal")
        message(FATAL_ERROR "UNIX build supports only OpenGL and Vulkan.")
    endif()
    set(DISPLAY_API "GLFW")
else()
    message(FATAL_ERROR "Unsupported platform.")
endif()

message("-- Selcting rendering API: ${RENDERING_API}")
message("-- Selcting display API: ${DISPLAY_API}")

# Find the required package for the selected rendering API
if(${RENDERING_API} STREQUAL "OpenGL")
    find_package(OpenGL REQUIRED)
elseif (${RENDERING_API} STREQUAL "Metal")
    message(WARNING "FIXME : Implement find_package(Metal Required) for Metal build. Remove this warning, if it is irrelevant.")
elseif (${RENDERING_API} STREQUAL "Vulkan")
    message(WARNING "FIXME : Implement find_package(Vulkan Required) for Vulkan build. Remove this warning, if it is irrelevant.")
endif()


set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/Thirdparty)
set(PPGE_CORE_DIR ${PROJECT_SOURCE_DIR}/PPGE)
set(PPGE_EDITOR_DIR ${PROJECT_SOURCE_DIR}/Editor)
set(GAME_DIR ${PROJECT_SOURCE_DIR}/Sandbox)

add_subdirectory(PPGE)
add_subdirectory(Sandbox)
add_subdirectory(Thirdparty)
