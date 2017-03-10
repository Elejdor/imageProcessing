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
	m_window = glfwCreateWindow( 1280, 720, "Image Processing", NULL, NULL );
}
