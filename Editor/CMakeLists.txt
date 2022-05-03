cmake_minimum_required(VERSION 3.20)
project(Editor VERSION 0.0.1 LANGUAGES CXX)

# Include directories of editor project
include_directories(
    ${DEPENDENCIES_SRC_DIR}/spdlog/include
    ${DEPENDENCIES_SRC_DIR}/imgui/src
    ${DEPENDENCIES_SRC_DIR}/math/simplemath/include
    ${DEPENDENCIES_SRC_DIR}/assimp/include
    ${DEPENDENCIES_BIN_DIR}/assimp/include
    ${DEPENDENCIES_SRC_DIR}/entt/src
    ${CMAKE_CURRENT_SOURCE_DIR})
if(WIN32)
    include_directories(
        ${DEPENDENCIES_SRC_DIR}/directxtk/src/Inc)
endif()

# Linked libraries of editor project
link_libraries(Engine)

# Preprocessor definitions of editor project
add_compile_definitions(
    $<$<CONFIG:Debug>:EDITOR_DEBUG>
    $<$<CONFIG:Debug>:PPGE_DEBUG>
    $<$<CONFIG:Release>:PPGE_RELEASE>
    $<$<CONFIG:Release>:EDITOR_RELEASE>)

# Recursively search header files
file(GLOB EDITOR_HDRS 
    ${PPGE_EDITOR_DIR}/widgets/*.h)
set(EDITOR_HDRS ${EDITOR_HDRS} 
    ${PPGE_EDITOR_DIR}/editor.h)
# Recursively search source files
file(GLOB EDITOR_SRCS 
    ${PPGE_EDITOR_DIR}/widgets/*.cpp)
set(EDITOR_SRCS ${EDITOR_SRCS} 
    ${PPGE_EDITOR_DIR}/editor.cpp
    ${PPGE_EDITOR_DIR}/main.cpp)

# Add editor executable
if(WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${EDITOR_SRCS} ${EDITOR_HDRS})
else()
    add_executable(${PROJECT_NAME} ${EDITOR_SRCS} ${EDITOR_HDRS})
endif()

# Create solution source grouping
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EDITOR_SRCS})
