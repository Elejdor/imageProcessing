#include "build.h"
#include "imageRenderer.h"
#include "image.h"

ImageRenderer::ImageRenderer() 
	: m_img( nullptr )
{
	glGenVertexArrays( 1, &m_vb );
	glGenTextures( 1, &m_textureBinding );
}

void ImageRenderer::Draw()
{
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

void ImageRenderer::SetImage( const Image * const img )
{
	m_img = img;

	glBindTexture( GL_TEXTURE_2D, m_textureBinding );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_img->GetWidth(), m_img->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_img->GetData() );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}

void ImageRenderer::CreateBuffers()
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
