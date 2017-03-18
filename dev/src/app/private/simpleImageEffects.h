#pragma once
#include "processingPass.h"

namespace gf
{
	class ImageProcessor;

	namespace effects
	{
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

		class ChangeContrast : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( gf::ImageProcessor* proc ) override;

			void SetAvg( const CalculateAvg* avg ) { m_avg = avg; }
			void SetContrast( Int16 val ) { m_contrast = val; }

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

		private:
			const CalculateAvg*		m_avg;
			Int16					m_contrast;
			Uint8					m_valueLUT[ 255 ];
		};

		class ChangeBrightness : public gf::IProcessingPass
		{
		public:
			virtual Bool OnStarted( ImageProcessor* proc ) override;

			virtual Uint8 Process( Uint8 input ) override;
			virtual Color3 Process( Color3 input ) override;

			void SetBrightness( Int32 val ) { m_brightness = val; }

		private:
			Uint8  m_valueLUT[ 256 ];
			Int32  m_brightness;
		};
	}
}