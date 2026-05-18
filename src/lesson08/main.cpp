#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../utils/shader_utils.h"
#include "../utils/texture.h"
#include "glm/common.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.inl"


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

    glfwSwapInterval(1); // синхронізує рендер цикл із частотою екрана

    glClearColor(1.0, 1.0, 1.0, 1.0);

    std::string vertexShaderName = "res/shaders/rect.vert";
    std::string fragmentShaderName = "res/shaders/rect.frag";
    GLuint shaderProgram = createProgram(
        vertexShaderName,
        fragmentShaderName);

    GLint colorUniformPos = glGetUniformLocation(shaderProgram, "uColor");
    GLint shiftUniformPos = glGetUniformLocation(shaderProgram, "uShift");
    GLint texture0_loc = glGetUniformLocation(shaderProgram, "uTexture0");
    GLint texture1_loc = glGetUniformLocation(shaderProgram, "uTexture1");
    GLint texture2_loc = glGetUniformLocation(shaderProgram, "uTexture2");
    GLint t_loc = glGetUniformLocation(shaderProgram, "uT");
    GLint transform_loc = glGetUniformLocation(shaderProgram, "uTransformation");

    float vertices[] = {
        /* координати */  -0.5f, -0.5f,  /* тестурні координати */  0.0f, 0.0f,  //  0
        /* координати */   0.5f, -0.5f,  /* тестурні координати */  1.0f, 0.0f, // 1
        /* координати */   0.5f, 0.5f,   /* тестурні координати */  1.0f, 1.0f, // 2
        /* координати */  -0.5f, 0.5f,   /* тестурні координати */  0.0f, 1.0f, // 3
    };

    unsigned int indices[] = {
        0, 1, 2, // перший трикутник
        0, 2, 3, // другий трикутник
    };

    GLuint VBO, indexBuffer; // data - ідентифікатор для даних - місток CPU та GPU
    GLuint VAO; // vertex array object

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &indexBuffer);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind = activate
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint posAttribLocation = glGetAttribLocation(shaderProgram, "aPos");
    glVertexAttribPointer(
        posAttribLocation,                  // знайдена командою glGetAttribLocation позиція атрибуту у шейдері
        2,                  // 2 компоненти: x, y
        GL_FLOAT,           // тип даних
        GL_FALSE,           // не нормалізувати
        4 * sizeof(float),  // stride: 2 float-а на вершину
        (void*)0            // offset: починаємо з 0
    );
    glEnableVertexAttribArray(posAttribLocation);

    GLuint textureCoordsAttribLocation = glGetAttribLocation(shaderProgram, "aUV");
    glVertexAttribPointer(
        textureCoordsAttribLocation,                  // знайдена командою glGetAttribLocation позиція атрибуту у шейдері
        2,                  // 2 компоненти: u, v
        GL_FLOAT,           // тип даних
        GL_FALSE,           // не нормалізувати
        4 * sizeof(float),  // stride: 4 float-а на вершину
        (void*)(2 * sizeof(float))        // offset: починаємо з 2
    );
    glEnableVertexAttribArray(textureCoordsAttribLocation);

    glBindVertexArray(0); // деактивувати VAO

    unsigned int texture0 = loadTexture("res/textures/0.jpeg");
    unsigned int texture1 = loadTexture("res/textures/1.jpeg");
    unsigned int texture2 = loadTexture("res/textures/2.jpeg");

    float t = 0.0f;
    float deltaTime = 1.0f / 60.0f;

    auto transformation = glm::mat4(1.0f);

    // transformation = glm::translate(transformation, glm::vec3(1.0f, 0.0f, 0.0f));
    // transformation = glm::rotate(transformation, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    // transformation = glm::scale(transformation, glm::vec3(2.0f, 2.0f, 2.0f));

    /* Loop until the user closes the window */
    do
    {
        t += deltaTime;
        if (t >= 1.0f || t <= 0.0f) {
            deltaTime = -deltaTime;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);

        glUniform1f(t_loc, t);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glUniform1i(texture0_loc, 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(texture1_loc, 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(texture2_loc, 2);

        transformation = glm::rotate(transformation, glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transform_loc, 1, GL_FALSE, glm::value_ptr(transformation));
        
        glBindVertexArray(VAO);

        // glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE));

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &indexBuffer);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);
    glDeleteTextures(1, &texture0);
    glDeleteTextures(1, &texture1);
    glDeleteTextures(1, &texture2);

    glfwTerminate();
    return 0;
}
