#pragma once
#include "../../utils/public/types.h"

#include <vector>

class GfViewport;

namespace rendering
{
	class Renderer
	{
	public:
		Bool Init();
		void RegisterViewport( GfViewport* vp );

	private:
		std::vector< GfViewport* > m_viewports;
	};
}