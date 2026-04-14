#include <iostream>
#include <ostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "shader_utils.h"

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


    std::string vertexShaderName = "res/shaders/triangle.vert";
    std::string fragmentShaderName = "res/shaders/triangle.frag";

    GLuint shaderProgram = createProgram(vertexShaderName, fragmentShaderName);

    float vertices[] = {
        // first triangle
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        // second triangle
        0.5f, -0.5f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    };

    GLuint VBO; // ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object

    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind = activate
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    GLuint posAtriLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAtriLocation,            // знайдена командою posAtriLocation позиція атрибуту у шейдері
        2,                          // 2 компоненти: x, y
        GL_FLOAT,                   // тип даних
        GL_FALSE,                   // не нормалізувати
        5 * sizeof(float),          // stride: 5 float-a на вершину
        (void*)0                    // offset: починаємо з 0
        );
    glEnableVertexAttribArray(posAtriLocation);

    // color attribute
    GLuint colorAtriLocation = glGetAttribLocation(shaderProgram, "aColor");
    glVertexAttribPointer(
        colorAtriLocation,
        3,
        GL_FLOAT,
        GL_FALSE,
        5 * sizeof(float),
        (void*)(2 * sizeof(float))
        );
    glEnableVertexAttribArray(colorAtriLocation);

    glBindVertexArray(0); // деактивувати VAO

    /* Loop until the user closes the window */
    do
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
