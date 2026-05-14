#include <iostream>
#include <ostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "../utils/shader_utils.h"
#include "../utils/texture.h"

#include <chrono>
#include <thread>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);


    std::string vertexShaderName = "res/shaders/triangle.vert";
    std::string fragmentShaderName = "res/shaders/triangle.frag";

    GLuint shaderProgram = createProgram(vertexShaderName, fragmentShaderName);

    GLint colorUniformPos = glGetUniformLocation(shaderProgram, "uColor");
    GLuint shiftUniformPos = glGetUniformLocation(shaderProgram, "uShift");
    GLint textureUniformPos = glGetUniformLocation(shaderProgram, "uTexture");
    GLint transformUniformPos = glGetUniformLocation(shaderProgram, "uTransform");

    float rectVertices[] {
        /* координати */ -0.5f, -0.5f, /* текстурні координати */ 0.0f, 0.0f, // 0
        /* координати */ 0.5f, -0.5f,  /* текстурні координати */ 1.0f, 0.0f, // 1
        /* координати */ 0.5f, 0.5f,   /* текстурні координати */ 1.0f, 1.0f, // 2
        /* координати */ -0.5f, 0.5f,  /* текстурні координати */ 0.0f, 1.0f, // 3
    };

    unsigned int squareIndices[] {
        0, 1, 2,
        0, 2, 3,
    };

    GLuint VBO, rectIndexVBO; // ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &rectIndexVBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind = activate
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectVertices), rectVertices, GL_STATIC_DRAW);

    // position attribute
    GLuint posAtriLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAtriLocation,            // знайдена командою posAtriLocation позиція атрибуту у шейдері
        2,                          // 2 компоненти: x, y
        GL_FLOAT,                   // тип даних
        GL_FALSE,                   // не нормалізувати
        4 * sizeof(float),          // stride: 4 float-a на вершину
        (void*)0                    // offset: починаємо з 0
        );
    glEnableVertexAttribArray(posAtriLocation);

    GLuint textureCoordsAtriLocation = glGetAttribLocation(shaderProgram, "aUV");
    glVertexAttribPointer(
        textureCoordsAtriLocation,
        2,
        GL_FLOAT,
        GL_FALSE,
        4 * sizeof(float),
        (void*)(2 * sizeof(float))
        );
    glEnableVertexAttribArray(textureCoordsAtriLocation);

    glBindVertexArray(0); // деактивувати VAO

    unsigned int texture1 = loadTexture("res/textures/house.jpg");

    bool isPaused = false;
    bool spaceWasPressed = false;

    float angle = 0.0f;
    float rotationSpeed = glm::radians(90.0f);

    auto previousTime = std::chrono::high_resolution_clock::now();

    const double targetFrameTime = 1.0 / 60.0;

    /* Loop until the user closes the window */
    do
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> delta = currentTime - previousTime;
        previousTime = currentTime;

        float deltaTime = delta.count();

        int spaceState = glfwGetKey(window, GLFW_KEY_SPACE);

        if (spaceState == GLFW_PRESS && !spaceWasPressed) {
            isPaused = !isPaused;
            spaceWasPressed = true;
        }

        if (spaceState == GLFW_RELEASE) {
            spaceWasPressed = false;
        }

        if (!isPaused) {
            angle += rotationSpeed * deltaTime;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::rotate(
            transform,
            angle,
            glm::vec3(0.0f, 0.0f, 1.0f)
            );

        glUniformMatrix4fv(
            transformUniformPos,
            1,
            GL_FALSE,
            glm::value_ptr(transform)
            );

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(textureUniformPos, 0);

        glBindVertexArray(VAO);

        glDrawElements(
            GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> frameDuration = frameEnd - frameStart;

        double sleepTime = targetFrameTime - frameDuration.count();

        if (sleepTime > 0.0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        }

    } while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &rectIndexVBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture1);

    glfwTerminate();
    return 0;
}
