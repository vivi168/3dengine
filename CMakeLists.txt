cmake_minimum_required(VERSION 3.10)

project(3dengine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TARGET_H
        ${CMAKE_SOURCE_DIR}/3dengine/Camera.h
        ${CMAKE_SOURCE_DIR}/3dengine/InputManager.h
        ${CMAKE_SOURCE_DIR}/3dengine/Model.h
        ${CMAKE_SOURCE_DIR}/3dengine/HeightMap.h
        ${CMAKE_SOURCE_DIR}/3dengine/Mesh.h
        ${CMAKE_SOURCE_DIR}/3dengine/Shader.h
        )
set(TARGET_CPP
        ${CMAKE_SOURCE_DIR}/3dengine/Camera.cpp
        ${CMAKE_SOURCE_DIR}/3dengine/HeightMap.cpp
        ${CMAKE_SOURCE_DIR}/3dengine/Mesh.cpp
        ${CMAKE_SOURCE_DIR}/3dengine/Model.cpp
        ${CMAKE_SOURCE_DIR}/3dengine/main.cpp
        ${CMAKE_SOURCE_DIR}/3dengine/vendor/gl3w.c
        )

find_package(SDL2 REQUIRED)
include_directories(
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/3dengine/include
        ${CMAKE_SOURCE_DIR}/3dengine/vendor/glm
        )

add_executable(3dengine.exe ${TARGET_H} ${TARGET_CPP})
target_link_libraries(3dengine.exe ${SDL2_LIBRARIES})
