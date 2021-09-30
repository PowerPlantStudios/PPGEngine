cmake_minimum_required(VERSION 3.20)
project(Sandbox VERSION 0.0.1 LANGUAGES CXX)

include_directories(
    ${DEPENDENCIES_DIR}/spdlog/include
    ${DEPENDENCIES_DIR}/math/simplemath/include)

add_executable(Game ${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp)
target_link_libraries(Game PRIVATE Engine)
