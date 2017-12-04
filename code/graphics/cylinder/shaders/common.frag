#version 120

uniform sampler2D diffuseTex;

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

//varying vec4 fragColor; //выходной цвет фрагмента

void main() {
//    vec3 diffuseMaterial = texture(diffuseTex, interpTc).rgb; //читаем из текстуры	
//
    
    vec3 diffuseMaterial = vec3(0.4, 0.2, 0.5);
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
    //vec3 color = vec3(r, g, b);

    gl_FragColor = vec4(color, 1.0);
}
