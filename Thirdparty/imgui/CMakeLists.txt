cmake_minimum_required(VERSION 3.20)
project(IMGUI LANGUAGES CXX)

include_directories(${DEPENDENCIES_DIR}/glad/include)
include_directories(${DEPENDENCIES_DIR}/glfw/include)

set(IMGUI_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_demo.cpp)

set(IMGUI_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imconfig.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imstb_rectpack.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imstb_textedit.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/imstb_truetype.h)

set(IMGUI_SRCS ${IMGUI_SRCS}  
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_opengl3.cpp)
# set(IMGUI_SRCS ${IMGUI_SRCS}  
#     ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_vulkan.cpp)
set(IMGUI_SRCS ${IMGUI_SRCS}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_glfw.cpp)

if (WIN32)
    set(IMGUI_SRCS ${IMGUI_SRCS}  
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_dx11.cpp)
    set(IMGUI_SRCS ${IMGUI_SRCS}  
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_dx12.cpp)
    set(IMGUI_SRCS ${IMGUI_SRCS}  
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_win32.cpp)
elseif(APPLE)
    set(IMGUI_SRCS ${IMGUI_SRCS}  
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_metal.mm)
    set(IMGUI_SRCS ${IMGUI_SRCS}  
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_osx.mm)
endif()

add_library(imgui STATIC ${IMGUI_SRCS} ${IMGUI_HDRS})
target_include_directories(imgui PUBLIC ${IMGUI_HDRS})

target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
target_link_libraries(imgui PRIVATE glfw OpenGL::GL)
