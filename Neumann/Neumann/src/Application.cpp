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

// Prototypes
void FrameBufferSize(GLFWwindow *window, short width, short height);
void ProcessInput(GLFWwindow *window);
void mouseCall(GLFWwindow *window, double xPos, double yPos);
void scrollCall(GLFWwindow *window, double xOffset, double yOffset);

// Global Settings
bool firstMouse = true;
const short screenWidth = 800;
const short screenHeight = 600;
    // Timing
float deltaTime = 0.0f; // Delta of current and last frame
float lastFrame = 0.0f; // Time of last frame

/// Create Camera -------------------------------------------------------------------------------------------------------------------------------
float mouseX = screenWidth / 2, mouseY = screenHeight / 2;
float lastX = screenWidth / 2, lastY = screenHeight / 2;
float yaw = -90.0f, pitch = 0.0f, fov = 45.0f;
glm::vec3 camPos    = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camUp     = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camFront  = glm::vec3(0.0f, 0.0f, -1.0f);

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
    window = glfwCreateWindow(screenWidth, screenHeight, "Neumann | Graphics Engine", NULL, NULL);
    if(!window) {
        std::cerr << "Failed to create window\n"; glfwTerminate();
        return -1;
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hides cursor
    glfwSetCursorPosCallback(window, mouseCall); // Check position!
    glfwSetScrollCallback(window, scrollCall);

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
        ProcessInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.Use();

            // Deltatime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /// Create transformations -----------------------------------------------------------------------------------------------------------------------
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f) = glm::lookAt(camPos, camPos + camFront, camUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

            // Apply transformations
        unsigned int modelLoc = glGetUniformLocation(shader.m_ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.m_ID, "view");
        unsigned int projLoc  = glGetUniformLocation(shader.m_ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        /// Set lighting ------------------------------------------------------------------------------------------------------------------------------
        glGetUniformLocation(shader.m_ID, "objectColour");
        glGetUniformLocation(shader.m_ID, "lightColour");
        shader.SetVec3("objectColour", 1.0f, 0.5f, 0.31f);

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

/// Functions ========================================================================================================================================
void mouseCall(GLFWwindow* window, double xPos, double yPos) {
    if(firstMouse) {
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
    yaw     += xOffset;
    pitch   += yOffset;

    // Clamp
    if(pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction = glm::vec3(0.0f);
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(direction);
}

void scrollCall(GLFWwindow* window, double xOffset, double yOffset) {
    fov -= static_cast<float>(yOffset);
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 75.0f)
        fov = 75.0f;
}

void ProcessInput(GLFWwindow *window) { // Add diagonal movement
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float camSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camPos += camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camSpeed * camFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
}

// Alter window size
void FrameBufferSize(GLFWwindow *window, short width, short height) {
    glViewport(0, 0, width, height);
}