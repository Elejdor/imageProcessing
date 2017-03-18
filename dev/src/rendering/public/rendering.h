#pragma once
#include "../../utils/public/types.h"

#include <vector>

class GfViewport;
class ImageRenderer;

namespace gf
{
	class Image;
}

namespace rendering
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		Bool Init();

		Bool Render();
		void SetImage( const gf::Image* const img );

	private:
		void InitResources();
		void SetMainWindow( GfViewport* vp );
		void RenderImage();

	private:
		std::vector< GfViewport* >	m_viewports;
		GfViewport*					m_viewport;
		ImageRenderer*				m_image;
	};
}