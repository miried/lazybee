/*
 * glfw.h - GLFW subsystem
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

#ifndef GLFW_H
#define GLFW_H

#include <GLFW/glfw3.h>

class if_glfw
{
public:
	void	init( void );
	void	createwindow( const char *name );
	void	drawFrame( void );
	int	windowShouldClose( void );
	void	shutdown( void );
	// constructor
	if_glfw( const char *name=NULL )
	{
		init();
		if (name) createwindow( name );
		else createwindow( "OpenGL window" );
	}
	~if_glfw()
	{
		shutdown();
	}
protected:
	// vars
	GLFWwindow* mainwindow;
};

#endif //GLFW_H
