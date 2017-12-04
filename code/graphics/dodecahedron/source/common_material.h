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

#include "material.h"

/**
  Типовой шейдер. Применяется для обычных 3д-моделей. Реализует освещение и текстурирование.
  Инкапсулирует работу с шейдерами: создание шейдера, хранение uniform-переменных
 */

class CommonMaterial: public Material {
public:
    CommonMaterial() {
    }

    void initialize();                 // override;
    void applyCommonUniforms();        // override;
    void applyModelSpecificUniforms(); // override;

    void setTime(float time) {
        _time = time;
    }

    void setModelMatrix(glm::mat4 mat) {
        _modelMatrix = mat;
    }
    void setViewMatrix(glm::mat4 mat) {
        _viewMatrix = mat;
    }
    void setProjectionMatrix(glm::mat4 mat) {
        _projMatrix = mat;
    }

    void setLightPos(glm::vec4 vec) {
        _lightPos = vec;
    }
    void setAmbientColor(glm::vec3 vec) {
        _ambientColor = vec;
    }
    void setDiffuseColor(glm::vec3 vec) {
        _diffuseColor = vec;
    }
    void setSpecularColor(glm::vec3 vec) {
        _specularColor = vec;
    }

    void setShininess(float s) {
        _shininess = s;
    }

    void setDiffuseTexUnit(int number, int unit) {
        switch (number) {
            case 1:
                _diffuseTexUnit1 = unit;
                break;
            case 2:
                _diffuseTexUnit2 = unit;
                break;
            case 3:
                _diffuseTexUnit3 = unit;
                break;
            case 4:
                _diffuseTexUnit4 = unit;
                break;
            case 5:
                _alphaUnit = unit;
                break;
        }
    }

protected:
    //====== идентификаторы uniform-переменных ======
    GLuint _timeUniform;
    GLuint _modelMatrixUniform;
    GLuint _viewMatrixUniform;
    GLuint _projMatrixUniform;
    GLuint _normalToCameraMatrixUniform;

    GLuint _lightPosUniform;
    GLuint _ambientColorUniform;
    GLuint _diffuseColorUniform;
    GLuint _specularColorUniform;

    GLuint _shininessUniform;

    GLuint _diffuseTexUniform1;
    GLuint _diffuseTexUniform2;
    GLuint _diffuseTexUniform3;
    GLuint _diffuseTexUniform4;
    GLuint _alphaMap;

    //====== переменные, которые содержат значения, которые будут записаны в uniform-переменные шейдеров ======
    float _time;

    //матрицы
    glm::mat4 _modelMatrix;
    glm::mat4 _viewMatrix;
    glm::mat4 _projMatrix;

    //параметры освещения
    glm::vec4 _lightPos; //in world space
    glm::vec3 _ambientColor;
    glm::vec3 _diffuseColor;
    glm::vec3 _specularColor;

    //параметры материалов
    float _shininess;

    //текстурные юниты
    int _diffuseTexUnit1;
    int _diffuseTexUnit2;
    int _diffuseTexUnit3;
    int _diffuseTexUnit4;
    int _alphaUnit;
};
