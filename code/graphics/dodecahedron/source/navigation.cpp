#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"

//Callback для обработки нажатий на клавиатуре
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Camera* camera = (Camera*)glfwGetWindowUserPointer(window);

    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;

            case GLFW_KEY_A:
                camera->rotateLeft(true);
                break;

            case GLFW_KEY_D:
                camera->rotateRight(true);
                break;

            case GLFW_KEY_W:
                camera->rotateUp(true);
                break;

            case GLFW_KEY_S:
                camera->rotateDown(true);
                break;

            case GLFW_KEY_R:
                camera->zoomUp(true);
                break;

            case GLFW_KEY_F:
                camera->zoomDown(true);
                break;

            case GLFW_KEY_SPACE:
                camera->homePos();
                break;
        }
    } else if (action == GLFW_RELEASE) {
        switch (key) {
            case GLFW_KEY_A:
                camera->rotateLeft(false);
                break;

            case GLFW_KEY_D:
                camera->rotateRight(false);
                break;

            case GLFW_KEY_W:
                camera->rotateUp(false);
                break;

            case GLFW_KEY_S:
                camera->rotateDown(false);
                break;

            case GLFW_KEY_R:
                camera->zoomUp(false);
                break;

            case GLFW_KEY_F:
                camera->zoomDown(false);
                break;
        }
    }
}
