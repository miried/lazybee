/*
 * renderer.cpp - renderer subsystem
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
#include "renderer.h"

void error_callback(int error, const char* description);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void renderframe( GLFWwindow* window );
void GLEW_Init( void );

static void LoadShaders( void );
static void LoadTriangle( void );

std::vector<tdogl::Shader> shaders;
tdogl::Program* gProgram = NULL;
GLuint gVAO = 0;
GLuint gVBO = 0;

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

// loads the vertex shader and fragment shader, and links them to make the global gProgram
static void LoadShaders( void )
{
	shaders.push_back(tdogl::Shader::shaderFromFile("vertex-shader.txt", GL_VERTEX_SHADER));
	shaders.push_back(tdogl::Shader::shaderFromFile("fragment-shader.txt", GL_FRAGMENT_SHADER));
	gProgram = new tdogl::Program(shaders);
}

// loads a triangle into the VAO and VBO globals: gVAO and gVBO
static void LoadTriangle()
{
	// make and bind the VAO
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// make and bind the VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	// Put the three triangle verticies into the VBO
	GLfloat vertexData[] = {
		//  X     Y     Z
		 0.0f, 0.8f, 0.0f,
		-0.8f,-0.8f, 0.0f,
		 0.8f,-0.8f, 0.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vert"));
	glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// unbind the VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void renderframe( GLFWwindow* window )
{
	// clear everything
	glClearColor(0, 0, 0, 1); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind the program (the shaders)
	glUseProgram(gProgram->object());

	// bind the VAO (the triangle)
	glBindVertexArray(gVAO);

	// draw the VAO
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// unbind the VAO
	glBindVertexArray(0);

	// unbind the program
	glUseProgram(0);
}

/*
================
GLFW_Init

init GLFW system
================
*/
void renderer::init( const char *name )
{
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		printf( "glfwInit failed.\n");
		exit(EXIT_FAILURE);
	}
	
	createwindow( name );

	GLEW_Init();

	LoadShaders();
	LoadTriangle();
}

/*
================
GLFW_Window

open GLFW window
================
*/
void renderer::createwindow( const char *name )
{
//#define OLD_PROFILE
#ifdef OLD_PROFILE
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#else
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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
GLFW_RenderFrame

Frame Renderer
================
*/
void renderer::drawFrame( void )
{
	// Render New Frame
	renderframe(mainwindow);

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
int renderer::windowShouldClose( void )
{
	return glfwWindowShouldClose(mainwindow);
}

/*
================
GLFW_Shutdown

Cleanup and Shutdown
================
*/
void renderer::shutdown( void )
{
	// Cleanup
	delete gProgram;
	shaders.pop_back();
	shaders.pop_back();

	glfwDestroyWindow(mainwindow);
	glfwTerminate();
}

void GLEW_Init( void )
{
	GLenum err;

	glewExperimental = GL_TRUE;
	err = glewInit();
	if( err != GLEW_OK) {
		con_printf("glewInit failed: %s.\n", glewGetErrorString(err) );
		exit(EXIT_FAILURE);
	}
}
