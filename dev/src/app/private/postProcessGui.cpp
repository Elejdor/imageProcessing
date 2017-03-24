#include "build.h"
#include "postProcessGui.h"

#include "../../../external/imgui/include/imgui.h"
#include "../../rendering/public/image.h"

#include "imageProcessor.h"
#include "processingPass.h"

namespace gf
{
	//////////////////////////////////////////////////////////////////////////
	// EffectControl
	//////////////////////////////////////////////////////////////////////////
	EffectControl::EffectControl( const std::string& name )
		: m_name( name )
	{
	}

	void EffectControl::DrawDefault()
	{
		ImGui::Text( m_name.c_str() );
	}

	//////////////////////////////////////////////////////////////////////////
	// PostProcessWindow
	//////////////////////////////////////////////////////////////////////////
	void PostProcessWindow::DrawDefault()
	{
		static int currentItem = 1;
		const char* items[] = { "item1", "item2" };
		ImGui::PushItemWidth( -1 );
		ImGui::ListBox( "##effects", &currentItem, items, 2, 4 );

		if ( ImGui::Button( "Add" ) )
		{

		}
		ImGui::SameLine();
		if ( ImGui::Button( "Apply" ) )
		{
			if ( m_processor )
				m_processor->ProcessImage();
		}
	}
}
