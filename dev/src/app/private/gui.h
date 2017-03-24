#pragma once
#include "../../rendering/public/drawable.h"
#include <memory>

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
		std::unique_ptr< PostProcessWindow > m_simpleEffects;
	};
}