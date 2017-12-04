#include <math.h>
#include <glimg/glimg.h>
#include <map>

#include "mesh.h"

using namespace std;

class vec3 {
public:
    vec3() {
    }
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

vec3 splitEdge(const vec3& a, const vec3& b, int depth) {
    for (size_t i = 0; i < used.size(); ++i) {
        if ((used[i].first == a && used[i].second == b) || (used[i].first == b && used[i].second == a)) {
            return splits[i];
        }
    }

    int l = (int)((b - a).Length() * 20);
    vec3 ans = (a + b) / 2 + vec3(
            0,
            0,
            (rand() % (2 * l) - l) / 100.0);

    used.push_back(make_pair(a, b));
    splits.push_back(ans);
    return ans;
}

void recRandomTerrain(int depth, vector<float>& v, /*vector<float> &n,*/ vector<float>& t, vec3& a, vec3& b, vec3& c, vec3& d, float size) {
    if (depth == 0) {
        if (rand() % 2 == 1) {
            addVec3(v, a);
            addVec3(v, b);
            addVec3(v, c);

            /*
               addVec3(n, (c - a) ^ (b - a));
               addVec3(n, (a - b) ^ (c - b));
               addVec3(n, (b - c) ^ (a - c));
             */
            normalsMean[make_pair(a.x, a.y)].push_back((c - a) ^ (b - a));
            normalsMean[make_pair(b.x, b.y)].push_back((a - b) ^ (c - b));
            normalsMean[make_pair(c.x, c.y)].push_back((b - c) ^ (a - c));

            addVec2(t, (a + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (b + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (c + vec3(size, size, 0)) / (size * 2));

            addVec3(v, c);
            addVec3(v, d);
            addVec3(v, a);

            /*
               addVec3(n, (a - c) ^ (d - c));
               addVec3(n, (c - d) ^ (a - d));
               addVec3(n, (d - a) ^ (c - a));
             */

            normalsMean[make_pair(c.x, c.y)].push_back((a - c) ^ (d - c));
            normalsMean[make_pair(d.x, d.y)].push_back((c - d) ^ (a - d));
            normalsMean[make_pair(a.x, a.y)].push_back((d - a) ^ (c - a));

            addVec2(t, (c + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (d + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (a + vec3(size, size, 0)) / (size * 2));

        } else {
            addVec3(v, a);
            addVec3(v, b);
            addVec3(v, d);

            /*
               addVec3(n, (d - a) ^ (b - a));
               addVec3(n, (a - b) ^ (d - b));
               addVec3(n, (b - d) ^ (a - d));
             */

            normalsMean[make_pair(a.x, a.y)].push_back((d - a) ^ (b - a));
            normalsMean[make_pair(b.x, b.y)].push_back((a - b) ^ (d - b));
            normalsMean[make_pair(d.x, d.y)].push_back((b - d) ^ (a - d));

            addVec2(t, (a + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (b + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (d + vec3(size, size, 0)) / (size * 2));

            addVec3(v, b);
            addVec3(v, c);
            addVec3(v, d);

            /*
               addVec3(n, (d - b) ^ (c - b));
               addVec3(n, (b - c) ^ (d - c));
               addVec3(n, (c - d) ^ (b - d));
             */

            normalsMean[make_pair(b.x, b.y)].push_back((d - b) ^ (c - b));
            normalsMean[make_pair(c.x, c.y)].push_back((b - c) ^ (d - c));
            normalsMean[make_pair(d.x, d.y)].push_back((c - d) ^ (b - d));

            addVec2(t, (b + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (c + vec3(size, size, 0)) / (size * 2));
            addVec2(t, (d + vec3(size, size, 0)) / (size * 2));
        }

    } else {
        vec3 ab = splitEdge(a, b, depth);
        vec3 bc = splitEdge(b, c, depth);
        vec3 cd = splitEdge(c, d, depth);
        vec3 da = splitEdge(d, a, depth);
        vec3 abcd = splitEdge(ab, cd, depth);

        recRandomTerrain(depth - 1, v, /*n,*/ t, a, ab, abcd, da, size);
        recRandomTerrain(depth - 1, v, /*n,*/ t, ab, b, bc, abcd, size);
        recRandomTerrain(depth - 1, v, /*n,*/ t, abcd, bc, c, cd, size);
        recRandomTerrain(depth - 1, v, /*n,*/ t, cd, d, da, abcd, size);
    }
}

Mesh Mesh::makeTerrain(int depth, float size) {
    vector<float> vertices;
    vector<float> normals;
    vector<float> texcoords;

    vec3 a = vec3(-size, -size, 10.0);
    vec3 b = vec3(-size, size, 10.0);
    vec3 c = vec3(size, size, 10.0);
    vec3 d = vec3(size, -size, 10.0);

    srand(time(0));
    recRandomTerrain(depth, vertices, /*normals,*/ texcoords, a, b, c, d, size);

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
