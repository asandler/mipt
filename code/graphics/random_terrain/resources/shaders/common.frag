#version 120

uniform sampler2D diffuseTex1;
uniform sampler2D diffuseTex2;
uniform sampler2D diffuseTex3;
uniform sampler2D diffuseTex4;
uniform sampler2D alphaMap;

uniform vec3 ambientColor; //цвет окружающего света (аппроксимация множественных переотражений)
uniform vec3 diffuseColor; //цвет источника света
uniform vec3 specularColor; //бликовый цвет источника света
uniform float shininessFactor; //блеск (свойство материала, влияет на размер блика)

varying vec3 normalCamSpace; //нормаль в системе координат камеры (интерполирована между вершинами треугольника)
varying vec4 lightPosCamSpace; //положение источника света в системе координат камеры (интерполировано между вершинами треугольника)
varying vec4 posCamSpace; //координаты вершины в системе координат камеры (интерполированы между вершинами треугольника)

varying vec2 interpTc;

varying float r;
varying float g;
varying float b;

void main() {
    vec3 diffuseMaterial1 = texture2D(diffuseTex1, interpTc).rgb; //читаем из текстуры	
    vec3 diffuseMaterial2 = texture2D(diffuseTex2, interpTc).rgb; //читаем из текстуры
    vec3 diffuseMaterial3 = texture2D(diffuseTex3, interpTc).rgb; //читаем из текстуры	
    vec3 diffuseMaterial4 = texture2D(diffuseTex4, interpTc).rgb; //читаем из текстуры
    vec4 map = texture2D(alphaMap, interpTc).rgba / 4; //читаем карту

    vec3 diffuseMaterial = map.r * diffuseMaterial1 + map.g * diffuseMaterial2 + map.b * diffuseMaterial3 + map.a * diffuseMaterial4;
    
    vec3 lightDirCamSpace = lightPosCamSpace.xyz - posCamSpace.xyz; //направление на источник света
    lightDirCamSpace = normalize(lightDirCamSpace); //нормализуем направление

    vec3 normal = normalize(normalCamSpace); //нормализуем нормаль после интерполяции
                                
    float cosAngIncidence = dot(normal, lightDirCamSpace); //интенсивность диффузного света
    cosAngIncidence = clamp(cosAngIncidence, 0, 1);

    vec3 viewDirection = normalize(-posCamSpace.xyz); //направление на виртуальную камеру (она находится в точке (0.0, 0.0, 0.0))
            
    vec3 halfAngle = normalize(lightDirCamSpace + viewDirection); //биссектриса между направлениями на камеру и на источник света
    float blinnTerm = dot(normal, halfAngle); //интенсивность бликового освещения по Блинну
    blinnTerm = clamp(blinnTerm, 0, 1);
    blinnTerm = cosAngIncidence != 0.0 ? blinnTerm : 0.0;
    blinnTerm = pow(blinnTerm, shininessFactor);  //регулируем размер блика

    //результирующий цвет
    vec3 color = diffuseMaterial * ambientColor + diffuseMaterial * diffuseColor * cosAngIncidence + specularColor * blinnTerm;

    gl_FragColor = vec4(color, 1.0);
}
