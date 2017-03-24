#include "build.h"
#include "imageRenderer.h"
#include "image.h"

ImageRenderer::ImageRenderer() 
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

void ImageRenderer::SetImage( const gf::Image* const img )
{
	glBindTexture( GL_TEXTURE_2D, m_textureBinding );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_R8, img->GetWidth(), img->GetHeight(), 0, GL_RED, GL_UNSIGNED_BYTE, img->GetData() );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
}