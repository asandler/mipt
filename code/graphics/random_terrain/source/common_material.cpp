#define _USE_MATH_DEFINES
#include <math.h>

#include <glimg/glimg.h>

#include "common_material.h"

void CommonMaterial::initialize() {
    std::string vertFilename = "shaders/common.vert";
    std::string fragFilename = "shaders/common.frag";

    _programId = makeShaderProgram(vertFilename, fragFilename);

    //=========================================================
    //Инициализация uniform-переменных для преобразования координат
    _modelMatrixUniform = glGetUniformLocation(_programId, "modelMatrix");
    _viewMatrixUniform = glGetUniformLocation(_programId, "viewMatrix");
    _projMatrixUniform = glGetUniformLocation(_programId, "projectionMatrix");
    _normalToCameraMatrixUniform = glGetUniformLocation(_programId, "normalToCameraMatrix");

    //=========================================================
    //Инициализация uniform-переменных для освещения
    _lightPosUniform = glGetUniformLocation(_programId, "lightPos");
    _ambientColorUniform = glGetUniformLocation(_programId, "ambientColor");
    _diffuseColorUniform = glGetUniformLocation(_programId, "diffuseColor");
    _specularColorUniform = glGetUniformLocation(_programId, "specularColor");

    //=========================================================
    //Инициализация uniform-переменных свойств матириалов
    _shininessUniform = glGetUniformLocation(_programId, "shininessFactor");

    //=========================================================
    //Инициализация uniform-переменных для текстурирования
    _diffuseTexUniform1 = glGetUniformLocation(_programId, "diffuseTex1");
    _diffuseTexUniform2 = glGetUniformLocation(_programId, "diffuseTex2");
    _diffuseTexUniform3 = glGetUniformLocation(_programId, "diffuseTex3");
    _diffuseTexUniform4 = glGetUniformLocation(_programId, "diffuseTex4");
    _alphaMap = glGetUniformLocation(_programId, "alphaMap");

    //=========================================================
    //Инициализация прочих uniform-переменных
    _timeUniform = glGetUniformLocation(_programId, "time");
}

void CommonMaterial::applyCommonUniforms() {
    glUniform1f(_timeUniform, _time);
    glUniformMatrix4fv(_viewMatrixUniform, 1, GL_FALSE, glm::value_ptr(_viewMatrix));
    glUniformMatrix4fv(_projMatrixUniform, 1, GL_FALSE, glm::value_ptr(_projMatrix));

    glUniform4fv(_lightPosUniform, 1, glm::value_ptr(_lightPos));
    glUniform3fv(_ambientColorUniform, 1, glm::value_ptr(_ambientColor));
    glUniform3fv(_diffuseColorUniform, 1, glm::value_ptr(_diffuseColor));
    glUniform3fv(_specularColorUniform, 1, glm::value_ptr(_specularColor));
}

void CommonMaterial::applyModelSpecificUniforms() {
    glUniformMatrix4fv(_modelMatrixUniform, 1, GL_FALSE, glm::value_ptr(_modelMatrix));

    glm::mat3 normalToCameraMatrix = glm::transpose(glm::inverse(glm::mat3(_viewMatrix * _modelMatrix)));
    glUniformMatrix3fv(_normalToCameraMatrixUniform, 1, GL_FALSE, glm::value_ptr(normalToCameraMatrix));

    glUniform1f(_shininessUniform, _shininess);

    glUniform1i(_diffuseTexUniform1, _diffuseTexUnit1);
    glUniform1i(_diffuseTexUniform2, _diffuseTexUnit2);
    glUniform1i(_diffuseTexUniform3, _diffuseTexUnit3);
    glUniform1i(_diffuseTexUniform4, _diffuseTexUnit4);
    glUniform1i(_alphaMap, _alphaUnit);
}
