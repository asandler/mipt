#define USEMATHDEFINES
#include <math.h>

#include <glimg/glimg.h>

#include "camera.h"

Camera::Camera()
    : rotatingLeft(false)
    , rotatingRight(false)
    , rotatingUp(false)
      , rotatingDown(false)
      ,

    zoomingUp(false)
, zoomingDown(false)
    ,

    phiAng(0.0f)
    , thetaAng(0.0f)
, distance(50.0f)
    ,

    oldTime(0.0) {
        viewMatrix = glm::lookAt(glm::vec3(0.0f, -5.0f, 0.0f), glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        projMatrix = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
    }

void Camera::setWindowSize(int width, int height) {
    projMatrix = glm::perspective(45.0f, (float)width / height, 0.1f, 100.f);
}

void Camera::update() {
    float dt = glfwGetTime() - oldTime;
    oldTime = glfwGetTime();

    float speed = 1.0f;

    if (rotatingLeft) {
        phiAng -= speed * dt;
    }
    if (rotatingRight) {
        phiAng += speed * dt;
    }
    if (rotatingUp) {
        thetaAng += speed * dt;
    }
    if (rotatingDown) {
        thetaAng -= speed * dt;
    }
    if (zoomingUp) {
        distance += distance * dt;
    }
    if (zoomingDown) {
        distance -= distance * dt;
    }

    thetaAng = glm::clamp(thetaAng, -(float)M_PI * 0.45f, (float)M_PI * 0.45f);
    distance = glm::clamp(distance, 0.5f, 500.0f);

    cameraPos = glm::vec3(glm::cos(phiAng) * glm::cos(thetaAng), glm::sin(phiAng) * glm::cos(thetaAng), glm::sin(thetaAng)) * distance;
    viewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Camera::homePos() {
    phiAng = 0.0;
    thetaAng = M_PI * 0.05;
    distance = 20.0;

    cameraPos = glm::vec3(glm::cos(phiAng) * glm::cos(thetaAng), glm::sin(phiAng) * glm::cos(thetaAng), glm::sin(thetaAng)) * distance;
    viewMatrix = glm::lookAt(cameraPos, glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
}
