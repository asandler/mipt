#define _USE_MATH_DEFINES
#include <math.h>

#include <glimg/glimg.h>

#include "Application.h"

//Функция обратного вызова для обработки нажатий на клавиатуре. Определена в файле Navigation.cpp
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

Application::Application(): _oldTime(0.0f), _num(0), _forvard(true) {}

Application::~Application() {
    glfwTerminate();
}

void Application::initContext() {
    if (!glfwInit()) {
        std::cerr << "ERROR: could not start GLFW3\n";		
        exit(1);
    } 

    glfwWindowHint(GLFW_STENCIL_BITS, 8);

    _window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);
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
    glewInit ();

    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    //	glEnable(GL_POLYGON_OFFSET_FILL);	
}

void Application::makeScene() {
    makeSceneImplementation(0);
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

void Application::makeSceneImplementation(float F) {
    //инициализация шейдеров
    _commonMaterial.initialize();

    //загрузка 3д-моделей
    _terrain = Mesh::makeSphere(2, F);

    //Инициализация значений переменных освщения
    _lightPos = glm::vec4(20.0f, 20.0f, 5.f, 1.0f);
    _ambientColor = glm::vec3(0.2, 0.2, 0.2);
    _diffuseColor = glm::vec3(0.8, 0.8, 0.8);
    _specularColor = glm::vec3(0.5, 0.5, 0.5);
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

    //	_commonMaterial.setDiffuseTexUnit(0); //текстурный юнит 0
    _commonMaterial.setModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
    _commonMaterial.setShininess(100.0f);
    _commonMaterial.applyModelSpecificUniforms();

    if (_forvard)
        _terrain = Mesh::makeSphere(2, _num++ * 0.00004);
    else
        _terrain = Mesh::makeSphere(2, _num-- * 0.00004);
    if (_num == 0)
        _forvard = true;
    if (_num == 30)
        _forvard = false;
    glBindVertexArray(_terrain.getVao()); //Подключаем VertexArray для сферы
    glDrawArrays(GL_TRIANGLES, 0, _terrain.getNumVertices()); //Рисуем сферу
}
