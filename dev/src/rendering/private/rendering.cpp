#include "build.h"
#include "rendering.h"

#include <GL/gl3w.h>
#include "../../../external/glfw/include/GLFW/glfw3.h"
#include <iostream>

#include "gfViewport.h"
#include "imguiFunctions.h"

namespace rendering
{
	static void error_callback( int error, const char* description )
	{
		fprintf( stderr, "Error %d: %s\n", error, description );
	}

	Renderer::Renderer()
		: m_viewport( nullptr )
	{
	}
	
	Renderer::~Renderer()
	{
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

		return true;
	}

	Bool Renderer::Render()
	{
		bool show_test_window = true;
		bool show_another_window = true;
		const ImVec4 clear_color = ImColor( 114, 144, 154 );

		ImGui_ImplGlfwGL3_NewFrame();

		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		{
			static float f = 0.0f;
			ImGui::Text( "Hello, world!" );
			ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );
			ImGui::ColorEdit3( "clear color", ( float* )&clear_color );
			ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		}

		// 2. Show another simple window, this time using an explicit Begin/End pair
		if ( show_another_window )
		{
			ImGui::SetNextWindowSize( ImVec2( 200, 100 ), ImGuiSetCond_FirstUseEver );
			ImGui::Begin( "Another Window", &show_another_window );
			ImGui::Text( "Hello" );
			ImGui::End();
		}

		// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if ( show_test_window )
		{
			ImGui::SetNextWindowPos( ImVec2( 650, 20 ), ImGuiSetCond_FirstUseEver );
			ImGui::ShowTestWindow( &show_test_window );
		}

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize( m_viewport->GetWindow(), &display_w, &display_h );
		glViewport( 0, 0, display_w, display_h );
		glClearColor( clear_color.x, clear_color.y, clear_color.z, clear_color.w );
		glClear( GL_COLOR_BUFFER_BIT );
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
}