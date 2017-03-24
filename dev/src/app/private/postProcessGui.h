#pragma once
#include <memory>
#include <vector>
#include <string>
#include <typeinfo>

namespace gf
{
	class Image;
	class IProcessingPass;
	class ImageProcessor;

	class EffectControl
	{
	public:
		EffectControl( const std::string& name );

		virtual void Draw() = 0;
		virtual IProcessingPass* GetEffect() = 0;
		void DrawDefault();

	protected:
		std::string m_name;
	};

	class PostProcessWindow
	{
	public:
		virtual void Draw() = 0;
		void SetImage( Image* img ) { m_img = img; }
		void DrawDefault();

	protected:
		void ResetProcessor();

	protected:
		Image* m_img;
		std::unique_ptr< ImageProcessor >	m_processor;
		std::vector< EffectControl* >		m_effects;
	};
}
