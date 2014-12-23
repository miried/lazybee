/*
 * main.cpp - lazybee engine
 *
 * Copyright (C) 2014 Michael Rieder
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */

#include <cstdarg>
#include "main.h"
#include "bspmap.h"
#include "renderer.h"

bspmap *dm2map;
renderer *r;

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
	r = new renderer("lazybee");

	con_printf( "============================================================\n" );
	con_printf( "Renderer initialized\n" );
	con_printf( "============================================================\n" );
	con_printf( "Vendor         : %s\n", glGetString(GL_VENDOR) );
	con_printf( "Renderer       : %s\n", glGetString(GL_RENDERER) );
	con_printf( "OpenGL version : %s\n", glGetString(GL_VERSION) );
	con_printf( "GLSL version   : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION) );
	con_printf( "============================================================\n" );

	con_printf( "successful!\n" );

	r->renderloop();

	shutdown();

	return EXIT_SUCCESS;
}

void shutdown( void )
{
	delete r;
	delete dm2map;
}
