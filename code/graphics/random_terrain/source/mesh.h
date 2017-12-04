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
  Содержит VAO для меша (полигональной 3д-модели) и количество вершин.
  Также имеется несколько статических методов для создания конкретных мешей.
 */
class Mesh {
public:
    Mesh()
        : _vao(0)
          , _numVertices(0) 
    {}

    Mesh(GLuint vao, int numVertices)
        : _vao(vao)
          , _numVertices(numVertices) 
    {}

    GLuint getVao() {
        return _vao;
    }
    int getNumVertices() {
        return _numVertices;
    }

    static Mesh makeTerrain(int depth, float size);

protected:
    GLuint _vao;
    int _numVertices;
};
