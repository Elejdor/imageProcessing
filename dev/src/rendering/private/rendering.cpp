#include "build.h"
#include "rendering.h"

#include <GL/gl3w.h>
#include "../../../external/glfw/include/GLFW/glfw3.h"

#include "gfViewport.h"
#include "imguiFunctions.h"
#include "image.h"
#include "drawable.h"

#include "../private/imageRenderer.h"

#include <iostream>

namespace shaders
{	
	char* vsTexture =
		"#version 330\n"
		"layout(location = 0) in vec3 vertexPosition_modelspace;\n"
		"layout(location = 1) in vec2 vertexUV;\n"

		"out vec2 UV;\n"
		"void main()\n"
		"{\n"
		"	gl_Position.xyz = vertexPosition_modelspace;\n"
		"	gl_Position.w = 1.0;\n"
		"   UV = vertexUV;\n"
		"}\n";

	char* fsTexture =
		"#version 330\n"
		"in vec2 UV;\n"
		"out vec3 color;\n"
		"uniform sampler2D texSampler;\n"
		"void main()\n"
		"{\n"
		"   float value = texture( texSampler, UV ).x;\n"
		"	color = vec3( value, value, value );\n"
		"}\n";
}

namespace rendering
{
	GLint g_imageShader = 0;
	GLint g_texAttrib = 0;
	GLint g_mvpAttrib = 0;

	static void error_callback( int error, const char* description )
	{
		fprintf( stderr, "Error %d: %s\n", error, description );
	}

	Renderer::Renderer()
		: m_viewport( nullptr )
		, m_image( nullptr )
		, m_gui( nullptr )
	{
	}
	
	Renderer::~Renderer()
	{
		delete m_image;
		delete m_viewport;

		ImGui_ImplGlfwGL3_Shutdown();
		glfwTerminate();
	}

	Bool Renderer::Init()
	{
		// Setup window
		glfwSetErrorCallback( error_callback );
		if ( !glfwInit() )
			return false;

		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

		InitResources();

		return true;
	}

	void Renderer::InitResources()
	{
		SetMainWindow( new GfViewport() );

		// create shader for image
		g_imageShader = glCreateProgram();
		GLint vShader = glCreateShader( GL_VERTEX_SHADER );
		GLint fShader = glCreateShader( GL_FRAGMENT_SHADER );

		glShaderSource( vShader, 1, &shaders::vsTexture, 0 );
		glShaderSource( fShader, 1, &shaders::fsTexture, 0 );

		glCompileShader( vShader );
		glCompileShader( fShader );

		glAttachShader( g_imageShader, vShader );
		glAttachShader( g_imageShader, fShader );

		glLinkProgram( g_imageShader );

		GLint n = 0;
		glGetProgramiv( g_imageShader, GL_ACTIVE_UNIFORMS, &n );

		g_texAttrib = glGetUniformLocation( g_imageShader, "texSampler" );
		//g_mvpAttrib = glGetUniformLocation( g_imageShader, "MVP" );

		m_image = new ImageRenderer();
	}

	Bool Renderer::Render()
	{
		const ImVec4 clear_color = ImColor( 114, 144, 154 );

		ImGui_ImplGlfwGL3_NewFrame();

		if ( m_gui )
			m_gui->Draw();

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize( m_viewport->GetWindow(), &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );
		glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );
		glClear( GL_COLOR_BUFFER_BIT );

		RenderImage();
		ImGui::Render();

		glfwSwapBuffers( m_viewport->GetWindow() );
		glfwPollEvents();

		return glfwWindowShouldClose( m_viewport->GetWindow() ) == 0;
	}

	void Renderer::SetMainWindow( GfViewport* vp )
	{
		SC_ASSERT( vp, "null viewport passed" );
		m_viewport = vp;
		
		GLFWwindow* window = vp->GetWindow();
		SC_ASSERT( window, "Missing window" );

		glfwMakeContextCurrent( window );
		gl3wInit();

		ImGui_ImplGlfwGL3_Init( window, true );
	}

	void Renderer::SetImage( const gf::Image* const img )
	{
		SC_ASSERT( m_image, "Image renderer not initialized" );
		m_image->SetImage( img );
	}

	void Renderer::RenderImage()
	{
		SC_ASSERT( m_image, "Image rendere does not exist" );
		glUseProgram( g_imageShader );
		m_image->Draw();
	}
}