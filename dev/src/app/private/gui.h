#pragma once
#include "../../rendering/public/drawable.h"
#include "postProcessGui.h"

namespace gf
{
	class GuiDrawer : public IDrawable
	{
	public:
		GuiDrawer();
		virtual void Draw() override;

	private:
		PostProcessWindow* m_simpleEffects;
	};
}