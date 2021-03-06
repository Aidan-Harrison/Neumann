#include <iostream>
// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// GLM
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
// Assimp
// Custom
#include "Renderer.h"
#include "..\src\VIVa\VertexBuffer.h"
#include "..\src\VIVa\IndexBuffer.h"
#include "..\src\VIVa\VertexArray.h"
#include "..\src\Shader\ShaderHandler.h"
#include "..\src\Camera\Camera.h"
#include "..\src\Input\Input.h"
#include "..\src\Shader\TextureHandler.h"

// Prototypes
void FrameBufferSize(GLFWwindow *window, short width, short height);
void ScrollCall(GLFWwindow* window, double xOffset, double yOffset);
void MouseCall(GLFWwindow* window, double xPos, double yPos);
void LightMatSetup(Shader &shader, glm::vec3 lightPos, Camera &cam);
void SetPointLight(Shader& shader, glm::vec3 lightPos[]);

using namespace GlobalRend;
Camera cam;
Input input;

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
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    /// Main Body ====================================================================================================================================
        // Vertices
    float vertices[] = {
        // Positions             Normals                Texture Coordinates
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 0.0f, // bottom-right
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f, // top-right   
         0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    1.0f, 1.0f, // top-right 
        -0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 1.0f, // top-left 
        -0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,    0.0f, 0.0f, // bottom-left 
        // front face                                  
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 1.0f, // top-right  
         0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 0.0f, // bottom-right
         0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     1.0f, 1.0f, // top-right 
        -0.5f, -0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 0.0f, // bottom-left  
        -0.5f,  0.5f,  0.5f,     0.0f,  0.0f, 1.0f,     0.0f, 1.0f, // top-left    
        // left face                                  
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f, 0.0f,     1.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f, 0.0f,     0.0f, 1.0f, // bottom-left  
        -0.5f,  0.5f, -0.5f,    -1.0f,  0.0f, 0.0f,     1.0f, 1.0f, // top-left    
        -0.5f, -0.5f, -0.5f,    -1.0f,  0.0f, 0.0f,     0.0f, 1.0f, // bottom-left 
        -0.5f,  0.5f,  0.5f,    -1.0f,  0.0f, 0.0f,     1.0f, 0.0f, // top-right  
        -0.5f, -0.5f,  0.5f,    -1.0f,  0.0f, 0.0f,     0.0f, 0.0f, // bottom-right
        // right face           
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f, 0.0f,     1.0f, 0.0f, // top-left
         0.5f,  0.5f, -0.5f,     1.0f,  0.0f, 0.0f,     1.0f, 1.0f, // top-right   
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f, 0.0f,     0.0f, 1.0f, // bottom-right
         0.5f, -0.5f, -0.5f,     1.0f,  0.0f, 0.0f,     0.0f, 1.0f, // bottom-right
         0.5f, -0.5f,  0.5f,     1.0f,  0.0f, 0.0f,     0.0f, 0.0f, // bottom-left 
         0.5f,  0.5f,  0.5f,     1.0f,  0.0f, 0.0f,     1.0f, 0.0f, // top-left 
         // bottom face           
        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f, 0.0f,     0.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f, 0.0f,     1.0f, 0.0f, // bottom-left  
         0.5f, -0.5f, -0.5f,     0.0f, -1.0f, 0.0f,     1.0f, 1.0f, // top-left    
         0.5f, -0.5f,  0.5f,     0.0f, -1.0f, 0.0f,     1.0f, 0.0f, // bottom-left 
        -0.5f, -0.5f, -0.5f,     0.0f, -1.0f, 0.0f,     0.0f, 1.0f, // top-right  
        -0.5f, -0.5f,  0.5f,     0.0f, -1.0f, 0.0f,     0.0f, 0.0f, // bottom-right
        // top face                                     
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f, 0.0f,     0.0f, 1.0f, // top-left
         0.5f,  0.5f, -0.5f,     0.0f,  1.0f, 0.0f,     1.0f, 1.0f, // top-right  
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f, 0.0f,     1.0f, 0.0f, // bottom-right 
         0.5f,  0.5f,  0.5f,     0.0f,  1.0f, 0.0f,     1.0f, 0.0f, // bottom-right 
        -0.5f,  0.5f,  0.5f,     0.0f,  1.0f, 0.0f,     0.0f, 0.0f, // bottom-left 
        -0.5f,  0.5f, -0.5f,     0.0f,  1.0f, 0.0f,     0.0f, 1.0f  // top-left
    };

    glm::vec3 cubePos[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, -2.0f)
    };

        // Vertex buffer, Index buffer & V-Array object
    VertexArray va; // Auto generates with constructor
    unsigned int texture = 1; // Abstract further like VertexArray
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.Push<float>(2); // Check if correct size?
    vb.Bind();
    unsigned int diffuseMap = LoadTexture("res/Textures/PinkSummer.jpg", texture);
    unsigned int specularMap = LoadTexture("res/Textures/PinkSummer.jpg", texture);
    va.AddBuffer(vb, layout);
    Shader shader("res/Shaders/shader.vs", "res/Shaders/shader.fs");

    vb.Unbind();

    ///* Loop until the user closes the window */ ==========================================================================================================
    while(!glfwWindowShouldClose(window)) {
        /* Input */
        input.ProccessInput(window, cam);

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
        glm::vec3 lightPos = glm::vec3(-3.0f, 0.0f, 0.0f);
        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        cam.Transformation(cam.m_View, cam.m_Projection);
        cam.Translate(cam.m_View);

            // Apply transformations | Convert to shader handler functions
        unsigned int modelLoc = glGetUniformLocation(shader.m_ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.m_ID, "view");
        unsigned int projLoc  = glGetUniformLocation(shader.m_ID, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &cam.m_View[0][0]);
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(cam.m_Projection));

        /// Lighting & Materials -------------------------------------------------------------------------------------------------------------------
            // Point light positions
        glm::vec3 pointLightPositions[] = {
            glm::vec3(1.0f, 0.2f, 2.0f),
            glm::vec3(-3.0f, -3.0f, -4.0f),
            glm::vec3(4.0f, 2.0f, -6.0f),
            glm::vec3(2.0f, 0.5f, -3.0f),
        };
        // SetPointLight(shader, pointLightPositions);
        LightMatSetup(shader, lightPos, cam);
        glActiveTexture(GL_TEXTURE0);
        glActiveTexture(GL_TEXTURE1);

        /// Draw cubes -----------------------------------------------------------------------------------------------------------------------------
        BindTexture(diffuseMap);
        BindTexture(specularMap);
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

    cam.CalculateMovement(cam, xOffset, yOffset);
}

void ScrollCall(GLFWwindow* window, double xOffset, double yOffset) {
    cam.FOV -= static_cast<float>(yOffset);
    if (cam.FOV < 1.0f)
        cam.FOV = 1.0f;
    if (cam.FOV > 75.0f)
        cam.FOV = 75.0f;
}