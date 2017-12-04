#include <math.h>
#include <glimg/glimg.h>
#include <map>

#include "mesh.h"

using namespace std;

class vec3 {
    public:
        vec3() {}
        vec3(const vector<float> _a)
            : x(_a[0])
              , y(_a[1])
              , z(_a[2])
        {}
        vec3(float _x, float _y, float _z)
            : x(_x)
              , y(_y)
              , z(_z)
        {}
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

vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

bool operator==(const vec3& a, const vec3& b) {
    return a.x == b.x && a.y == b.y & a.z == b.z;
}

vec3 operator/(const vec3& a, int d) {
    return vec3(a.x / d, a.y / d, a.z / d);
}

vec3 operator^(const vec3& a, const vec3& b) {
    return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

float frand(float low, float high) {
    return low + (high - low) * (rand() % 1000) * 0.001f;
}

void addVec2(vector<float>& vec, vec3 v) {
    vec.push_back(v.x);
    vec.push_back(v.y);
}

void addVec3(vector<float>& vec, vec3 v) {
    vec.push_back(v.x);
    vec.push_back(v.y);
    vec.push_back(v.z);
}

void addVec4(vector<float>& vec, float r, float g, float b, float a) {
    vec.push_back(r);
    vec.push_back(g);
    vec.push_back(b);
    vec.push_back(a);
}

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

vector<pair<vec3, vec3> > used;
vector<vec3> splits;
map<pair<float, float>, vector<vec3> > normalsMean;
map<pair<float, float>, vec3> normalsMeanValues;

void dodecahedronVertices(vector<float>& v, vector<float>& t, float radius) {
    float phi = 1.61803;
    float rev_phi = 0.61803;

    vector<vector<float> > vertices = {
        {-1, -1, -1},
        {-1, -1,  1},
        {-1,  1, -1},
        {-1,  1,  1},

        { 1, -1, -1},
        { 1, -1,  1},
        { 1,  1, -1},
        { 1,  1,  1},

        {0, -rev_phi, -phi},
        {0,  rev_phi, -phi},
        {0, -rev_phi,  phi},
        {0,  rev_phi,  phi},

        {-rev_phi, -phi, 0},
        { rev_phi, -phi, 0},
        {-rev_phi,  phi, 0},
        { rev_phi,  phi, 0},

        {-phi, 0, -rev_phi},
        { phi, 0, -rev_phi},
        {-phi, 0,  rev_phi},
        { phi, 0,  rev_phi}
    };

    vector<vector<float> > edges = {
        {1, 17, 3}, {1, 3, 9}, {3, 10, 9},

        {10, 9, 7}, {7, 9, 5}, {5, 7, 18},

        {1, 9, 5}, {1, 5, 14}, {1, 13, 14},

        {5, 14, 6}, {5, 18, 6}, {18, 6, 20},

        {3, 10, 15}, {10, 15, 7}, {15, 7, 16},

        {7, 16, 8}, {7, 8, 18}, {18, 8, 20},

        {17, 19, 2}, {17, 1, 2}, {1, 2, 13},

        {13, 2, 11}, {6, 11, 13}, {13, 14, 6},

        {11, 6, 20}, {11, 20, 12}, {12, 8, 20},

        {19, 2, 11}, {19, 11, 4}, {4, 12, 11},

        {17, 19, 4}, {17, 4, 3}, {3, 4, 15},

        {4, 12, 15}, {12, 15, 8}, {15, 16, 8},
    };

    for (size_t i = 0; i < edges.size(); ++i) {
        vec3 a = vec3(vertices[edges[i][0] - 1]);
        vec3 b = vec3(vertices[edges[i][1] - 1]);
        vec3 c = vec3(vertices[edges[i][2] - 1]);

        addVec3(v, a);
        addVec3(v, b);
        addVec3(v, c);

        normalsMean[make_pair(a.x, a.y)].push_back((c - a) ^ (b - a));
        normalsMean[make_pair(b.x, b.y)].push_back((a - b) ^ (c - b));
        normalsMean[make_pair(c.x, c.y)].push_back((b - c) ^ (a - c));

        vec3 tca = vec3(acos(a.z / a.Length()), atan(a.y / a.x), 0);
        vec3 tcb = vec3(acos(b.z / b.Length()), atan(b.y / b.x), 0);
        vec3 tcc = vec3(acos(c.z / c.Length()), atan(c.y / c.x), 0);

        addVec2(t, tca);
        addVec2(t, tcb);
        addVec2(t, tcc);
    }
}

Mesh Mesh::makeTerrain(int depth, float size) {
    vector<float> vertices;
    vector<float> normals;
    vector<float> texcoords;

    dodecahedronVertices(vertices, texcoords, 1);

    int numVertices = vertices.size() / 3;

    /*
       use normalsMean to create normals (calculate mean values first)
     */

    for (map<pair<float, float>, vector<vec3> >::iterator it = normalsMean.begin(); it != normalsMean.end(); ++it) {
        vec3 m = vec3(0, 0, 0);
        for (vector<vec3>::iterator jt = it->second.begin(); jt != it->second.end(); ++jt) {
            m = m + *jt;
        }
        normalsMeanValues[it->first] = m;
    }

    for (size_t i = 0; i < vertices.size(); i += 3) {
        vec3 v = normalsMeanValues[make_pair(vertices[i], vertices[i + 1])];
        addVec3(normals, v);
    }

    int offset = vertices.end() - vertices.begin();
    vertices.insert(vertices.end(), normals.begin(), normals.end());
    int offset2 = vertices.end() - vertices.begin();
    vertices.insert(vertices.end(), texcoords.begin(), texcoords.end());

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    GLuint terrainVao = 0;
    glGenVertexArrays(1, &terrainVao);
    glBindVertexArray(terrainVao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(offset * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)(offset2 * sizeof(float)));

    glBindVertexArray(0);

    return Mesh(terrainVao, numVertices);
}
