/*
 * glfw.cpp - GLFW subsystem
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

// includes
#include "main.h"
#include "glfw.h"

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void renderframe( void );

/*
================
error_callback

handle glfw errors
================
*/
void error_callback(int error, const char* description)
{
	con_printf( "GLFW: An error (%i) occurred: %s\n", error, description );
}

/*
================
GLFW_Init

init GLFW system
================
*/
void if_glfw::init( void )
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		printf( "glfwInit failed.\n");
		exit(EXIT_FAILURE);
	}
}

/*
================
GLFW_Window

open GLFW window
================
*/
void if_glfw::createwindow( const char *name )
{
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	mainwindow = glfwCreateWindow(640, 480, name, NULL, NULL);
	if (!mainwindow) {
		printf("glfwOpenWindow failed.\n");
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(mainwindow);

	glfwSetKeyCallback(mainwindow, key_callback);
}

/*
================
key_callback

callback for keypress events
================
*/
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		// Key pressed
		if (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_ENTER )
			// close
			glfwSetWindowShouldClose(window, GL_TRUE);
}

void renderframe( void )
{
	// render nothing
}

/*
================
GLFW_RenderFrame

Frame Renderer
================
*/
void if_glfw::drawFrame( void )
{
	// Render New Frame
	renderframe();

	// New Frame Buffer
	glfwSwapBuffers(mainwindow);

	// Check for new Events
	glfwPollEvents();
}

/*
================
GLFW_RenderFrame

Frame Renderer
================
*/
int if_glfw::windowShouldClose( void )
{
	return glfwWindowShouldClose(mainwindow);
}

/*
================
GLFW_Shutdown

Cleanup and Shutdown
================
*/
void if_glfw::shutdown( void )
{
	// Cleanup
	glfwDestroyWindow(mainwindow);
	glfwTerminate();
}
