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
		"	color = texture( texSampler, UV ).rgb;\n"
		"}\n";
}

namespace rendering
{
	GLint g_imageShader = 0;
	GLint g_texAttrib = 0;
	GLint g_mvpAttrib = 0;

	class ImageRenderer
	{
	public:
		ImageRenderer( Image* img )
			: m_img( img ) 
		{ 
			glGenVertexArrays( 1, &m_vb );
			glGenTextures( 1, &m_textureBinding );
			glBindTexture( GL_TEXTURE_2D, m_textureBinding );

			glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_img->GetWidth(), m_img->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_img->GetData() );

			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		}

		void Draw()
		{
			glUseProgram( g_imageShader );
			glBindVertexArray( m_vb );

			static const GLfloat g_vertex_buffer_data[] = {
				-1.0f, -1.0f, 0.0f,
				1.0f, -1.0f, 0.0f,
				-1.0f,  1.0f, 0.0f,
				1.0f,  1.0f, 0.0f
			};

			GLuint vertexbuffer;
			// Generate 1 buffer, put the resulting identifier in vertexbuffer
			glGenBuffers( 1, &vertexbuffer );
			// The following commands will talk about our 'vertexbuffer' buffer
			glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
			// Give our vertices to OpenGL.
			glBufferData( GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );

			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray( 0 );
			glBindBuffer( GL_ARRAY_BUFFER, vertexbuffer );
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				( void* )0            // array buffer offset
			);

			const Float UVs[] =
			{
				1.0f, 1.0f,
				0.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f
			};

			GLuint uvBuffer;
			glGenBuffers( 1, &uvBuffer );
			glBindBuffer( GL_ARRAY_BUFFER, uvBuffer );
			glBufferData( GL_ARRAY_BUFFER, sizeof( UVs ), UVs, GL_STATIC_DRAW );

			glEnableVertexAttribArray( 1 );
			glBindBuffer( GL_ARRAY_BUFFER, uvBuffer );
			glVertexAttribPointer(
				1,
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				0            // array buffer offset
			);

			// Draw the triangle !
			glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 ); // Starting from vertex 0; 3 vertices total -> 1 triangle
			glDisableVertexAttribArray( 1 );
			glDisableVertexAttribArray( 0 );
		}

	private:
		void CreateBuffers()
		{
			if ( m_img )
			{
				// texture
				glGenTextures( 1, &m_textureBinding );
				glBindTexture( GL_TEXTURE_2D, m_textureBinding );
				glTexImage2D( GL_TEXTURE_2D,
							  0,
							  GL_RGB,
							  m_img->GetWidth(), m_img->GetHeight(),
							  0,
							  GL_BGR,
							  GL_UNSIGNED_BYTE,
							  m_img->GetData() );


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
		GLuint m_textureBinding;
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

			GLint n = 0;
			glGetProgramiv( g_imageShader, GL_ACTIVE_UNIFORMS, &n );

			g_texAttrib = glGetUniformLocation( g_imageShader, "texSampler" );
			//g_mvpAttrib = glGetUniformLocation( g_imageShader, "MVP" );

			glProgramCreated = true;
		}
		// image test



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

		static Image iss;
		if ( iss.GetData() == nullptr )
			iss.Load( "iss.jpeg" );

		ImageRenderer rend( &iss );
		rend.Draw();

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