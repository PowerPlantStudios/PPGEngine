cmake_minimum_required(VERSION 3.20)
project(Engine VERSION 0.0.1 LANGUAGES CXX)

include_directories(${DEPENDENCIES_DIR}/glfw/include)
include_directories(${DEPENDENCIES_DIR}/glad/include)
include_directories(${DEPENDENCIES_DIR}/spdlog/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB_RECURSE PPGE_HDRS *.h)
file(GLOB_RECURSE PPGE_SRCS *.cpp)

add_library(Engine STATIC ${PPGE_SRCS} ${PPGE_HDRS})

target_include_directories(Engine 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/PPGE.h 
    PRIVATE ${PPGE_HDRS})
target_link_libraries(Engine 
    PRIVATE glfw glad)

if (WIN32)
    add_compile_definitions(PPGE_PLATFORM_WIN)
    add_compile_definitions(GLFW_INCLUDE_NONE)
endif(WIN32)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PPGE_HDRS})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PPGE_SRCS})
