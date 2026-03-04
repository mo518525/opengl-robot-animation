#include "ApplicationWindow.h"


ApplicationWindow::ApplicationWindow() :
        OpenGLWindow(1280,				//width
			720,				//height
			false,				//fullscreen
			false,				//vsync
			4,					//OpenGL Version Major
			0,					//OpenGL Version Minor	=> Here the OpenGL Version is 3.3
			"Visual Computing Praktikum",			//Window title
			4,					//MSAA samples for default framebuffer
			false,				//use latest available OpenGL version (instead of the one specified above)
			120.0)				//Update frequency
{}

ApplicationWindow::~ApplicationWindow()
{

}

//Initialization here. (i.e. load a scene, load textures ...)
void ApplicationWindow::init()
{
	m_scene = std::unique_ptr<RobotAnimationScene>(new RobotAnimationScene(this));
	m_scene->init();
	setCursorVisible(true);
}

//cleanup. Free resources here.
void ApplicationWindow::shutdown()
{
	m_scene->shutdown();
}

//Update Window Logic here
void ApplicationWindow::update(GLdouble dtime)
{
	m_scene->update(static_cast<float>(dtime));
}

//Render a frame
void ApplicationWindow::render(GLdouble dtime)
{
	m_scene->render(static_cast<float>(dtime));
}

//Keyboard events
void ApplicationWindow::onKey(Key key, Action action, Modifier modifier)
{
	if (key == Key::Escape && action == Action::Down)
		quit();		
	m_scene->onKey(key, action, modifier);
}

//Mouse move events
void ApplicationWindow::onMouseMove(MousePosition mouseposition)
{
	m_scene->onMouseMove(mouseposition);
}

//Mouse Button events
void ApplicationWindow::onMouseButton(MouseButton button, Action action, Modifier modifier)
{
	m_scene->onMouseButton(button, action, modifier);
}

//Mouse scroll events
void ApplicationWindow::onMouseScroll(double xscroll, double yscroll)
{
	m_scene->onMouseScroll(xscroll, yscroll);
}

//Window resize events
void ApplicationWindow::onFrameBufferResize(int width, int height)
{
	m_scene->onFrameBufferResize(width, height);
}
