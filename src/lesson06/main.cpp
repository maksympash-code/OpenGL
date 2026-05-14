#include <iostream>
#include <ostream>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "../utils/shader_utils.h"
#include "../utils/texture.h"

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

    float rectVertices[] {
        /* координати */ -0.9f, -0.4f, /* текстурні координати */ 0.0f, 0.0f, // 0
        /* координати */ -0.4f, -0.4f,  /* текстурні координати */ 1.0f, 0.0f, // 1
        /* координати */ -0.4f, 0.4f,   /* текстурні координати */ 1.0f, 1.0f, // 2
        /* координати */ -0.9f, 0.4f,  /* текстурні координати */ 0.0f, 1.0f, // 3

        /* координати */ -0.25f, -0.4f, /* текстурні координати */ 0.0f, 0.0f, // 4
        /* координати */ 0.25f, -0.4f,  /* текстурні координати */ 1.0f, 0.0f, // 5
        /* координати */ 0.25f, 0.4f,   /* текстурні координати */ 1.0f, 1.0f, // 6
        /* координати */ -0.25f, 0.4f,  /* текстурні координати */ 0.0f, 1.0f, // 7

        /* координати */ 0.4f, -0.4f, /* текстурні координати */ 0.0f, 0.0f, // 8
        /* координати */ 0.9f, -0.4f,  /* текстурні координати */ 1.0f, 0.0f, // 9
        /* координати */ 0.9f, 0.4f,   /* текстурні координати */ 1.0f, 1.0f, // 10
        /* координати */ 0.4f, 0.4f,  /* текстурні координати */ 0.0f, 1.0f, // 11
    };

    unsigned int rectIndices[] {
        // rectangle 1
        0, 1, 2,
        0, 2, 3,

        // rectangle 2
        4, 5, 6,
        4, 6, 7,

        // rectangle 3
        8, 9, 10,
        8, 10, 11,
    };

    GLuint VBO, rectIndexVBO; // ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &rectIndexVBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndices), rectIndices, GL_STATIC_DRAW);

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
    unsigned int texture2 = loadTexture("res/textures/girl.jpg");
    unsigned int texture3 = loadTexture("res/textures/dog.jpg");


    /* Loop until the user closes the window */
    do
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glUniform1i(textureUniformPos, 0);

        glBindVertexArray(VAO);

        // rect 1 with texture 1
        glBindTexture(GL_TEXTURE_2D, texture1);
        glDrawElements(
            GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            0
            );

        // rect 2 with texture 2
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawElements(GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            (void*)(6 * sizeof(unsigned int))
            );

        // rect 3 with texture 3
        glBindTexture(GL_TEXTURE_2D, texture3);
        glDrawElements(GL_TRIANGLES,
            6,
            GL_UNSIGNED_INT,
            (void*)(12 * sizeof(unsigned int))
            );
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &rectIndexVBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    glfwTerminate();
    return 0;
}
