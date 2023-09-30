cmake_minimum_required(VERSION 3.12)
project(my_glfw_glew_project)

# Add the following lines to find GLFW and GLEW
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(my_glfw_glew_project src/Application.cpp src/Renderer.cpp src/VertexBuffer.cpp src/IndexBuffer.cpp)

target_link_libraries(my_glfw_glew_project OpenGL::GL GLEW::GLEW glfw)
