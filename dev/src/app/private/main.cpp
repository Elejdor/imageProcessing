#include "build.h"

#include "../../rendering/public/rendering.h"
#include "../../rendering/public/gfViewport.h"
#include "../../rendering/public/image.h"

#include <stdio.h>

int main( int, char** )
{
	rendering::Renderer renderer;
	renderer.Init();

	static Image iss;
	if ( iss.GetData() == nullptr )
		iss.Load( "iss.jpeg" );

	renderer.SetImage( &iss );

	// Main loop
	while ( renderer.Render() );

	return 0;
}