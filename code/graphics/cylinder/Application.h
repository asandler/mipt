#pragma once

#define GLM_FORCE_RADIANS

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "CommonMaterial.h"
#include "SkyBoxMaterial.h"
#include "Camera.h"
#include "Mesh.h"

class Application {
public:
    Application();
    ~Application();

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

    //полигональные 3д-модели
    Mesh _terrain;

    float _oldTime;
    int _num;
    bool _forvard;

    void makeSceneImplementation(float);
    void drawScene(Camera& camera);
    void drawBackground(Camera& camera);
};
