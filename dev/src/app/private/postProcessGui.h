#pragma once
#include <memory>
#include <vector>
#include <string>

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
		virtual const IProcessingPass* GetEffect() const = 0;

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
		Image* m_img;
		std::unique_ptr< ImageProcessor >	m_processor;
		std::vector< EffectControl >		m_effects;
	};
}
