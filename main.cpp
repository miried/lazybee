// lazybee engine
// (C) 2014 Michael Rieder
//

#include <cstdarg>
#include "main.h"
#include "bspmap.h"
#include <GL/glew.h>
#include "glfw.h"

bspmap *dm2map;
if_glfw *glfw;

void shutdown( void );
void GLEW_Init( void );

void con_printf( const char *string, ... )
{
	va_list vl;
	va_start(vl,string);
	vprintf(string,vl);
	va_end(vl);
}

int main( int argc, char *argv[] )
{
	dm2map = new bspmap("mohdm2.bsp");
	glfw = new if_glfw("lazybee");

	GLEW_Init();
	con_printf( "============================================================\n" );
	con_printf( "Renderer initialized\n" );
	con_printf( "============================================================\n" );
	con_printf( "Vendor         : %s\n", glGetString(GL_VENDOR) );
	con_printf( "Renderer       : %s\n", glGetString(GL_RENDERER) );
	con_printf( "OpenGL version : %s\n", glGetString(GL_VERSION) );
	con_printf( "GLSL version   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );
	con_printf( "============================================================\n" );

	con_printf( "successful!\n" );

	while (!glfw->windowShouldClose())
		glfw->drawFrame();

	shutdown();

	return EXIT_SUCCESS;
}

void shutdown( void )
{
	delete glfw;
	delete dm2map;
}

void GLEW_Init( void )
{
	GLenum err;

	err = glewInit();
	if( err != GLEW_OK) {
		con_printf("glewInit failed: %s.\n", glewGetErrorString(err) );
		exit(EXIT_FAILURE);
	}
}
