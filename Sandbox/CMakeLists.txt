cmake_minimum_required(VERSION 3.20)
project(Sandbox VERSION 0.0.1 LANGUAGES CXX)

include_directories(
    ${DEPENDENCIES_SRC_DIR}/spdlog/include
    ${DEPENDENCIES_SRC_DIR}/imgui/src
    ${DEPENDENCIES_SRC_DIR}/math/simplemath/include
    ${DEPENDENCIES_SRC_DIR}/assimp/include
    ${DEPENDENCIES_BIN_DIR}/assimp/include
    ${DEPENDENCIES_SRC_DIR}/entt/src)
if(WIN32)
    include_directories(
        ${DEPENDENCIES_SRC_DIR}/directxtk/src/Inc)
endif()

if(WIN32)
    add_executable(Game WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp)
else()
    add_executable(Game ${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp)
endif()

target_compile_definitions(Game PRIVATE 
    $<$<CONFIG:Debug>:PPGE_DEBUG> 
    $<$<CONFIG:Release>:PPGE_RELEASE>)

target_link_libraries(Game PRIVATE Engine)
