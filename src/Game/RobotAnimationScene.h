#pragma once

#include "OpenGLWindow.h"
#include "Transform.h"
#include <ShaderProgram.h>
#include <memory>
#include <AssetManager.h>

class RobotAnimationScene
{
public:
	RobotAnimationScene(OpenGLWindow* window);
	~RobotAnimationScene();

	bool init();
	void shutdown();
	void render(float dt);
	void update(float dt);
	OpenGLWindow* getWindow();

	void onKey(Key key, Action action, Modifier modifier);
	void onMouseMove(MousePosition mouseposition);
	void onMouseButton(MouseButton button, Action action, Modifier modifier);
	void onMouseScroll(double xscroll, double yscroll);
	void onFrameBufferResize(int width, int height);

	void setNearPlane(float nearPlane);
	void setFarPlane(float farPlane);
	void setFOV(float fov);
	void setAspectRatio(float aspectRatio);

private:
	OpenGLWindow* m_window;
	AssetManager m_assets;
	ShaderProgram* m_shader;
	GLuint vaoID, vboID;

	Transform* rotate;
	Transform* scaleHead;
	Transform* translateHead;
	Transform*  scaleRumpf;
	Transform* scaleA;
	Transform* translateOA1;
	Transform* translateOA2;
	Transform* translateUA1;
	Transform* translateUA2;
	Transform* scaleLeg;
	Transform* translateLeg1;
	Transform* translateLeg2;
	Transform* aroundPoint1;
	Transform* aroundPoint2;
	Transform* rotateUA;
	Transform* animationArm1;
	Transform* animationArm2;
	Transform* transformLight;


	float max;
	float step;
	float angle;
	bool increasing;

	// Camera variables
	glm::vec3 cameraPos;
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;
	float nearPlane;
	float farPlane;
	float fov;
	float aspectRatio;

	// Light variables
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	float     lightTime;

	void updateProjectionMatrix();
	void handleCameraInput();


	GLuint m_vaoID;
	GLuint m_vboID;
	GLuint m_iboID;


};


