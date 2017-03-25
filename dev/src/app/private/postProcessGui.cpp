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
	// NormalizeControl
	//////////////////////////////////////////////////////////////////////////
	class NormalizeControl : public EffectControl
	{
	public:
		NormalizeControl()
			: m_min( 0 )
			, m_max( 255 )
			, EffectControl( "Normalize" )
		{ }

		virtual void Draw() override
		{
			DrawDefault();
			const int min = 0;
			const int max = 255;

			ImGui::SliderInt( "Min", &m_min, min, max );
			ImGui::SliderInt( "Max", &m_max, min, max );

			if ( m_min > m_max )
				m_min = m_max;

			m_effect.SetNewRange( ( Uint8 )m_min, ( Uint8 )m_max );
		}

		virtual IProcessingPass* GetEffect() override { return &m_effect; }

	private:
		effects::Normalize m_effect;
		int m_min;
		int m_max;
	};

	//////////////////////////////////////////////////////////////////////////
	// Contrast
	//////////////////////////////////////////////////////////////////////////
	class ContrastControl : public EffectControl
	{
	public:
		ContrastControl()
			: m_val( 0 )
			, EffectControl( "Contrast" )
		{ }

		virtual void Draw() override
		{
			DrawDefault();
			const int min = -255;
			const int max = 255;

			ImGui::SliderInt( "Contrast enhancement", &m_val, min, max );

			m_effect.SetContrast( m_val );
		}

		virtual IProcessingPass* GetEffect() override { return &m_effect; }

	private:
		effects::ChangeContrast m_effect;
		int m_val;
	};

	//////////////////////////////////////////////////////////////////////////
	// Negate
	//////////////////////////////////////////////////////////////////////////
	class NegateControl : public EffectControl
	{
	public:
		NegateControl()
			: m_active( true )
			, EffectControl( "Contrast" )
		{ }

		virtual void Draw() override
		{
			DrawDefault();

			ImGui::Checkbox( "Enabled", &m_active );
			m_effect.SetActive( m_active );
		}

		virtual IProcessingPass* GetEffect() override { return &m_effect; }

	private:
		effects::Negate			m_effect;
		Bool					m_active;
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
		ImGui::Begin( "Effects stack" );

		for ( Uint32 i = 0; i < m_effects.size(); ++i )
		{
			m_effects[ i ]->Draw();
		}

		ImGui::End();

		ImGui::Begin( "Simple effects" );
		static int currentItem = 0;
		const char* items[] = { 
			effects::ChangeBrightness::GetName(), 
			effects::Normalize::GetName(),
			effects::ChangeContrast::GetName(),
			effects::Negate::GetName()
		};

		ImGui::PushItemWidth( -1 );
		ImGui::ListBox( "##effects", &currentItem, items, 3, 4 );

		if ( ImGui::Button( "Add" ) )
		{
			if ( currentItem == 0 )
				m_effects.push_back( new BrightnessControl() );
			else if ( currentItem == 1 )
				m_effects.push_back( new NormalizeControl() );
			else if ( currentItem == 2 )
				m_effects.push_back( new ContrastControl() );
			else if ( currentItem == 3 )
				m_effects.push_back( new NegateControl() );

			ResetProcessor();
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

		static Int32 row[ 9 ] = { 0 };
		ImGui::InputInt3( "##row1", row );
		ImGui::InputInt3( "##row2", row + 3 );
		ImGui::InputInt3( "##row3", row + 6 );

		if ( ImGui::Button( "Apply filter" ) )
		{
			ResetProcessor();

			FilterMatrix filter;
			for ( Uint8 i = 0; i < 9; ++i )
				filter.arr[ i ] = static_cast< Uint8 >( row[ i ] );

			m_processor->FilterImage( filter );
			extern void SetMainImage( gf::Image* img );
			SetMainImage( m_processor->GetOutput() );
			m_processor->SetImage( m_img ); // reset processor's output
		}

		ImGui::End();
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
