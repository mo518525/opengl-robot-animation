#ifndef _APPLICATION_WINDOW_H_
#define _APPLICATION_WINDOW_H_

#include <OpenGLWindow.h>
#include <RobotAnimationScene.h>
#include <memory>

class ApplicationWindow : public OpenGLWindow
{
public:
	ApplicationWindow();
	~ApplicationWindow();

	void init() override;
	void shutdown() override;

	void update(GLdouble dtime) override;
	void render(GLdouble dtime) override;

	void onKey(Key key, Action action, Modifier modifier) override;
	void onMouseMove(MousePosition mouseposition) override;
	void onMouseButton(MouseButton button, Action action, Modifier modifier) override;
	void onMouseScroll(double xscroll, double yscroll) override;

	void onFrameBufferResize(int width, int height) override;

private:
	std::unique_ptr<RobotAnimationScene> m_scene;
};

#endif
