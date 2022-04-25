cmake_minimum_required(VERSION 3.20)
project(SIMPLEMATH LANGUAGES CXX)

include_directories(
    ${DEPENDENCIES_SRC_DIR}/math/dxmath/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SIMPLEMATH_HDRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/simple_math.h
    ${CMAKE_CURRENT_SOURCE_DIR}/include/simple_math.inl)

set(SIMPLEMATH_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simple_math.cpp)

add_library(simplemath STATIC ${SIMPLEMATH_SRCS} ${SIMPLEMATH_HDRS})
target_include_directories(simplemath PUBLIC ${SIMPLEMATH_HDRS})
target_link_libraries(simplemath PRIVATE DirectXMath)
