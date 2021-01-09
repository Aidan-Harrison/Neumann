#include <iostream>
// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// GLM
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
// Custom
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShaderHandler.h"
#include "Camera.h"
// #include "Input.h" // Come back to this

// Prototypes
void FrameBufferSize(GLFWwindow *window, short width, short height);
void MouseCall(GLFWwindow* window, double xPos, double yPos); // Capitalize
void ScrollCall(GLFWwindow* window, double xOffset, double yOffset);
void ProccessInput(GLFWwindow* window);

// Global Settings | Abstract later
bool firstMouse = false;
double lastX, lastY;
float deltaTime = 0.0f; // Delta of current and last frame
float lastFrame = 0.0f; // Time of last frame
const short screenWidth = 800;
const short screenHeight = 600;

Camera cam;

/// Main =============================================================================================================================================
int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "Neumann | Graphics Engine", NULL, NULL); // CHANGE AFTER RENDERER CLASS MADE
    if(!window) {
        std::cerr << "Failed to create window\n"; glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hides cursor
    glfwSetCursorPosCallback(window, MouseCall);
    glfwSetScrollCallback(window, ScrollCall);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
        std::cerr << "Error! GLEW not ok\n";
    glEnable(GL_DEPTH_TEST);

    /// Main Body ====================================================================================================================================
        // Vertices
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, 
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f, 
         0.5f, -0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 

        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f, -0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f,  0.5f,  0.5f, 

         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f, 

        -0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f, -0.5f, 
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f,  0.5f, 
        -0.5f, -0.5f, -0.5f, 

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f, 
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f, 
        -0.5f,  0.5f, -0.5f, 
    };

    glm::vec3 cubePos[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, -2.0f)
    };

        // Indicies
    /*
    unsigned int indicies[] = {
        0, 1, 3, // First triangle
        1, 2, 3
        // IndexBuffer ib(indicies, sizeof(indicies));
        // ib.Bind();
    };
    */

        // Vertex buffer, Index buffer & V-Array object
    VertexArray va; // Auto generates with constructor
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.Push<float>(2); // Check if correct size?
    vb.Bind();
    va.AddBuffer(vb, layout);
    Shader shader("res/Shaders/shader.vs", "res/Shaders/shader.fs");

    vb.Unbind();

    ///* Loop until the user closes the window */ ==========================================================================================================
    while(!glfwWindowShouldClose(window)) {
        /* Input */
        ProccessInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.Use();

            // Deltatime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /// Transformations -----------------------------------------------------------------------------------------------------------------------
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        cam.Transformation(cam.m_View, cam.m_Projection);
        cam.Translate(cam.m_View);

            // Apply transformations
        unsigned int modelLoc = glGetUniformLocation(shader.m_ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.m_ID, "view");
        unsigned int projLoc  = glGetUniformLocation(shader.m_ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &cam.m_View[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.m_Projection));

        /// Lighting ------------------------------------------------------------------------------------------------------------------------------
        glGetUniformLocation(shader.m_ID, "objectColour");
        glGetUniformLocation(shader.m_ID, "lightColour");
        shader.SetVec3("objectColour", 1.0f, 0.5f, 0.31f);
        shader.SetVec3("lightColour", 1.0f, 1.0f, 1.0f);

        glm::vec3 lightPos = glm::vec3(-3.0f, 0.0f, 0.0f);
        shader.SetVec3("lightPos", lightPos);
        shader.SetVec3("viewPos", cam.m_CamPos);

        /// Draw cubes -----------------------------------------------------------------------------------------------------------------------------
        va.Bind();
        for(unsigned int i = 0; i < 2; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePos[i]);
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        }
            // GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /// Deallocate resources =========================================================================================================================
    va.~VertexArray();
    vb.~VertexBuffer();
    // ib.~IndexBuffer();

    glfwTerminate();
    return 0;
}

// Alter window size
void FrameBufferSize(GLFWwindow *window, short width, short height) {
    glViewport(0, 0, width, height);
}

void MouseCall(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // Y-coords go top to bot, so inversed
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.05f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;
    cam.yaw += xOffset;
    cam.pitch += yOffset;

    // Clamp
    if (cam.pitch > 89.0f)
        cam.pitch = 89.0f;
    if (cam.pitch < -89.0f)
        cam.pitch = -89.0f;

    glm::vec3 direction = glm::vec3(0.0f);
    direction.x = cos(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    direction.y = sin(glm::radians(cam.pitch));
    direction.z = sin(glm::radians(cam.yaw)) * cos(glm::radians(cam.pitch));
    cam.m_CamFront = glm::normalize(direction);
}

void ScrollCall(GLFWwindow* window, double xOffset, double yOffset) {
    cam.FOV -= static_cast<float>(yOffset);
    if (cam.FOV < 1.0f)
        cam.FOV = 1.0f;
    if (cam.FOV > 75.0f)
        cam.FOV = 75.0f;
}

void ProccessInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float camSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.m_CamPos += camSpeed * cam.m_CamFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.m_CamPos -= camSpeed * cam.m_CamFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.m_CamPos -= glm::normalize(glm::cross(cam.m_CamFront, cam.m_CamUp)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.m_CamPos += glm::normalize(glm::cross(cam.m_CamFront, cam.m_CamUp)) * camSpeed;
}