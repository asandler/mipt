#pragma once

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
  Виртуальная камера. Хранит матрицы вида и проекции. Позволяет вращать виртуальной камерой
 */
class Camera {
public:
    Camera();

    glm::mat4 getViewMatrix() {
        return viewMatrix;
    }

    void setViewMatrix(glm::mat4 mat) {
        viewMatrix = mat;
    }

    glm::mat4 getProjMatrix() {
        return projMatrix;
    }
    void setProjMatrix(glm::mat4 mat) {
        projMatrix = mat;
    }

    glm::vec3 getCameraPos() {
        return cameraPos;
    }
    void setCameraPos(glm::vec3 mat) {
        cameraPos = mat;
    }

    void update();

    void setWindowSize(int width, int height);

    //Функции для управления положением камеры
    void rotateLeft(bool b) {
        rotatingLeft = b;
    }
    void rotateRight(bool b) {
        rotatingRight = b;
    }

    void rotateUp(bool b) {
        rotatingUp = b;
    }
    void rotateDown(bool b) {
        rotatingDown = b;
    }

    void zoomUp(bool b) {
        zoomingUp = b;
    }
    void zoomDown(bool b) {
        zoomingDown = b;
    }

    void homePos();

protected:
    glm::mat4 viewMatrix;
    glm::mat4 projMatrix;
    glm::vec3 cameraPos;

    bool rotatingLeft;
    bool rotatingRight;
    float phiAng;

    bool rotatingUp;
    bool rotatingDown;
    float thetaAng;

    bool zoomingUp;
    bool zoomingDown;
    float distance;

    float oldTime;
};
