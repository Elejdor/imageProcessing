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

			template< typename T >
			Uint8 Clamp0255( T val )
			{
				if ( val > T( 255 ) )
					return 255u;
				if ( val < T( 0 ) )
					return 0u;

				return ( Uint8 )val;
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
			if ( !m_active )
				return false;

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
					m_valueLUT[ i ] = ( Uint8 )i;
				else
					m_valueLUT[ i ] = m_threshold;
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

		//////////////////////////////////////////////////////////////////////////
		// HighPassFilter
		//////////////////////////////////////////////////////////////////////////
		Bool HighPassFilter::OnStarted( ImageProcessor* proc )
		{
			for ( Uint16 i = 0; i < 256; ++i )
			{
				if ( i > m_threshold )
					m_valueLUT[ i ] = ( Uint8 )i;
				else
					m_valueLUT[ i ] = m_threshold;
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

		//////////////////////////////////////////////////////////////////////////
		// ValueHistogram
		//////////////////////////////////////////////////////////////////////////
		Bool ValueHistogram::OnStarted( ImageProcessor* proc )
		{
			for ( Uint8 i = 0; i < 255; ++i )
				m_values[ i ] = 0;

			return true;
		}

		Uint8 ValueHistogram::Process( Uint8 input )
		{
			++m_values[ input ];
			return input;
		}

		Color3 ValueHistogram::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}

		//////////////////////////////////////////////////////////////////////////
		// HistogramCorrection
		//////////////////////////////////////////////////////////////////////////
		Bool HistogramCorrection::OnStarted( ImageProcessor * proc )
		{
			//const std::array< Float, 256 >& histogram = m_histogram->GetData();
			//static const Float exponent = 1.f / 3.f;
			//const Float gMin = std::powf( m_min, exponent );
			//const Float gMax = std::powf( m_max, exponent );

			//const Double scale = ( Double )( gMax - gMin ) / proc->GetOutput()->GetSize(); // ( max - min ) / N
			//Int64 sumPow = 0;
			//for ( Uint32 i = 0; i < 256; ++i )
			//{
			//	//sum += ( int )histogram[ i ];
			//	const Int64 h = ( Int64 )histogram[ i ];
			//	sumPow += h * h * h; // sum( 0, f, h[m]^3 )
			//	
			//	const Int64 scaled = Int64( scale * sumPow );
			//	m_lut[ i ] = helper::Clamp0255( Int64( gMin + scaled ) ); // min + ( max - min ) / N * sum
			//}

			const std::array< Float, 256 >& histogram = m_histogram->GetData();
			static const Float exponent = 1.f / 3.f;

			const Double scale = ( Double )( m_max - m_min ) / proc->GetOutput()->GetSize(); // ( max - min ) / N
			Int64 sumPow = 0;
			for ( Uint32 i = 0; i < 256; ++i )
			{
				//sum += ( int )histogram[ i ];
				const Int64 h = ( Int64 )histogram[ i ];
				sumPow += h; // sum( 0, f, h[m]^3 )

				const Int64 scaled = Int64( scale * sumPow );
				m_lut[ i ] = helper::Clamp0255( Int64( m_min + scaled ) ); // min + ( max - min ) / N * sum
			}

			return true;
		}

		Uint8 HistogramCorrection::Process( Uint8 input )
		{
			return m_lut[ input ];
		}

		Color3 HistogramCorrection::Process( Color3 input )
		{
			SC_ASSERT( false, "Not implemented" );
			return Color3();
		}
	}
}