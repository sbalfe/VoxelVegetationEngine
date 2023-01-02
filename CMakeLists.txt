cmake_minimum_required(VERSION 3.23)
project(Voxel)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

add_executable(Voxel
        src/ResourceManagers/Shader.cpp
        src/Glad/glad.c
        src/stb_image.h
        src/Engine/Camera.h
        src/Engine/Block.h
        src/Engine/Renderer.h
        src/main_test.cpp)

#add_executable(Voxel src/test.cpp)

find_package(OpenGL)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(Boost 1.40)

include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${glm_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${fmt_INCLUDE_DIRS} ./ ./include)
target_link_libraries(Voxel ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_LIBRARIES} ${glm_LIBRARIES} fmt::fmt)


