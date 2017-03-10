#include "build.h"
#include "gfViewport.h"
#include "../../../external/glfw/include/GLFW/glfw3.h"

GfViewport::GfViewport()
	: m_window( nullptr )
{
	InitWindow();
}

void GfViewport::InitWindow()
{
	SC_ASSERT( m_window == nullptr, "Window already created" );
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode( monitor );
	const Float scale = 0.9f;
	m_window = glfwCreateWindow( ( Int32 )( mode->width * scale ), ( Int32 )( mode->height * scale ), "Image Processing", NULL, NULL );
}
