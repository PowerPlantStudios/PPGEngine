cmake_minimum_required(VERSION 3.20)
project(GLAD LANGUAGES C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 23)

set(GLAD_SRCS 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)

set(GLAD_HDRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h)

add_library(glad STATIC ${GLAD_SRCS} ${GLAD_HDRS})
target_include_directories(glad PUBLIC ${GLAD_HDRS})
