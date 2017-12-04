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
        : vao(0)
          , numVertices(0) 
    {}

    Mesh(GLuint vao, int numVertices)
        : vao(vao)
          , numVertices(numVertices) 
    {}

    GLuint getVao() {
        return vao;
    }
    int getNumVertices() {
        return numVertices;
    }

    static Mesh makeTerrain(int depth, float size);

protected:
    GLuint vao;
    int numVertices;
};
