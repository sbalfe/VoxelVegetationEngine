cmake_minimum_required(VERSION 3.23)
project(Voxel)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")


add_library(imgui # Main Imgui files
        ./src/ImGui/imgui.cpp ./src/ImGui/imgui_draw.cpp
        ./src/ImGui/imgui_tables.cpp ./src/ImGui/imgui_widgets.cpp
        # SDL2+OpenGL-specific files
        ./src/ImGui/imgui_impl_sdl.cpp
        ./src/ImGui/imgui_impl_opengl3.cpp src/Engine/Voxel/Voxel.cpp src/Engine/GUI/GUI.cpp src/Engine/GUI/GUI.h)

add_executable(Voxel
        src/Engine/Shaders/Shader.cpp
        src/Glad/glad.c
        src/stb_image.h
        src/main.cpp
        src/Engine/Position.h
        src/Engine/Camera/Camera.cpp
        src/Engine/Renderer.cpp
        src/Engine/Chunk/Chunk.cpp
        src/Engine/Voxel/Voxel.cpp)


find_package(OpenGL)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(fmt REQUIRED)
find_package(Boost 1.40)

include_directories(${OPENGL_INCLUDE_DIRS} ${SDL2_INCLUDE_DIRS} ${glm_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${fmt_INCLUDE_DIRS} ./ ./include ../ )
include_directories(./src/ImGui)
target_link_libraries(Voxel ${OPENGL_LIBRARIES} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${Boost_LIBRARIES} ${glm_LIBRARIES} fmt::fmt imgui)


