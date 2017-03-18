#pragma once

#include <GL/gl3w.h>

namespace gf
{
	class Image;
}

class ImageRenderer
{
public:
	ImageRenderer();

	void Draw();
	void SetImage( const gf::Image* const img );

private:
	void CreateBuffers();

private:
	const gf::Image*	m_img;
	GLuint				m_ib;
	GLuint				m_vb;
	GLuint				m_textureBinding;
};