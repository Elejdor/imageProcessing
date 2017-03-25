#include "build.h"
#include "simpleImageEffects.h"
#include "imageProcessor.h"
#include "../../rendering/public/image.h"

namespace gf
{
	namespace effects
	{
		namespace helper
		{
			Uint8 AddVal( Int16 val, Int16 add )
			{
				if ( add < 0 )
				{
					if ( val < ( -add ) )
						return 0;
				}
				else if ( add > 0 )
				{
					if ( 255 - val < add )
						return 255;
				}

				return val + add;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// CalculateAvg
		//////////////////////////////////////////////////////////////////////////
		CalculateAvg::CalculateAvg()
			: m_calculated( false )
			, m_bake( true )
			, m_result( 0 )
		{
		}

		Bool CalculateAvg::OnStarted( ImageProcessor* processor )
		{
			if ( m_bake && m_calculated )
				return false;

			m_result = 0;
			return true;
		}

		Bool CalculateAvg::OnFinished( ImageProcessor* proc )
		{
			m_result /= proc->GetOutput()->GetDataSize();
			m_calculated = true;
			return true;
		}

		Uint8 CalculateAvg::Process( Uint8 input )
		{
			m_result += input;
			return input;
		}

		Color3 CalculateAvg::Process( Color3 input )
		{
			m_result += ( input.r + input.g + input.b );
			return input;
		}

		//////////////////////////////////////////////////////////////////////////
		// ChangeContrast
		//////////////////////////////////////////////////////////////////////////
		ChangeContrast::ChangeContrast()
			: m_avg( new CalculateAvg )
		{ }

		ChangeContrast::~ChangeContrast()
		{
			delete m_avg;
			m_avg = nullptr;
		}

		Bool ChangeContrast::OnStarted( gf::ImageProcessor * proc )
		{
			const Uint8 avg = m_avg->GetResult();
			for ( Uint16 i = 0; i < 256; ++i )
			{
				if ( i < avg )
				{
					m_valueLUT[ i ] = helper::AddVal( i, -m_contrast );
				}
				else if ( avg < i )
				{
					m_valueLUT[ i ] = helper::AddVal( i, m_contrast );
				}
				else
				{
					m_valueLUT[ i ] = ( Uint8 )i;
				}
			}

			return true;
		}

		Uint8 ChangeContrast::Process( Uint8 input )
		{
			return m_valueLUT[ input ];
		}

		Color3 ChangeContrast::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}

		void ChangeContrast::OnEffectRegistered( ImageProcessor* proc )
		{
			proc->AddPass( m_avg );			
		}

		//////////////////////////////////////////////////////////////////////////
		// ChangeBrightness
		//////////////////////////////////////////////////////////////////////////
		Bool ChangeBrightness::OnStarted( ImageProcessor * proc )
		{
			for ( Uint16 i = 0; i < 256; ++i )
			{
				m_valueLUT[ i ] = helper::AddVal( i, m_brightness );
			}

			return true;
		}

		Uint8 ChangeBrightness::Process( Uint8 input )
		{
			return m_valueLUT[ input ];
		}

		Color3 ChangeBrightness::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}

		//////////////////////////////////////////////////////////////////////////
		// Negate
		//////////////////////////////////////////////////////////////////////////
		Bool Negate::OnStarted( ImageProcessor * proc )
		{
			for ( Uint16 i = 0; i < 255; ++i )
			{
				m_valueLUT[ i ] = 255 - i;
			}

			return true;
		}

		Uint8 Negate::Process( Uint8 input )
		{
			return m_valueLUT[ input ];
		}

		Color3 Negate::Process( Color3 input )
		{
			input.r = m_valueLUT[ input.r ];
			input.g = m_valueLUT[ input.g ];
			input.b = m_valueLUT[ input.b ];

			return input;
		}

		//////////////////////////////////////////////////////////////////////////
		// CalcRange
		//////////////////////////////////////////////////////////////////////////
		Bool CalcRange::OnStarted( ImageProcessor * proc )
		{
			m_min = 0xff;
			m_max = 0;
			return true;
		}

		Uint8 CalcRange::Process( Uint8 input )
		{
			if ( input < m_min )
				m_min = input;
			else if ( m_max < input )
				m_max = input;

			return input;
		}

		Color3 CalcRange::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}

		//////////////////////////////////////////////////////////////////////////
		// Normalize
		//////////////////////////////////////////////////////////////////////////
		Normalize::Normalize()
			: m_range( new CalcRange() )
		{ }

		Normalize::~Normalize()
		{
			delete m_range;
			m_range = nullptr;
		}

		Bool Normalize::OnStarted( ImageProcessor * proc )
		{
			const Uint8 min = m_range->GetMin();
			const Uint8 max = m_range->GetMax();

			const float rel = ( float )( m_newMax - m_newMin ) / ( max - min );

			for ( Uint16 i = 0; i < 256; ++i )
			{
				m_valueLUT[ i ] = ( Uint8 )( rel * ( i - min ) + m_newMin );
			}

			return true;
		}

		Uint8 Normalize::Process( Uint8 input )
		{
			return m_valueLUT[ input ];
		}

		Color3 Normalize::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}

		void Normalize::OnEffectRegistered( ImageProcessor * proc )
		{
			proc->AddPass( m_range );
		}

		//////////////////////////////////////////////////////////////////////////
		// LowPassFilter
		//////////////////////////////////////////////////////////////////////////
		Bool LowPassFilter::OnStarted( ImageProcessor* proc )
		{
			for ( Uint16 i = 0; i < 256; ++i )
			{
				if ( i < m_threshold )
					i = i;
				else
					i = 0;
			}
			return true;
		}

		Uint8 LowPassFilter::Process( Uint8 input )
		{
			return m_valueLUT[ input ];
		}

		Color3 LowPassFilter::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}

		// HighPassFilter
		Bool HighPassFilter::OnStarted( ImageProcessor* proc )
		{
			for ( Uint16 i = 0; i < 256; ++i )
			{
				if ( i > m_threshold )
					i = i;
				else
					i = 0;
			}
			return true;
		}

		Uint8 HighPassFilter::Process( Uint8 input )
		{
			return m_valueLUT[ input ];
		}

		Color3 HighPassFilter::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}
	}
}