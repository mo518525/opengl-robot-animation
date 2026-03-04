#include "RobotAnimationScene.h"
#include <AssetManager.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CubeMeshWithNormals.h"

using namespace glm;
using namespace std;


RobotAnimationScene::RobotAnimationScene(OpenGLWindow *window) :
        m_window(window),
        nearPlane(0.1f),
        farPlane(100.0f),
        fov(45.0f),
        aspectRatio((float)window->getWindowWidth() / (float)window->getWindowHeight()) {
    assert(window != nullptr);
}

RobotAnimationScene::~RobotAnimationScene() {}

bool RobotAnimationScene::init() {
    try {
        //Load shader
        m_assets.addShaderProgram("shader", AssetManager::createShaderProgram("assets/shaders/vertex.glsl",
                                                                              "assets/shaders/fragment.glsl"));
        m_shader = m_assets.getShaderProgram("shader");
        m_shader->use();



        // Initialize camera
        cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

        // Set initial camera parameters
        setNearPlane(0.1f);
        setFarPlane(100.0f);
        setFOV(45.0f);
        setAspectRatio((float)m_window->getWindowWidth() / (float)m_window->getWindowHeight());

        // Set the ViewMatrix uniform
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        m_shader->setUniform("View", view, false);

        // Set the ProjectionMatrix uniform
        updateProjectionMatrix();

        float vertices[] = {-0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, -0.5, 0.0, 0.0, 1.0,
                            0.5, 0.5, 0.0, 1.0, 0.0,
                            0.0, 1.0, 1.0, 0.0, 0.0,
                            -0.5, 0.5, 0.0, 1.0, 0.0};

        int indices[] = {0, 1, 2,
                         0, 2, 4,
                         4, 2, 3};


        static const float cubeVert[] = {0.5, -0.5, -0.5, 1, 0, 0,
                                         0.5, -0.5, 0.5, 0, 1, 0,
                                         -0.5, -0.5, 0.5, 0, 0, 1,
                                         -0.5, -0.5, -0.5, 1, 1, 0,
                                         0.5, 0.5, -0.5, 1, 0, 1,
                                         0.5, 0.5, 0.5, 0, 1, 1,
                                         -0.5, 0.5, 0.5, 1, 1, 1,
                                         -0.5, 0.5, -0.5, 0.5, 1, 0.5};

        static const int cubeInd[] = {1, 2, 3,
                                      7, 6, 5,
                                      4, 5, 1,
                                      5, 6, 2,
                                      2, 6, 7,
                                      0, 3, 7,
                                      0, 1, 3,
                                      4, 7, 5,
                                      0, 4, 1,
                                      1, 5, 2,
                                      3, 2, 7,
                                      4, 0, 7};



        max = (3.14 / 4);
        step = max / 100;
        angle = 0;
        increasing = true;


        // VAO
        glGenVertexArrays(1, &m_vaoID);
        glBindVertexArray(m_vaoID);

        // VBO
        glGenBuffers(1, &m_vboID);
        glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
        glBufferData(GL_ARRAY_BUFFER,
                     sizeof(cubeVertWithNormals),
                     cubeVertWithNormals,
                     GL_STATIC_DRAW);

        // Layout: 9 floats pro Vertex (pos 3, normal 3, color 3)
        GLsizei stride = 9 * sizeof(float);

        // Position (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // Normale (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Farbe (location = 2) – optional
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        //Setting the clear color to black (just once)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        //Depth test
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);   // oder weglassen, das ist Standard
        glClearDepth(1.0);

        transformLight = new Transform;

        rotate = new Transform;
        translateHead = new Transform;
        scaleRumpf = new Transform;
        scaleHead = new Transform;
        scaleA = new Transform;
        scaleLeg = new Transform;
        translateOA1 = new Transform;
        translateOA2 = new Transform;
        translateUA1 = new Transform;
        translateUA2 = new Transform;
        translateLeg1 = new Transform;
        translateLeg2 = new Transform;
        aroundPoint1 = new Transform;
        aroundPoint2 = new Transform;
        rotateUA = new Transform;
        animationArm1 = new Transform;
        animationArm2 = new Transform;
        transformLight = new Transform;



        scaleRumpf->scale(glm::vec3(0.4, 1, 0.4));

        scaleHead->scale(glm::vec3(0.2, 0.2, 0.2));
        translateHead->translate(glm::vec3(0, 0.7, 0));

        scaleA->scale(glm::vec3(0.1, 0.4, 0.1));
        translateOA1->translate(glm::vec3(0.3, 0.3, 0));
        translateOA2->translate(glm::vec3(-0.3, 0.3, 0));
        translateUA1->translate(glm::vec3(0.3, -0.13, 0));
        translateUA2->translate(glm::vec3(-0.3, -0.13, 0));

        scaleLeg->scale(glm::vec3(0.1, 0.4, 0.1));
        translateLeg1->translate(glm::vec3(0.1, -0.73, 0));
        translateLeg2->translate(glm::vec3(-0.1, -0.73, 0));

        rotateUA->rotateAroundPoint(vec3(0.3, 0.1, 0), vec3(0.3, 0, 0));

        //To get the side view
        rotate->rotate(glm::vec3(0, -1.57, 0));
        m_shader->setUniform("Rotate", rotate->getMatrix(), false);

        // --- Licht initialisieren ---
        lightPos   = glm::vec3(2.0f, 2.0f, 2.0f);   // Startposition des Lichts
        lightColor = glm::vec3(1.0f, 1.0f, 1.0f);   // weißes Licht
        lightTime  = 0.0f;

        // konstante Lichtfarbe an den Shader senden
        m_shader->setUniform("lightColor", lightColor);

        //aroundPoint1->rotateAroundPoint(glm::vec3(0.1, -0.43, 0), glm::vec3(max, 0, 0));
        //glDeleteBuffers(1, &bufferID);
        std::cout << "Scene initialization done\n";
        return true;
    }
    catch (std::exception &ex) {
        throw std::logic_error("Scene initialization failed:\n" + std::string(ex.what()) + "\n");
    }
}

