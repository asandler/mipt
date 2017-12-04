#define _USE_MATH_DEFINES
#include <math.h>

#include <glimg/glimg.h>

#include "application.h"
#include "texture.h"

//Функция обратного вызова для обработки нажатий на клавиатуре. Определена в файле navigation.cpp
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

void Application::initContext() {
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3\n";
        exit(1);
    }

    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    _window = glfwCreateWindow(640, 480, "Dodecahedron", NULL, NULL);
    if (!_window) {
        std::cerr << "ERROR: could not open window with GLFW3\n";
        glfwTerminate();
        exit(1);
    }
    glfwMakeContextCurrent(_window);

    glfwSetWindowUserPointer(_window, &_mainCamera); //регистрируем указатель на данный объект, чтобы потом использовать его в функциях обратного вызова
}

void Application::initGL() {
    glewExperimental = GL_TRUE;
    glewInit();

    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION);	// version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void Application::makeScene() {
    makeSceneImplementation();
}

void Application::run() {
    glfwSetKeyCallback(_window, keyCallback);

    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
        update();
        draw();
    }
}

void Application::update() {
    _mainCamera.update();
}

void Application::draw() {
    //Настройки размеров (если пользователь изменил размеры окна)
    int width, height;
    glfwGetFramebufferSize(_window, &width, &height);
    _mainCamera.setWindowSize(width, height);

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    drawScene(_mainCamera);
    glfwSwapBuffers(_window);
}

void Application::makeSceneImplementation() {
    //инициализация шейдеров
    _commonMaterial.initialize();

    //загрузка 3д-моделей
    _terrain = Mesh::makeTerrain(5, 20.0);

    _sandTexId = Texture::loadTexture("images/sand.jpg");

    //Инициализация значений переменных освещения
    _lightPos = glm::vec4(20.0f, 20.0f, 5.f, 1.0f);
    _ambientColor = glm::vec3(0.2, 0.2, 0.2);
    _diffuseColor = glm::vec3(0.8, 0.8, 0.8);
    _specularColor = glm::vec3(0.5, 0.5, 0.5);

    glGenSamplers(1, &_sampler);
    glSamplerParameterf(_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f);
    glSamplerParameteri(_sampler, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glSamplerParameteri(_sampler, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Application::drawScene(Camera& camera) {
    //====== Остальные объекты ======
    glUseProgram(_commonMaterial.getProgramId()); //Подключаем общий шейдер для всех объектов

    _commonMaterial.setTime((float)glfwGetTime());
    _commonMaterial.setViewMatrix(camera.getViewMatrix());
    _commonMaterial.setProjectionMatrix(camera.getProjMatrix());

    _commonMaterial.setLightPos(_lightPos);
    _commonMaterial.setAmbientColor(_ambientColor);
    _commonMaterial.setDiffuseColor(_diffuseColor);
    _commonMaterial.setSpecularColor(_specularColor);

    _commonMaterial.applyCommonUniforms();

    glActiveTexture(GL_TEXTURE0 + 0); //текстурный юнит
    glBindTexture(GL_TEXTURE_2D, _sandTexId);
    glBindSampler(0, _sampler);

    _commonMaterial.setDiffuseTexUnit(1, 0); //текстурный юнит 0
    _commonMaterial.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    _commonMaterial.setShininess(100.0f);
    _commonMaterial.applyModelSpecificUniforms();

    glBindVertexArray(_terrain.getVao()); //Подключаем VertexArray для поверхности
    glDrawArrays(GL_TRIANGLES, 0, _terrain.getNumVertices()); //Рисуем поверхность
}
