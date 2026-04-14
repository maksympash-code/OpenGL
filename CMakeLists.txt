cmake_minimum_required(VERSION 4.2)
project(OpenGL)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

set(libs
        ${OPENGL_LIBRARY}
        glfw
        glad
        glm
)

add_subdirectory(external)

set(LESSON_TARGET lesson01)

add_executable(${LESSON_TARGET} src/lesson01/main.cpp)

target_link_libraries(${LESSON_TARGET} ${libs})



