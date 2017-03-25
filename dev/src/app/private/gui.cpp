#include "build.h"
#include "gui.h"

#include "../../../external/imgui/include/imgui.h"
#include "../../rendering/public/image.h"

#include "imageProcessor.h"
#include "processingPass.h"
#include "postProcessGui.h"

#include <memory>

namespace gf
{
	class SimpleEffectsWnd : public PostProcessWindow
	{
	public:
		// Inherited via PostProcessWindow
		virtual void Draw() override
		{
			this->DrawDefault();
		}
	};

	GuiDrawer::GuiDrawer()
	{
		m_simpleEffects.reset( new SimpleEffectsWnd() );
	}

	GuiDrawer::~GuiDrawer()
	{
	}

	void GuiDrawer::Draw()
	{
		bool show_test_window = true;
		bool show_another_window = true;
		const ImVec4 clear_color = ImColor( 114, 144, 154 );

		if ( m_simpleEffects )
			m_simpleEffects->Draw();

		//// 1. Show a simple window
		//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		//{
		//	static float f = 0.0f;
		//	ImGui::Text( "Hello, world!" );
		//	ImGui::SliderFloat( "float", &f, 0.0f, 1.0f );
		//	ImGui::ColorEdit3( "clear color", ( float* )&clear_color );
		//	ImGui::Text( "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate );
		//}

		//// 2. Show another simple window, this time using an explicit Begin/End pair
		//if ( show_another_window )
		//{
		//	ImGui::SetNextWindowSize( ImVec2( 200, 100 ), ImGuiSetCond_FirstUseEver );
		//	ImGui::Begin( "Another Window", &show_another_window );
		//	ImGui::Text( "Hello" );
		//	ImGui::End();
		//}

		//// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		//if ( show_test_window )
		//{
		//	ImGui::SetNextWindowPos( ImVec2( 650, 20 ), ImGuiSetCond_FirstUseEver );
		//	ImGui::ShowTestWindow( &show_test_window );
		//}

	}

	void GuiDrawer::SetImage( Image * img )
	{
		m_simpleEffects->SetImage( img );
	}

}