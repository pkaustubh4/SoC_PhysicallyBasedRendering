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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/* void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
   void mouse_callback(GLFWwindow* window, double xpos, double ypos); */

// settings
const unsigned int SCR_WIDTH = 640;
const unsigned int SCR_HEIGHT = 480;

/*
// mouse
bool mousePressed = false;
double lastMouseX = static_cast<float>(SCR_WIDTH) / 2.0f;
double lastMouseY = static_cast<float>(SCR_HEIGHT) / 2.0f;
bool rotateCamera = false;

// camera

float cameraYaw = -90.0f;   // Yaw is initialized to -90 degrees so that the initial camera front vector points towards the positive z-axis.
float cameraPitch = 0.0f;   // Pitch is initialized to 0 degrees. */

glm::vec3 cameraPos(0.0f, 0.0f, 3.0f);   // Camera position
glm::vec3 cameraFront(0.0f, 0.0f, -1.0f); // Camera front direction
glm::vec3 cameraUp(0.0f, 1.0f, 0.0f);    // Camera up vector

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrameTime = 0.0f;

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Glass Cube", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set the framebuffer_size_callback function for window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
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

        IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int));

        glm::mat4 proj = glm::perspective(glm::radians(60.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

        glm::mat4 mvp = proj * view * model;

        /* Create camera positions and light directions */

        glm::vec3 lightDirection(0.5f, 1.0f, 0.3f); // Direction of the light in world space
        lightDirection = glm::normalize(lightDirection);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // Color of the light (white in this case)

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

        /*
        // Set up mouse button and cursor position callbacks
        GLCall(glfwSetMouseButtonCallback(window, mouse_button_callback));
        GLCall(glfwSetCursorPosCallback(window, mouse_callback));
        */

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            // Calculate deltaTime
            double currentFrameTime = glfwGetTime();
            deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
            lastFrameTime = (float)currentFrameTime;
            
            // Process input
            processInput(window);

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

            // Update view matrix
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            view = glm::translate(view, glm::vec3 (0, 0, -100));

            // Update model matrix
            model = glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            // Update MVP matrix
            mvp = proj * view * model;

            shader.Bind();
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

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(50.0) * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

/*
void updateCameraVectors()
{
    // Calculate the new camera front vector
    glm::vec3 front;
    front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    front.y = sin(glm::radians(cameraPitch));
    front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
    cameraFront = glm::normalize(front);

    // Recalculate the camera right and up vectors
    glm::vec3 right;
    right.x = sin(glm::radians(cameraYaw - 90.0f));
    right.y = 0.0f;
    right.z = cos(glm::radians(cameraYaw - 90.0f));
    cameraUp = glm::normalize(glm::cross(right, cameraFront));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            mousePressed = true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            rotateCamera = true;
        }
        else if (action == GLFW_RELEASE)
        {
            mousePressed = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            rotateCamera = false; // Set the flag to false when releasing the mouse button
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (rotateCamera) // Only update rotation if the mouse button is clicked
    {
        double xOffset = xpos - lastMouseX;
        double yOffset = lastMouseY - ypos; // Reversed since y-coordinates go from bottom to top

        lastMouseX = xpos;
        lastMouseY = ypos;

        float sensitivity = 0.05f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;

        cameraYaw += xOffset;
        cameraPitch += yOffset;

        // Limit the pitch to avoid camera flipping
        if (cameraPitch > 89.0f)
            cameraPitch = 89.0f;
        if (cameraPitch < -89.0f)
            cameraPitch = -89.0f;

        // Update cameraFront based on new yaw and pitch
        glm::vec3 front;
        front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        front.y = sin(glm::radians(cameraPitch));
        front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
        cameraFront = glm::normalize(front);

        updateCameraVectors();
    }
}
*/
