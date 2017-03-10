#pragma once
#include "../../utils/public/types.h"

#include <vector>

class GfViewport;

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

	private:
		std::vector< GfViewport* >	m_viewports;
		GfViewport*					m_viewport;
	};
}