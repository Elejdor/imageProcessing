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
		ImGui::Separator();
		ImGui::Text( m_name.c_str() );
	}

	//////////////////////////////////////////////////////////////////////////
	// PostProcessWindow
	//////////////////////////////////////////////////////////////////////////
	void PostProcessWindow::DrawDefault()
	{
	}

	void PostProcessWindow::ResetProcessor()
	{
		m_processor.reset( new ImageProcessor );
		m_processor->SetImage( m_img );

		for ( Uint32 i = 0; i < m_effects.size(); ++i )
		{
			m_processor->AddPass( m_effects[ i ]->GetEffect() );
		}
	}
}
