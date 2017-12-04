#define _USE_MATH_DEFINES
#include <math.h>
#include <glimg/glimg.h>
#include "Mesh.h"
#include <map>

using namespace std;

float frand(float low, float high) {
    return low + (high - low) * (rand() % 1000) * 0.001f;
}

void addVec2(std::vector<float>& vec, float s, float t) {
    vec.push_back(s);
    vec.push_back(t);
}

void addVec3(std::vector<float>& vec, float x, float y, float z) {
    vec.push_back(x);
    vec.push_back(y);
    vec.push_back(z);
}

void addVec4(std::vector<float>& vec, float r, float g, float b, float a) {
    vec.push_back(r);
    vec.push_back(g);
    vec.push_back(b);
    vec.push_back(a);
}

//вычисление цвета по линейной палитре
void getColorFromLinearPalette(float value, float& r, float& g, float& b) {
    if (value < 0.25f) {
        r = 0.0f;
        g = value * 4.0f;
        b = 1.0f;
    } else if (value < 0.5f) {
        r = 0.0f;
        g = 1.0f;
        b = (0.5f - value) * 4.0f;
    } else if (value < 0.75f) {
        r = (value - 0.5f) * 4.0f;
        g = 1.0f;
        b = 0.0f;
    } else {
        r = 1.0f;
        g = (1.0f - value) * 4.0f;
        b = 0.0f;
    }
}


Mesh::Mesh(): _vao(0), _numVertices(0) {}
Mesh::Mesh(GLuint vao, int numVertices): _vao(vao), _numVertices(numVertices) {}


Mesh Mesh::makeSphere(float radius, float F) {
    int N = 100;
    int M = 50;
    float H = 5;
    int numVertices = 0;

    std::vector<float> vertices;	
    std::vector<float> normals;
    std::vector<float> texcoords;

    //Нижняя крышка
    for (int j = 0; j < N; j++) {
        float phi = 2.0f * (float)M_PI * j / N + (float)M_PI;
        float phi1 = 2.0f * (float)M_PI * (j + 1) / N + (float)M_PI;

        addVec3(vertices, 0, 0, 0);
        addVec3(vertices, cos(phi) * radius, sin(phi) * radius, 0);
        addVec3(vertices, cos(phi1) * radius, sin(phi1) * radius, 0);

        addVec3(normals, 0, 0, 1);
        addVec3(normals, 0, 0, 1);
        addVec3(normals, 0, 0, 1);

        numVertices += 3;
    }

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            float phi = 2.0f * (float)M_PI * j / N + (float)M_PI;
            float phi1 = 2.0f * (float)M_PI * (j + 1) / N + (float)M_PI;

            //Первый треугольник, образующий квад

            int k, factor;
            if (i < M / 2) {
                k = i;
                factor = 1;
            } else {
                k = M - i;
                factor = -1;
            }

            float y_offset = F * (k / H) * ((3 * M * M - 4 * k * k) / (H * H));
            float y_offset1 = F * ((k + factor) / H) * ((3 * M * M - 4 * (k + factor) * (k + factor)) / (H * H));

            addVec3(vertices, cos(phi) * radius, sin(phi) * radius + y_offset, i / H);
            addVec3(vertices, cos(phi1) * radius, sin(phi1) * radius + y_offset, i / H);
            addVec3(vertices, cos(phi1) * radius, sin(phi1) * radius + y_offset1, (i + 1) / H);

            addVec3(normals, cos(phi), sin(phi), 0);
            addVec3(normals, cos(phi1), sin(phi1), 0);
            addVec3(normals, cos(phi1), sin(phi1), 0);

            numVertices += 3;

            //Второй треугольник, образующий квад
            addVec3(vertices, cos(phi) * radius, sin(phi) * radius + y_offset, i / H);
            addVec3(vertices, cos(phi1) * radius, sin(phi1) * radius + y_offset1, (i + 1) / H);
            addVec3(vertices, cos(phi) * radius, sin(phi) * radius + y_offset1, (i + 1) / H);			

            addVec3(normals, cos(phi), sin(phi), 0);
            addVec3(normals, cos(phi1), sin(phi1), 0);
            addVec3(normals, cos(phi), sin(phi), 0);

            numVertices += 3;
        }
    }

    //Верхняя крышка
    for (int j = 0; j < N; j++) {
        float phi = 2.0f * (float)M_PI * j / N + (float)M_PI;
        float phi1 = 2.0f * (float)M_PI * (j + 1) / N + (float)M_PI;

        addVec3(vertices, 0, 0, M / H);
        addVec3(vertices, cos(phi) * radius, sin(phi) * radius, M / H);
        addVec3(vertices, cos(phi1) * radius, sin(phi1) * radius, M / H);

        addVec3(normals, 0, 0, -1);
        addVec3(normals, 0, 0, -1);
        addVec3(normals, 0, 0, -1);

        numVertices += 3;
    }

    vertices.insert(vertices.end(), normals.begin(), normals.end());
    vertices.insert(vertices.end(), texcoords.begin(), texcoords.end());

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    GLuint sphereVao = 0;
    glGenVertexArrays(1, &sphereVao);
    glBindVertexArray(sphereVao);
    glEnableVertexAttribArray(0);	
    glEnableVertexAttribArray(1);
    //      glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(numVertices * 3 * 4));
    //	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(numVertices * 3 * 4 * 2));

    glBindVertexArray(0);

    return Mesh(sphereVao, numVertices);
}

