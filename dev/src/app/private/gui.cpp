#include "build.h"
#include "gui.h"

#include "../../../external/imgui/include/imgui.h"
#include "../../rendering/public/image.h"

#include "imageProcessor.h"
#include "processingPass.h"
#include "postProcessGui.h"
#include "simpleImageEffects.h"

#include <memory>

extern void SetMainImage( gf::Image* img );

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
			ImGui::SliderInt( "Brightness", &m_value, -255, 255 );
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

			ImGui::SliderInt( "Min", &m_min, 0, 255 );
			ImGui::SliderInt( "Max", &m_max, 0, 255 );

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

			ImGui::SliderInt( "Contrast enhancement", &m_val, -255, 255 );

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
			, EffectControl( "Negate" )
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
	// LowPassFilter
	//////////////////////////////////////////////////////////////////////////
	class LowPassFilterControl : public EffectControl
	{
	public:
		LowPassFilterControl()
			: m_threshold( 255 )
			, EffectControl( "Low-pass filter" )
		{ }

		virtual void Draw() override
		{
			DrawDefault();

			ImGui::SliderInt( "Threshold ", &m_threshold, 0, 255 );
			m_effect.SetThreshold( m_threshold );
		}

		virtual IProcessingPass* GetEffect() override { return &m_effect; }

	private:
		effects::LowPassFilter	m_effect;
		int						m_threshold;
	};

	//////////////////////////////////////////////////////////////////////////
	// HighPassFilter
	//////////////////////////////////////////////////////////////////////////
	class HighPassFilterControl : public EffectControl
	{
	public:
		HighPassFilterControl()
			: m_threshold( 0 )
			, EffectControl( "High-pass filter" )
		{ }

		virtual void Draw() override
		{
			DrawDefault();

			ImGui::SliderInt( "Threshold", &m_threshold, 0, 255 );
			m_effect.SetThreshold( m_threshold );
		}

		virtual IProcessingPass* GetEffect() override { return &m_effect; }

	private:
		effects::HighPassFilter	m_effect;
		int						m_threshold;
	};

	//////////////////////////////////////////////////////////////////////////
	// SimpleEffectsWnd
	//////////////////////////////////////////////////////////////////////////
	class SimpleEffectsWnd : public PostProcessWindow
	{
	public:
		// Inherited via PostProcessWindow
		virtual void Draw() override
		{
			this->DrawDefault();

			ImGui::Begin( "Effects stack" );

			if ( ImGui::Button( "Apply" ) )
			{
				ResetProcessor();

				m_processor->ProcessImage();

				// HACK
				SetMainImage( m_processor->GetOutput() );
				m_processor->SetImage( m_img ); // reset processor's output
			}

			ImGui::SameLine();
			if ( ImGui::Button( "Reset" ) )
			{
				for ( Int32 i = 0; i < m_effects.size(); ++i )
					delete m_effects[ i ];

				m_effects.clear();
				ResetProcessor();
			}

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
				effects::Negate::GetName(),
				effects::LowPassFilter::GetName(),
				effects::HighPassFilter::GetName()
			};

			ImGui::PushItemWidth( -1 );
			ImGui::ListBox( "##effects", &currentItem, items, 6, 7 );

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
				else if ( currentItem == 4 )
					m_effects.push_back( new LowPassFilterControl() );
				else if ( currentItem == 5 )
					m_effects.push_back( new HighPassFilterControl() );
			}

			ImGui::Separator();
			static Int32 rosenfeld = 0;
			ImGui::SliderInt( "R", &rosenfeld, 0, 20 );
			if ( ImGui::Button( "Rosenfeld operator" ) )
			{
				ResetProcessor();
				m_processor->Rosenfeld( rosenfeld );
				SetMainImage( m_processor->GetOutput() );
				m_processor->SetImage( m_img ); // reset processor's output
			}

			ImGui::End();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// LinearFilterIngWnd
	//////////////////////////////////////////////////////////////////////////
	class LinearFilterIngWnd : public PostProcessWindow
	{
	public:
		// Inherited via PostProcessWindow
		virtual void Draw() override
		{
			ImGui::Begin( "Linear filtering" );

			// linear filtering
			static Int32 matrixData[ 9 ] = { 0 };
			struct Mat33i
			{
				Mat33i( Int32 m00, Int32 m01, Int32 m02,
						Int32 m10, Int32 m11, Int32 m12,
						Int32 m20, Int32 m21, Int32 m22 )
				{
					arr[ 0 ] = m00; arr[ 1 ] = m01; arr[ 2 ] = m02;
					arr[ 3 ] = m10; arr[ 4 ] = m11; arr[ 5 ] = m12;
					arr[ 6 ] = m20; arr[ 7 ] = m21; arr[ 8 ] = m22;
				}

				Int32 arr[ 9 ];
			};

			if ( ImGui::Button( "North" ) )
			{
				static const Int32 pattern[ 9 ] = {
					1, 1, 1,
					1, -2, 1,
					-1, -1, -1
				};

				memcpy( matrixData, pattern, 9 * sizeof( Int32 ) );
			}

			ImGui::SameLine();

			if ( ImGui::Button( "North-east" ) )
			{
				static const Int32 pattern[ 9 ] = {
					1, 1, 1,
					-1, -2, 1,
					-1, -1, 1
				};

				memcpy( matrixData, pattern, 9 * sizeof( Int32 ) );
			}

			ImGui::SameLine();
			if ( ImGui::Button( "East" ) )
			{
				static const Int32 pattern[ 9 ] = {
					-1, 1, 1,
					-1, -2, 1,
					-1, 1, 1
				};

				memcpy( matrixData, pattern, 9 * sizeof( Int32 ) );
			}

			ImGui::SameLine();
			if ( ImGui::Button( "South-east" ) )
			{
				static const Int32 pattern[ 9 ] = {
					-1, -1, 1,
					-1, -2, 1,
					1, 1, 1
				};

				memcpy( matrixData, pattern, 9 * sizeof( Int32 ) );
			}

			if ( ImGui::Button( "Gauss" ) )
			{
				static const Int32 pattern[ 9 ] = {
					1, 2, 1,
					2, 12, 2,
					1, 2, 1
				};

				memcpy( matrixData, pattern, 9 * sizeof( Int32 ) );
			}

			ImGui::SameLine();
			if ( ImGui::Button( "Laplacian" ) )
			{
				static const Int32 pattern[ 9 ] = {
					0, 1, 0,
					1, -4, 1,
					0, 1, 0
				};

				memcpy( matrixData, pattern, 9 * sizeof( Int32 ) );
			}

			ImGui::SameLine();
			if ( ImGui::Button( "Median" ) )
			{
				static const Int32 pattern[ 9 ] = {
					1, 1, 1,
					1, 1, 1,
					1, 1, 1
				};

				memcpy( matrixData, pattern, 9 * sizeof( Int32 ) );
			}

			ImGui::InputInt3( "##row1", matrixData );
			ImGui::InputInt3( "##row2", matrixData + 3 );
			ImGui::InputInt3( "##row3", matrixData + 6 );

			if ( ImGui::Button( "Apply filter" ) )
			{
				ResetProcessor();

				FilterMatrix filter;
				for ( Uint8 i = 0; i < 9; ++i )
					filter.arr[ i ] = static_cast< Uint8 >( matrixData[ i ] );

				m_processor->FilterImage( filter );
				SetMainImage( m_processor->GetOutput() );
				m_processor->SetImage( m_img ); // reset processor's output
			}

			ImGui::End();
		}
	};

	//////////////////////////////////////////////////////////////////////////
	// HistogramWnd
	//////////////////////////////////////////////////////////////////////////
	class HistogramWnd : public PostProcessWindow
	{
	public:
		HistogramWnd()
			: m_min( 0 )
			, m_max( 255 )
		{
			m_correction.SetHistogram( &m_histogram );
		}

		// Inherited via PostProcessWindow
		virtual void Draw() override
		{
			ImGui::Begin( "Histogram" );

			ImGui::SliderInt( "Min. density", &m_min, 0, 255 );
			ImGui::SliderInt( "Max. density", &m_max, 0, 255 );
			m_correction.SetRange( m_min, m_max );

			if ( ImGui::Button( "Calculate" ) )
			{
				ResetProcessor();
				m_processor->AddPass( &m_histogram );
				m_processor->ProcessImage( true );
				m_effects.clear();
			}

			if ( ImGui::Button( "Apply correction" ) )
			{
				ResetProcessor();
				m_processor->AddPass( &m_correction );
				m_processor->ProcessImage();
				SetMainImage( m_processor->GetOutput() );
				m_effects.clear();
				m_processor->AddPass( &m_histogram );
				m_processor->ProcessImage( true );
			}

			ImGui::PlotHistogram(
				"##histogram",
				m_histogram.GetData().data(),
				256,
				0,
				nullptr,
				0, std::numeric_limits<Float>::max(),
				ImVec2( 800, 100 ), 4 );

			ImGui::End();
		}

	private:
		effects::ValueHistogram			m_histogram;
		effects::HistogramCorrection	m_correction;
		int m_min;
		int m_max;
	};

	//////////////////////////////////////////////////////////////////////////
	// GuiDrawer
	//////////////////////////////////////////////////////////////////////////
	GuiDrawer::GuiDrawer()
	{
		m_windows.push_back( WndUniquePtr( new SimpleEffectsWnd ) );
		m_windows.push_back( WndUniquePtr( new LinearFilterIngWnd ) );
		m_windows.push_back( WndUniquePtr( new HistogramWnd ) );
	}

	GuiDrawer::~GuiDrawer()
	{
	}

	void GuiDrawer::Draw()
	{
		bool show_test_window = true;
		bool show_another_window = true;
		const ImVec4 clear_color = ImColor( 114, 144, 154 );

		for ( std::unique_ptr< PostProcessWindow >& wnd : m_windows )
			wnd->Draw();
	}

	void GuiDrawer::SetImage( Image * img )
	{
		for ( std::unique_ptr< PostProcessWindow >& wnd : m_windows )
			wnd->SetImage( img );
	}

}