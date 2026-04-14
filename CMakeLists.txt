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

######### LESSON_1 #########
set(LESSON_TARGET lesson01)
set(LESSON_DIR src/${LESSON_TARGET})

add_executable(${LESSON_TARGET} ${LESSON_DIR}/main.cpp)

target_link_libraries(${LESSON_TARGET} ${libs})

######### LESSON_2 #########
set(LESSON_TARGET lesson02)
set(LESSON_DIR src/${LESSON_TARGET})

add_executable(${LESSON_TARGET} ${LESSON_DIR}/main.cpp
        ${LESSON_DIR}/shader_utils.cpp
)

target_link_libraries(${LESSON_TARGET} ${libs})
target_include_directories(${LESSON_TARGET} PRIVATE LESSON_DIR)

add_custom_command(
        TARGET ${LESSON_TARGET} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/${LESSON_DIR}/res
        $<TARGET_FILE_DIR:${LESSON_TARGET}>/res
)


