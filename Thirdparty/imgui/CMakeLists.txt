cmake_minimum_required(VERSION 3.20)
project(IMGUI LANGUAGES CXX)

if(${GRAPHICS_API} STREQUAL "OpenGL")
    include_directories(${DEPENDENCIES_DIR}/glad/include)
endif()
if(${DISPLAY_API} STREQUAL "GLFW")
    include_directories(${DEPENDENCIES_DIR}/glfw/include)
endif()

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

# TODO : Add here checks for other APIs and add proper backend files to the project
if(${GRAPHICS_API} STREQUAL "OpenGL")
    set(IMGUI_SRCS ${IMGUI_SRCS}  
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_opengl3.cpp)
endif()

if(${DISPLAY_API} STREQUAL "GLFW")
    set(IMGUI_SRCS ${IMGUI_SRCS}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_glfw.cpp)
endif()

add_library(imgui STATIC ${IMGUI_SRCS} ${IMGUI_HDRS})
target_include_directories(imgui PUBLIC ${IMGUI_HDRS})

if (${GRAPHICS_API} STREQUAL "OpenGL")
    target_compile_definitions(imgui PRIVATE IMGUI_IMPL_OPENGL_LOADER_GLAD)
    target_link_libraries(imgui PRIVATE glfw OpenGL::GL)
endif()
