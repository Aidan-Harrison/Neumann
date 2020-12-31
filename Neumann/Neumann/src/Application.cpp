#include <iostream>
#include <string> // For 'getline()'
#include <sstream>
#include <fstream>

// OpenGL includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// Prototypes
void FrameBufferSize(GLFWwindow *window, short width, short height);
void ProcessInput(GLFWwindow *window);
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

// Global Settings
const short screenWidth = 800;
const short screenHeight = 600;

// Shader Parser
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
            ss[(int)type] << line << '\n'; // Add shader to string stream
        }
    }
    return {ss[0].str(), ss[1].str()};
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit())
        return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(screenWidth, screenHeight, "Neumann", NULL, NULL);
    if(!window) {
        std::cerr << "Failed to create window\n"; glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
        std::cerr << "Error! GLEW not ok\n";

    /// Main Body

    // Vertices
    float vertices[] = { // Only need four due to Index buffer
         0.5f,  0.5f, 0.0f, // TRight
         0.5f, -0.5f, 0.0f, // BRight
        -0.5f, -0.5f, 0.0f, // BLeft
        -0.5f,  0.5f, 0.0f  // TLeft
    };

    // Indicies
    unsigned int indicies[] = {
        0, 1, 3, // First triangle
        1, 2, 3
    };

    // Vertex buffer & array
    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);
    glBindVertexArray(VAO);

    // Vertex Buffer Data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Index buffer data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); 
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Shader comp
    ShaderSource source = ParseShader("res/Shaders/Main.shader");
    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind vertex buffer
    glBindVertexArray(0); // Unbind vertex array object

    ///* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)) {
        /* Input */
        ProcessInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw triangle
        glUseProgram(shader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /// Deallocate resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

void ProcessInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
        char *message = (char*)alloca(length * sizeof(char)); // Stack allocate using 'alloca'
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

    // Free shader from memory
    glDeleteShader(vShader);
    glDeleteShader(fShader);

    return program;
}