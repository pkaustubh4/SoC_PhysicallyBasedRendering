#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "renderer.h"

#include "vb.h"
#include "ib.h"
#include "va.h"
#include "shader.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

int main(void)
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
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float vertices[] = {
    -25.0f, -25.0f, -25.0f,  0.0f,   0.0f,
     25.0f, -25.0f, -25.0f,  100.0f, 0.0f,
     25.0f,  25.0f, -25.0f,  100.0f, 100.0f,
     25.0f,  25.0f, -25.0f,  100.0f, 100.0f,
    -25.0f,  25.0f, -25.0f,  0.0f,   100.0f,
    -25.0f, -25.0f, -25.0f,  0.0f,   0.0f,

    -25.0f, -25.0f,  25.0f,  0.0f,   0.0f,
     25.0f, -25.0f,  25.0f,  100.0f, 0.0f,
     25.0f,  25.0f,  25.0f,  100.0f, 100.0f,
     25.0f,  25.0f,  25.0f,  100.0f, 100.0f,
    -25.0f,  25.0f,  25.0f,  0.0f,   100.0f,
    -25.0f, -25.0f,  25.0f,  0.0f,   0.0f,

    -25.0f,  25.0f,  25.0f,  100.0f, 0.0f,
    -25.0f,  25.0f, -25.0f,  100.0f, 100.0f,
    -25.0f, -25.0f, -25.0f,  0.0f,   100.0f,
    -25.0f, -25.0f, -25.0f,  0.0f,   100.0f,
    -25.0f, -25.0f,  25.0f,  0.0f,   0.0f,
    -25.0f,  25.0f,  25.0f,  100.0f, 0.0f,

     25.0f,  25.0f,  25.0f,  100.0f, 0.0f,
     25.0f,  25.0f, -25.0f,  100.0f, 100.0f,
     25.0f, -25.0f, -25.0f,  0.0f,   100.0f,
     25.0f, -25.0f, -25.0f,  0.0f,   100.0f,
     25.0f, -25.0f,  25.0f,  0.0f,   0.0f,
     25.0f,  25.0f,  25.0f,  100.0f, 0.0f,

    -25.0f, -25.0f, -25.0f,  0.0f,   100.0f,
     25.0f, -25.0f, -25.0f,  100.0f, 100.0f,
     25.0f, -25.0f,  25.0f,  100.0f, 0.0f,
     25.0f, -25.0f,  25.0f,  100.0f, 0.0f,
    -25.0f, -25.0f,  25.0f,  0.0f,   0.0f,
    -25.0f, -25.0f, -25.0f,  0.0f,   100.0f,

    -25.0f,  25.0f, -25.0f,  0.0f,   100.0f,
     25.0f,  25.0f, -25.0f,  100.0f, 100.0f,
     25.0f,  25.0f,  25.0f,  100.0f, 0.0f,
     25.0f,  25.0f,  25.0f,  100.0f, 0.0f,
    -25.0f,  25.0f,  25.0f,  0.0f,   0.0f,
    -25.0f,  25.0f, -25.0f,  0.0f,   100.0f
        };

        unsigned int indices[] = {
            0,  1,  2,   2,  3,  0,  // Front face
            4,  5,  6,   6,  7,  4,  // Back face
            8,  9,  10,  10, 11, 8,  // Left face
            12, 13, 14,  14, 15, 12, // Right face
            16, 17, 18,  18, 19, 16, // Top face
            20, 21, 22,  22, 23, 20  // Bottom face
        };

        VertexArray va;
        VertexBuffer vb(vertices, 36 * 5 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(3); // For x, y, z
        layout.Push<float>(2); // For u, v

        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, sizeof(indices)/sizeof(unsigned int));

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), 640.0f/480.0f, 0.1f, 1000.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -300));
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 mvp = proj * view * model;

        /* Load texture images and create texture objects */
        
        Shader shader("res/shaders/texture.shader");
        shader.Bind();

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();
            shader.SetUniform4f("u_color", 0.67f, 0.51f, 0.73f, 1.0f);

            model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);

            va.Bind();
            vb.Bind();
            
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            glfwPollEvents();

        }
    }
    glfwTerminate();
    return 0;
}
#include "vb.h"
#include "ib.h"
#include "va.h"
#include "shader.h"

#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

int main(void)
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
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
            100.0f, 100.0f, // 0
            200.0f, 100.0f, // 1
            200.0f, 200.0f, // 2
            100.0f, 200.0f  // 3
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(-0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));

        glm::mat4 mvp = proj * view * model;

        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_color", 0.0f, 1.0f, 1.0f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        float r = 0.0f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            shader.Bind();
            shader.SetUniform4f("u_color", 0.0f, r, 1.0f, 1.0f);

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            glfwPollEvents();

        }
    }
    glfwTerminate();
    return 0;
}
