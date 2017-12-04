#version 120

//стандартные матрицы для преобразования координат
uniform mat4 modelMatrix; //из локальной в мировую
uniform mat4 viewMatrix; //из мировой в систему координат камеры
uniform mat4 projectionMatrix; //из системы координат камеры в усеченные координаты

//матрица для преобразования нормалей из локальной системы координат в систему координат камеры
uniform mat3 normalToCameraMatrix;

uniform vec4 lightPos; //положение источника света в мировой системе координат (для точечного источника)

attribute vec3 vp; //координаты вершины в локальной системе координат
attribute vec3 normal; //нормаль в локальной системе координат
attribute vec2 tc; //текстурные координаты в локальной системе координат

varying vec3 normalCamSpace; //нормаль в системе координат камеры
varying vec4 lightPosCamSpace; //положение источника света в системе координат камеры
varying vec4 posCamSpace; //координаты вершины в системе координат камеры

varying vec2 interpTc; //выходные текстурные координаты

varying float r;
varying float g;
varying float b;

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    posCamSpace = viewMatrix * modelMatrix * vec4(vp, 1.0); //преобразование координат вершины в систему координат камеры
    gl_Position = projectionMatrix * posCamSpace;

    normalCamSpace = normalize(normalToCameraMatrix * normal); //преобразование нормали в систему координат камеры
    lightPosCamSpace = viewMatrix * lightPos; //преобразование положения источника света в систему координат камеры

    b = 0.4;
    g = 0.1;
    r = vp.z * 0.1;

    interpTc = tc; //просто копируем
}
