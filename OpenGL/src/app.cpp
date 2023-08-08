#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "debug.h"
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
    window = glfwCreateWindow(640, 480, "Glass Cube", NULL, NULL);
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

        glm::mat4 proj = glm::perspective(glm::radians(60.0f), 640.0f / 480.0f, 0.1f, 1000.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -500));
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 mvp = proj * view * model;

        /* Create camera positions and light directions */
        
        glm::vec3 lightDirection(0.5f, 1.0f, 0.3f); // Direction of the light in world space
        lightDirection = glm::normalize(lightDirection);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // Color of the light (white in this case)

        glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);   // Camera position
        glm::vec3 cameraFront(0.0f, 0.0f, -1.0f); // Camera front direction
        glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);    // Camera up vector


        Shader shader("res/shaders/texture.shader");
        shader.Bind();

        Renderer renderer;

        // Load and bind texture images
        unsigned int texture1 = renderer.LoadTexture("res/textures/text1.png");
        unsigned int texture2 = renderer.LoadTexture("res/textures/text2.png");

        // Set the texture slots for each texture in the shader
        shader.SetUniform1i("u_Texture1", 0); // 0 corresponds to texture slot 0
        shader.SetUniform1i("u_Texture2", 1); // 1 corresponds to texture slot 1

        // Set up light properties
        shader.SetUniform3f("u_LightDirection", lightDirection.x, lightDirection.y, lightDirection.z);
        shader.SetUniform3f("u_LightColor", lightColor.x, lightColor.y, lightColor.z);
        
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.Unbind();

        // Update view matrix once outside the loop
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

            shader.Bind();
            shader.SetUniformMat4f("u_MVP", mvp);

            // Activate texture slots
            GLCall(glActiveTexture(GL_TEXTURE0));
            GLCall(glBindTexture(GL_TEXTURE_2D, texture1));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

            GLCall(glActiveTexture(GL_TEXTURE1));
            GLCall(glBindTexture(GL_TEXTURE_2D, texture2));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));

            model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            mvp = proj * view * model;
            shader.SetUniformMat4f("u_MVP", mvp);

            // Set up view and model matrices
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

            // Update view matrix
            view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -100));

            // Update model matrix
            model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            // Update MVP matrix
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
