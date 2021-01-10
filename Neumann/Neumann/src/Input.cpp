#include "Input.h"
#include "Renderer.h"

using namespace GlobalRend;



void Input::ProccessInput(GLFWwindow *window, Camera &cam) {
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