#pragma once
#include "../../utils/public/types.h"

#include <vector>

class GfViewport;
class Image;

namespace rendering
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Bool Init();
		Bool Render();
		void SetMainWindow( GfViewport* vp );
		void SetImage( Image* img );

	private:
		void RenderImage();

	private:
		std::vector< GfViewport* >	m_viewports;
		GfViewport*					m_viewport;
		Image*						m_image;
	};
}