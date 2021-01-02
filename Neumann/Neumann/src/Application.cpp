#include <iostream>
#include <string> // For 'getline()'
#include <sstream>
#include <fstream>

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

// Prototypes
void FrameBufferSize(GLFWwindow *window, short width, short height);
void ProcessInput(GLFWwindow *window);
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

// Global Settings
const short screenWidth = 800;
const short screenHeight = 600;
float deltaTime = 0.0f; // Delta of current and last frame
float lastFrame = 0.0f; // Time of last frame

/// Create Camera -------------------------------------------------------------------------------------------------------------------------------
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camFront = glm::vec3(0.0f, 0.0f, -1.0f);

/// Shader Parser ======================================================================================================================================
struct ShaderSource {
    std::string vertexSource;
    std::string fragmentSource;
};

static ShaderSource ParseShader(const std::string &filePath) {
    std::ifstream stream(filePath); // Create file streaming object

    enum class ShaderType{NONE = -1, VERTEX, FRAGMENT};

    std::string line;
    std::stringstream ss[2]; // One for vertex, one for fragment
    ShaderType type = ShaderType::NONE; // Default
    while(getline(stream, line)) {
        if(line.find("#shader") != std::string::npos) { // Has found
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if(line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else {
            ss[static_cast<int>(type)] << line << '\n'; // Add shader to string stream
        }
    }
    return {ss[0].str(), ss[1].str()};
}

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

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
        std::cerr << "Error! GLEW not ok\n";
    glEnable(GL_DEPTH_TEST);

    /// Main Body ====================================================================================================================================
        // Vertices
    float vertices[] = { // Only need four due to Index buffer
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

        // Indicies
    unsigned int indicies[] = {
        0, 1, 3, // First triangle
        1, 2, 3
    };

        // Vertex buffer, Index buffer & V-Array object
    unsigned int VAO;
    GLCall(glGenVertexArrays(1, &VAO));
    GLCall(glBindVertexArray(VAO));

    VertexBuffer vb(vertices, sizeof(vertices));
    IndexBuffer ib(indicies, sizeof(indicies));

    vb.Bind();
    ib.Bind();

    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
    GLCall(glEnableVertexAttribArray(0));

        // Shader parse and compile
    ShaderSource source = ParseShader("res/Shaders/Main.shader");
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    GLCall(glUseProgram(shader));

    vb.Unbind();
    GLCall(glBindVertexArray(0)); // Unbind vertex array object

    ///* Loop until the user closes the window */ ==========================================================================================================
    while(!glfwWindowShouldClose(window)) {
        /* Input */
        ProcessInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT);
        GLCall(glUseProgram(shader));

            // Deltatime
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        /// Create transformations -----------------------------------------------------------------------------------------------------------------------
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f) = glm::lookAt(camPos, camPos + camFront, camUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 100.0f);

        model = glm::rotate(model, static_cast<float>(glfwGetTime()) * glm::radians(-55.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

            // Apply transformations
        unsigned int modelLoc = glGetUniformLocation(shader, "model");
        unsigned int viewLoc = glGetUniformLocation(shader, "view");
        unsigned int projLoc = glGetUniformLocation(shader, "projection");
        GLCall(glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)));
        GLCall(glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]));
        GLCall(glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection)));

        /// Draw triangle -----------------------------------------------------------------------------------------------------------------------------
        GLCall(glBindVertexArray(VAO));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
            // GLCall(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /// Deallocate resources =========================================================================================================================
    vb.~VertexBuffer();
    ib.~IndexBuffer();
    GLCall(glDeleteVertexArrays(1, &VAO));
    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}

/// Functions ========================================================================================================================================
void ProcessInput(GLFWwindow *window) { // Add diagonal movement
    const float camSpeed = 2.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camPos += camSpeed * camFront;
    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camPos -= camSpeed * camFront;
    else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camPos -= glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camPos += glm::normalize(glm::cross(camFront, camUp)) * camSpeed;
}

// Alter window size
void FrameBufferSize(GLFWwindow *window, short width, short height) {
    glViewport(0, 0, width, height);
}

static unsigned int CompileShader(unsigned int type, const std::string &shader) {
    unsigned int id = glCreateShader(type);
    const char* src = shader.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // i = integer, v = vector
    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char*)alloca(length * sizeof(char)); // Stack allocate using 'alloca' | Check!
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile:\n" << message << '\n';
        glDeleteShader(id);
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned vShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned fShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    // Attach and link
    glAttachShader(program, vShader);
    glAttachShader(program, fShader);
    glLinkProgram(program);
    glValidateProgram(program);

    // Free shader/s from memory
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}