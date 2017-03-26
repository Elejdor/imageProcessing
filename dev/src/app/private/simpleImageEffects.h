#pragma once
#include "processingPass.h"

#include <array>

namespace gf
{
	class ImageProcessor;

	namespace effects
	{
		//////////////////////////////////////////////////////////////////////////
		// CalculateAvg
		//////////////////////////////////////////////////////////////////////////
		class CalculateAvg : public gf::IProcessingPass
		{
		public:
			CalculateAvg();

			virtual Bool OnStarted( gf::ImageProcessor* proc ) override;
			virtual Bool OnFinished( gf::ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			Uint8 GetResult() const { return ( Uint8 )m_result; }

		private:
			Uint32	m_result;
			Bool	m_calculated;
			Bool	m_bake;
		};

		//////////////////////////////////////////////////////////////////////////
		// ChangeContrast 
		//////////////////////////////////////////////////////////////////////////
		class ChangeContrast : public gf::IProcessingPass
		{
		public:
			ChangeContrast();
			~ChangeContrast();

			virtual Bool OnStarted( gf::ImageProcessor* proc ) override;

			void SetContrast( Int16 val ) { m_contrast = val; }

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void OnEffectRegistered( ImageProcessor* proc ) override;

			static const char* GetName() { return "SimpleContrast"; }

		private:
			CalculateAvg*			m_avg;
			Int16					m_contrast;
			Uint8					m_valueLUT[ 255 ];
		};

		//////////////////////////////////////////////////////////////////////////
		// ChangeBrightness 
		//////////////////////////////////////////////////////////////////////////
		class ChangeBrightness : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void SetBrightness( Int32 val ) { m_brightness = val; }

			static const char* GetName() { return "Brightness"; }

		private:
			Uint8  m_valueLUT[ 256 ];
			Int32  m_brightness;
		};
		
		//////////////////////////////////////////////////////////////////////////
		// Negate
		//////////////////////////////////////////////////////////////////////////
		class Negate : public gf::IProcessingPass
		{
		public:
			Negate()
				: m_active( true )
			{ }

			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			static const char* GetName() { return "Negate"; }
			inline void SetActive( Bool active ) { m_active = active; }

		private:
			Uint8	m_valueLUT[ 256 ];
			Bool	m_active;
		};
		
		//////////////////////////////////////////////////////////////////////////\
		// CalcRange
		//////////////////////////////////////////////////////////////////////////
		class CalcRange : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;
			
			Uint8 GetMin() const { return m_min; }
			Uint8 GetMax() const { return m_max; }
			
		private:
			Uint8		m_min;
			Uint8		m_max;
		};

		//////////////////////////////////////////////////////////////////////////
		// Normalize
		//////////////////////////////////////////////////////////////////////////
		class Normalize : public gf::IProcessingPass
		{
		public:
			Normalize();
			~Normalize();

			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void SetNewRange( Uint8 min, Uint8 max ) { m_newMin = min; m_newMax = max; }

			static const char* GetName() { return "Normalize"; }

			void OnEffectRegistered( ImageProcessor* proc ) override;

		private:
			CalcRange*			m_range;

			Uint8				m_newMin;
			Uint8				m_newMax;

			Uint8				m_valueLUT[ 256 ];
		};

		//////////////////////////////////////////////////////////////////////////
		// LowPassFilter
		//////////////////////////////////////////////////////////////////////////
		class LowPassFilter : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void SetThreshold( Uint8 val ) { m_threshold = val; }

			static const char* GetName() { return "Low-pass Filter"; }

		private:
			const CalcRange*	m_range;
			Uint8				m_threshold;
			Uint8				m_valueLUT[ 256 ];
		};

		//////////////////////////////////////////////////////////////////////////
		// HighPassFilter
		//////////////////////////////////////////////////////////////////////////
		class HighPassFilter : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void SetThreshold( Uint8 val ) { m_threshold = val; }

			static const char* GetName() { return "High-pass Filter"; }

		private:
			const CalcRange*	m_range;
			Uint8				m_threshold;
			Uint8				m_valueLUT[ 256 ];
		};
		
		//////////////////////////////////////////////////////////////////////////
		// ValueHistogram
		//////////////////////////////////////////////////////////////////////////
		class ValueHistogram : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			static const char* GetName() { return "High-pass Filter"; }

			const std::array< Float, 256 >& GetData() const { return m_values; }

		private:
			std::array< Float, 256 > m_values;
		};

		//////////////////////////////////////////////////////////////////////////
		// HistogramCorrection
		//////////////////////////////////////////////////////////////////////////
		class HistogramCorrection : public gf::IProcessingPass
		{
		public:
			void SetHistogram( const ValueHistogram* histogram ) { m_histogram = histogram; }

			virtual Bool OnStarted( ImageProcessor* proc ) override;
			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void SetRange( Uint8 min, Uint8 max )
			{
				m_min = min;
				m_max = max;
			}

		private:
			Uint8 MapDensity( Uint8 value );

		private:
			const ValueHistogram* m_histogram;
			Uint8 m_lut[ 256 ];

			Uint8 m_min;
			Uint8 m_max;

		};
	}
}