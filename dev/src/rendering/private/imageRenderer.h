#pragma once

#include <GL/gl3w.h>

class Image;

class ImageRenderer
{
public:
	ImageRenderer();

	void Draw();
	void SetImage( const Image* const img );

private:
	void CreateBuffers();

private:
	const Image*	m_img;
	GLuint			m_ib;
	GLuint			m_vb;
	GLuint			m_textureBinding;
};