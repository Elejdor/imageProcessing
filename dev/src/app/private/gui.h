#pragma once
#include "../../rendering/public/drawable.h"

#include <memory>
#include <vector>

namespace gf
{
	class PostProcessWindow;
	class Image;

	class GuiDrawer : public IDrawable
	{
	public:
		GuiDrawer();
		~GuiDrawer();

		virtual void Draw() override;
		void SetImage( Image* img );

	private:
		typedef std::unique_ptr< PostProcessWindow > WndUniquePtr;
		typedef std::vector< WndUniquePtr > TWindows;
		TWindows m_windows;
	};
}