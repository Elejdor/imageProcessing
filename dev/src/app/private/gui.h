#pragma once
#include "../../rendering/public/drawable.h"

namespace gf
{
	class GuiDrawer : public IDrawable
	{
	public:
		virtual void Draw() override;
	};
}