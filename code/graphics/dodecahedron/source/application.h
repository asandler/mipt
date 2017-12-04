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

#include "common_material.h"
#include "camera.h"
#include "mesh.h"

class Application {
public:
    Application()
        : _oldTime(0.0f)
    {
    }

    ~Application() {
        glfwTerminate();
    }

    void initContext(); //Инициализация графического контекста

    void initGL(); //Настройка некоторых параметров OpenGL

    void makeScene(); //Создание трехмерной сцены

    void run(); //Цикл рендеринга

    void draw(); //Отрисовать один кадр

    void update(); //Обновление

protected:
    GLFWwindow* _window;
    CommonMaterial _commonMaterial;
    Camera _mainCamera;

    //параметры освещения
    glm::vec4 _lightPos; //in world space
    glm::vec3 _ambientColor;
    glm::vec3 _diffuseColor;
    glm::vec3 _specularColor;

    GLuint _sandTexId;
    GLuint _sampler;

    //полигональные 3д-модели
    Mesh _terrain;

    float _oldTime;

    void makeSceneImplementation();
    void drawScene(Camera& camera);
};