class vec3 {
    public:
        vec3() {}
        vec3(const vector<float> _a): x(_a[0]), y(_a[1]), z(_a[2]) {}
        vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
        float x, y, z;

        vector<float> toVector() {
            vector<float> ans;
            ans.push_back(x);
            ans.push_back(y);
            ans.push_back(z);
            return ans;
        }

        float Length() {
            return sqrt(x * x + y * y + z * z);
        }
};

vec3 operator +(const vec3 &a, const vec3 &b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator -(const vec3 &a, const vec3 &b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

bool operator ==(const vec3 &a, const vec3 &b) {
    return a.x == b.x && a.y == b.y &  a.z == b.z;
}

vec3 operator /(const vec3 &a, int d) {
    return vec3(a.x / d, a.y / d, a.z / d);
}

vector<pair<vec3, vec3> > used;
vector<vec3> splits;

vec3 splitEdge(const vec3 &a, const vec3 &b, int depth) {
    for (size_t i = 0; i < used.size(); ++i) {
        if ((used[i].first == a && used[i].second == b) || (used[i].first == b && used[i].second == a)) {
            return splits[i];
        }
    }
    int l = (int)(b - a).Length() * 2;
    vec3 ans = (a + b) / 2 + vec3(\
            0,\
            0,\
            (rand() % (2 * l) - l) / 10.0);
    //                                        (b - a).Length() / 5 * (1.0 * rand() / RAND_MAX - 0.5),\
    //                                        (b - a).Length() / 5 * (1.0 * rand() / RAND_MAX - 0.5),\
    //                                        rand() % 3);
    used.push_back(make_pair(a, b));
    splits.push_back(ans);
    return ans;
}

void recRandomTerrain(int depth, std::vector<float> &v, vec3 &a, vec3 &b, vec3 &c, vec3 &d) {
    if (depth == 5) {
        if (rand() % 2 == 1) {
            addVec3(v, a.x, a.y, a.z);
            addVec3(v, b.x, b.y, b.z);
            addVec3(v, c.x, c.y, c.z);

            addVec3(v, c.x, c.y, c.z);
            addVec3(v, d.x, d.y, d.z);
            addVec3(v, a.x, a.y, a.z);
        } else {
            addVec3(v, a.x, a.y, a.z);
            addVec3(v, b.x, b.y, b.z);
            addVec3(v, d.x, d.y, d.z);

            addVec3(v, b.x, b.y, b.z);
            addVec3(v, c.x, c.y, c.z);
            addVec3(v, d.x, d.y, d.z);
        }
    } else {
        vec3 ab = splitEdge(a, b, depth);
        vec3 bc = splitEdge(b, c, depth);
        vec3 cd = splitEdge(c, d, depth);
        vec3 da = splitEdge(d, a, depth);
        vec3 abcd = splitEdge(ab, cd, depth);

        recRandomTerrain(depth + 1, v, a, ab, abcd, da);
        recRandomTerrain(depth + 1, v, ab, b, bc, abcd);
        recRandomTerrain(depth + 1, v, abcd, bc, c, cd);
        recRandomTerrain(depth + 1, v, cd, d, da, abcd);
    }
}

Mesh Mesh::makeTerrain(int depth) {
    std::vector<float> vertices;	
    std::vector<float> normals;
    std::vector<float> texcoords;

    vec3 a = vec3(-20.0, -20.0, 1.0);
    vec3 b = vec3(-20.0, 20.0, 1.0);
    vec3 c = vec3(20.0, 20.0, 1.0);
    vec3 d = vec3(20.0, -20.0, 1.0);

    srand(time(0));
    recRandomTerrain(0, vertices, a, b, c, d);
    //front 1
    //        float size = 0.5;
    //	addVec3(vertices, size, -size, size);
    //	addVec3(vertices, size, size, size);
    //	addVec3(vertices, size, size, -size);

    //	addVec3(normals, 1.0, 0.0, 0.0);
    //	addVec3(normals, 1.0, 0.0, 0.0);
    //	addVec3(normals, 1.0, 0.0, 0.0);

    //	addVec2(texcoords, 0.0, 1.0);
    //	addVec2(texcoords, 1.0, 1.0);
    //	addVec2(texcoords, 1.0, 0.0);

    //front 2
    //	addVec3(vertices, size, -size, size);
    //	addVec3(vertices, size, size, -size);
    //	addVec3(vertices, size, -size, -size);

    //	addVec3(normals, 1.0, 0.0, 0.0);
    //	addVec3(normals, 1.0, 0.0, 0.0);
    //	addVec3(normals, 1.0, 0.0, 0.0);

    //	addVec2(texcoords, 0.0, 1.0);
    //	addVec2(texcoords, 1.0, 0.0);
    //	addVec2(texcoords, 0.0, 0.0);


    //	vertices.insert(vertices.end(), normals.begin(), normals.end());
    //	vertices.insert(vertices.end(), texcoords.begin(), texcoords.end());

    int numVertices = vertices.size() / 3;

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    GLuint cubeVao = 0;
    glGenVertexArrays(1, &cubeVao);
    glBindVertexArray(cubeVao);
    glEnableVertexAttribArray(0);
    //	glEnableVertexAttribArray(1);
    //	glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    //	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(numVertices * 3 * 4));
    //	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(numVertices * 3 * 4 * 2));

    glBindVertexArray(0);

    return Mesh(cubeVao, numVertices);
}