void RobotAnimationScene::updateProjectionMatrix() {
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
    m_shader->setUniform("Projection", projection, false);
}

void RobotAnimationScene::render(float dt) {
    // Sicherstellen, dass der Shader existiert
    if (!m_shader) {
        std::cerr << "Shader program not initialized!" << std::endl;
        return;
    }

    m_shader->use();


    // HIER dazu:
    m_shader->setUniform("lightPos", lightPos);
    m_shader->setUniform("viewPos", cameraPos);


    // Framebuffer löschen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // View- und Projection-Matrix aktualisieren
    glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
    m_shader->setUniform("View", view, false);
    updateProjectionMatrix();

    // Basis-Model-Matrix (Identität)
    glm::mat4 model = glm::mat4(1.0f);
    m_shader->setUniform("ModelView", model, false);

    // Deine globale Rotation (Seitenansicht)
    m_shader->setUniform("Rotate", rotate->getMatrix(), false);

    // VAO binden
    glBindVertexArray(m_vaoID);

    // Anzahl der Vertices im Cube:
    // 9 floats pro Vertex (pos, normal, color)
    const int vertexCount = sizeof(cubeVertWithNormals) / (9 * sizeof(float));

    // ---------------- Animation / variance --------------------
    static int   direction = 1;
    static float delta     = 0.0f;

    delta += dt * direction;
    float variance = sin(delta);
    m_shader->setUniform("variance", variance);

    if (delta > 4.0f || delta < -4.0f) {
        direction *= -1;
        increasing = !increasing;
        std::cout << "change direction" << std::endl;
    }

    std::cout << delta << ", " << dt << std::endl;

    if (increasing) {
        aroundPoint1->rotateAroundPoint(glm::vec3(0.1f, -0.43f, 0.0f), glm::vec3( 0.3f * dt, 0.0f, 0.0f));
        aroundPoint2->rotateAroundPoint(glm::vec3(0.1f, -0.43f, 0.0f), glm::vec3(-0.3f * dt, 0.0f, 0.0f));
        animationArm1->rotateAroundPoint(glm::vec3(0.3f,  0.5f,  0.0f), glm::vec3( 0.3f * dt, 0.0f, 0.0f));
        animationArm2->rotateAroundPoint(glm::vec3(0.3f,  0.5f,  0.0f), glm::vec3(-0.3f * dt, 0.0f, 0.0f));
    } else {
        aroundPoint1->rotateAroundPoint(glm::vec3(0.1f, -0.43f, 0.0f), glm::vec3(-0.3f * dt, 0.0f, 0.0f));
        aroundPoint2->rotateAroundPoint(glm::vec3(0.1f, -0.43f, 0.0f), glm::vec3( 0.3f * dt, 0.0f, 0.0f));
        animationArm1->rotateAroundPoint(glm::vec3(0.3f,  0.5f,  0.0f), glm::vec3(-0.3f * dt, 0.0f, 0.0f));
        animationArm2->rotateAroundPoint(glm::vec3(0.3f,  0.5f,  0.0f), glm::vec3( 0.3f * dt, 0.0f, 0.0f));
    }

    // ---------------- Roboter zeichnen --------------------

    // Rumpf
    m_shader->setUniform("ModelView", scaleRumpf->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    // Kopf
    m_shader->setUniform("ModelView",
                         translateHead->getMatrix() * scaleHead->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    // Oberarm 1 (OA1)
    m_shader->setUniform("ModelView",
                         animationArm1->getMatrix() *
                         translateOA1->getMatrix() *
                         scaleA->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    // Oberarm 2 (OA2)
    m_shader->setUniform("ModelView",
                         animationArm2->getMatrix() *
                         translateOA2->getMatrix() *
                         scaleA->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    // Unterarm 1 (UA1) – falls du keine Unterarme willst, diese vier Blöcke einfach auskommentieren
    m_shader->setUniform("ModelView",
                         animationArm1->getMatrix() *
                         rotateUA->getMatrix() *
                         translateUA1->getMatrix() *
                         scaleA->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    // Unterarm 2 (UA2)
    m_shader->setUniform("ModelView",
                         animationArm2->getMatrix() *
                         rotateUA->getMatrix() *
                         translateUA2->getMatrix() *
                         scaleA->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    // Bein 1
    m_shader->setUniform("ModelView",
                         aroundPoint1->getMatrix() *
                         translateLeg1->getMatrix() *
                         scaleLeg->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    // Bein 2
    m_shader->setUniform("ModelView",
                         aroundPoint2->getMatrix() *
                         translateLeg2->getMatrix() *
                         scaleLeg->getMatrix(), false);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // ---------------- Lichtquelle visualisieren --------------------
    glm::mat4 lightModel = glm::mat4(1.0f);

    // an die Lichtposition schieben
    lightModel = glm::translate(lightModel, lightPos);

    // kleiner machen
    lightModel = glm::scale(lightModel, glm::vec3(0.1f, 0.1f, 0.1f));

    // an Shader senden
    m_shader->setUniform("ModelView", lightModel, false);

    // Würfel zeichnen
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







    std::cout << "Frame rendered" << std::endl;
}


void RobotAnimationScene::update(float dt) {
    // Licht auf einer Kreisbahn um die Figur bewegen
    lightTime += dt;
    lightPos.x = 3.0f * sin(lightTime);
    lightPos.z = 3.0f * cos(lightTime);
    lightPos.y = 2.0f;   // etwas über der Figur

    handleCameraInput();
}


OpenGLWindow *RobotAnimationScene::getWindow() {
    return m_window;
}

void RobotAnimationScene::onKey(Key key, Action action, Modifier modifier) {

}

void RobotAnimationScene::onMouseMove(MousePosition mouseposition) {

}

void RobotAnimationScene::onMouseButton(MouseButton button, Action action, Modifier modifier) {

}

void RobotAnimationScene::onMouseScroll(double xscroll, double yscroll) {

}

void RobotAnimationScene::onFrameBufferResize(int width, int height) {

}

void RobotAnimationScene::shutdown() {

}
// Methods to set camera parameters
void RobotAnimationScene::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    updateProjectionMatrix();
}

void RobotAnimationScene::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    updateProjectionMatrix();
}

void RobotAnimationScene::setFOV(float fov) {
    this->fov = fov;
    updateProjectionMatrix();
}

void RobotAnimationScene::setAspectRatio(float aspectRatio) {
    this->aspectRatio = aspectRatio;
    updateProjectionMatrix();
}

void RobotAnimationScene::handleCameraInput() {
    if (m_window->getInput().getKeyState(Key::Up) == KeyState::Pressed) {
        setFOV(fov + 1.0f);
    }
    if (m_window->getInput().getKeyState(Key::Down) == KeyState::Pressed) {
        setFOV(fov - 1.0f);
    }
    if (m_window->getInput().getKeyState(Key::Left) == KeyState::Pressed) {
        setAspectRatio(aspectRatio - 0.1f);
    }
    if (m_window->getInput().getKeyState(Key::Right) == KeyState::Pressed) {
        setAspectRatio(aspectRatio + 0.1f);
    }
}
