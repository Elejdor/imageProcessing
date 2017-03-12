#include "build.h"
#include "rendering.h"

#include <GL/gl3w.h>
#include "../../../external/glfw/include/GLFW/glfw3.h"
#include <iostream>

#include "gfViewport.h"
#include "imguiFunctions.h"
#include "../public/image.h"

namespace shaders
{
	char* vsTexture =
		"#version 330\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"out vec2 Frag_UV;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	gl_Position = ProjMtx * vec4( Position.xy, 0, 1 );\n"
		"}\n";

	char* fsTexture =
		"#version 330\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = texture( Texture, Frag_UV.st );\n"
		"}\n";
}

namespace rendering
{
	GLint g_imageShader = 0;

	class RenderImage
	{
	public:
		RenderImage( Image* img )
			: m_img( img ) 
		{ 
		}

		void Draw()
		{

			if ( m_img )
			{
				// draw
				glEnableVertexAttribArray( 0 );
				glBindBuffer( GL_ARRAY_BUFFER, m_vb );
				glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, ( void* )0 );

				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ib );

				glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, ( void* )0 );
			}
		}

	private:
		void CreateBuffers()
		{
			if ( m_img )
			{
				// texture
				glGetIntegerv( GL_TEXTURE_BINDING_2D, &m_textureBinding );

				// plane
				const Uint32 indices[] = { 0, 1, 2, 0, 3, 1 };
				const Float verts[] = {
					1.0f, 1.0f, 0.0f,
					1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 0.0f,
					1.0f, 1.0f, 0.0f
				};

				// create buffers
				glGenBuffers( 1, &m_vb );
				glBindBuffer( GL_ARRAY_BUFFER, m_vb );
				glBufferData( GL_ARRAY_BUFFER, sizeof( verts ), verts, GL_STATIC_DRAW );

				glGenBuffers( 1, &m_ib );
				glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_ib );
				glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof( Uint32 ), indices, GL_STATIC_DRAW );
			}
		}

	private:
		Image* m_img;
		GLuint m_ib;
		GLuint m_vb;
		GLint  m_textureBinding;
	};

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

		static bool glProgramCreated = false;
		if ( !glProgramCreated )
		{
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
			glProgramCreated = true;
		}


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

	void Renderer::SetImage( Image* img )
	{
		m_image = img;
	}

	void Renderer::RenderImage()
	{
		if ( m_image )
		{

		}
	}
}