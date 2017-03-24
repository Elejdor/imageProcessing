#include "build.h"
#include "postProcessGui.h"

#include "../../../external/imgui/include/imgui.h"
#include "../../rendering/public/image.h"

#include "imageProcessor.h"
#include "processingPass.h"
#include "simpleImageEffects.h"

namespace gf
{
	//////////////////////////////////////////////////////////////////////////
	// BrightnessControl
	//////////////////////////////////////////////////////////////////////////
	class BrightnessControl : public EffectControl
	{
	public:
		BrightnessControl()
			: m_value( 0 )
			, EffectControl( "Brightness" )
		{ }

		virtual void Draw() override
		{
			DrawDefault();
			const int min = -255;
			const int max = 255;
			ImGui::SliderInt( "Brightness", &m_value, min, max );
			m_effect.SetBrightness( m_value );
		}

		virtual IProcessingPass* GetEffect() override { return &m_effect; }

	private:
		effects::ChangeBrightness m_effect;
		int m_value;
	};

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
		for ( Uint32 i = 0; i < m_effects.size(); ++i )
		{
			m_effects[ i ]->Draw();
		}

		static int currentItem = 0;
		const char* items[] = { 
			effects::ChangeBrightness::GetName(), 
			effects::ChangeContrast::GetName(),
			effects::Normalize::GetName()
		};

		ImGui::PushItemWidth( -1 );
		ImGui::ListBox( "##effects", &currentItem, items, 3, 4 );

		if ( ImGui::Button( "Add" ) )
		{
			if ( currentItem == 0 )
			{
				m_effects.push_back( new BrightnessControl() );
				ResetProcessor();
			}
		}

		ImGui::SameLine();
		if ( ImGui::Button( "Apply" ) )
		{
			if ( m_processor )
				m_processor->ProcessImage();

			// HACK
			extern void SetMainImage( gf::Image* img );
			SetMainImage( m_processor->GetOutput() );
			m_processor->SetImage( m_img ); // reset processor's output
		}
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
