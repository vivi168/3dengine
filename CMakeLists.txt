cmake_minimum_required(VERSION 3.10)

project(3dengine)

find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(Lua REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
    Camera.cpp
    HeightMap.cpp
    main.cpp
    Mesh.cpp
    Model.cpp
    Renderer.cpp
    Water.cpp
    vendor/gl3w.c)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIR} ${SDL2_INCLUDE_DIR} ${LUA_INCLUDE_DIR}})
target_link_libraries(${PROJECT_NAME} ${GLM_LIBRARIES} ${SDL2_LIBRARIES} ${LUA_LIBRARIES})

file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
