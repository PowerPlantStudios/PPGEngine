cmake_minimum_required (VERSION 3.11)
project (simpletexture LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/Inc)

set(SIMPLETEXTURE_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Inc/DDSTextureLoader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Inc/DirectXHelpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Inc/WICTextureLoader.h)

set(SIMPLETEXTURE_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/DDS.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/DDSTextureLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/DirectXHelpers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/LoaderHelpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/pch.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/PlatformHelpers.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/WICTextureLoader.cpp)


add_library(${PROJECT_NAME} STATIC ${SIMPLETEXTURE_SRCS} ${SIMPLETEXTURE_HDRS})

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16")
    target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/pch.h)
endif()

source_group(Inc REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/src/Inc/*.*)
source_group(Src REGULAR_EXPRESSION ${CMAKE_CURRENT_SOURCE_DIR}/src/Src/*.*)

target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/Inc>)

if(MSVC)
    string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
    string(REPLACE "/W3 " "/Wall " CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
endif()
