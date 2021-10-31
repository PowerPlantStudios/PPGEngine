cmake_minimum_required(VERSION 3.20)
project(PPGE)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenGL REQUIRED)

set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/Thirdparty)
set(PPGE_CORE_DIR ${PROJECT_SOURCE_DIR}/PPGE)
set(PPGE_EDITOR_DIR ${PROJECT_SOURCE_DIR}/Editor)
set(GAME_DIR ${PROJECT_SOURCE_DIR}/Sandbox)

add_subdirectory(PPGE)
add_subdirectory(Sandbox)
add_subdirectory(Thirdparty)
