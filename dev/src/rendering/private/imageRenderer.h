#pragma once
#include <GL/gl3w.h>
#include <memory>

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
	GLuint				m_ib;
	GLuint				m_vb;
	GLuint				m_textureBinding;
};