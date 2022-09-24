cmake_minimum_required(VERSION 3.20)
project(Samples VERSION 0.0.1 LANGUAGES CXX)

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
    add_executable(Simple WIN32 ${CMAKE_CURRENT_SOURCE_DIR}/Simple.cpp)
else()
    add_executable(Simple ${CMAKE_CURRENT_SOURCE_DIR}/Simple.cpp)
endif()

target_compile_definitions(Simple PRIVATE 
    $<$<CONFIG:Debug>:PPGE_DEBUG> 
    $<$<CONFIG:Release>:PPGE_RELEASE>)

target_link_libraries(Simple PRIVATE Engine)
