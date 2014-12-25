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

const glm::vec2 SCREEN_SIZE(800, 600);

std::vector<tdogl::Shader> shaders;
tdogl::Texture* gTexture = NULL;
tdogl::Program* gProgram = NULL;
tdogl::Camera gCamera;
GLuint gVAO = 0;
GLuint gVBO = 0;
GLfloat gDegreesRotated = 0.0f;
uint_t numverts = 0;

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

// loads a cube into the VAO and VBO globals: gVAO and gVBO
static void LoadCube()
{
	// make and bind the VAO
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// make and bind the VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	// Make a cube out of triangles (two triangles per side)
	GLfloat vertexData[] = {
		//  X     Y     Z       U     V
		// bottom
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,

		// top
		-1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

		// front
		-1.0f,-1.0f, 1.0f,   1.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,   0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,

		// back
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f, 1.0f,-1.0f,   0.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,   1.0f, 1.0f,

		// left
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,
		-1.0f,-1.0f,-1.0f,   0.0f, 0.0f,
		-1.0f,-1.0f, 1.0f,   0.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,   1.0f, 0.0f,

		// right
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,   1.0f, 0.0f,
		 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		 1.0f,-1.0f, 1.0f,   1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,   0.0f, 0.0f,
		 1.0f, 1.0f, 1.0f,   0.0f, 1.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vert"));
	glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE,
			5*sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
	glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,
			5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	// unbind the VAO
	glBindVertexArray(0);
	
	numverts = 6 * 2 * 3;
}


// loads the file "wooden-crate.jpg" into gTexture
static void LoadTexture()
{
	tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile("wooden-crate.jpg");
	bmp.flipVertically();
	gTexture = new tdogl::Texture(bmp);
}

// update the scene based on the time elapsed since last update
void renderer::update(float secondsElapsed)
{
    //rotate the cube
    const GLfloat degreesPerSecond = 0.0f;
    gDegreesRotated += secondsElapsed * degreesPerSecond;
    while(gDegreesRotated > 360.0f) gDegreesRotated -= 360.0f;

    //move position of camera based on WASD keys, and XZ keys for up and down
    const float moveSpeed = 25.0; //units per second
    if(glfwGetKey(mainwindow, 'S')){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward());
    } else if(glfwGetKey(mainwindow, 'W')){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());
    }
    if(glfwGetKey(mainwindow, 'A')){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
    } else if(glfwGetKey(mainwindow, 'D')){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());
    }
    if(glfwGetKey(mainwindow, 'Z')){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.up());
    } else if(glfwGetKey(mainwindow, 'X')){
        gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.up());
    }

    //rotate camera based on mouse movement
    const float mouseSensitivity = 0.1f;
    double mouseX, mouseY;
    glfwGetCursorPos(mainwindow, &mouseX, &mouseY);
    gCamera.offsetOrientation(mouseSensitivity * (float)mouseY, mouseSensitivity * (float)mouseX);
    glfwSetCursorPos(mainwindow, 0, 0); //reset the mouse, so it doesn't go out of the window
}

/*
================
GLFW_Init

init GLFW system
================
*/
void renderer::init( const char *name )
{
	GLenum error;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {
		printf( "glfwInit failed.\n");
		exit(EXIT_FAILURE);
	}

	createwindow( name );

	GLEW_Init();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_CLAMP);

	LoadShaders();
	LoadCube();
	LoadTexture();

	// setup gCamera
	gCamera.setPosition(glm::vec3(0,0,0));
	gCamera.lookAt(glm::vec3(1,0,0));
	gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);

	error = glGetError();
	if(error != GL_NO_ERROR) {
		con_printf( "OpenGL Error %i (%s)\n",error, glewGetErrorString(error) );
	}
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

	mainwindow = glfwCreateWindow(SCREEN_SIZE.x, SCREEN_SIZE.y, name, NULL, NULL);
	if (!mainwindow) {
		printf("glfwOpenWindow failed.\n");
		exit(EXIT_FAILURE);
	}

	// GLFW settings
	glfwSetInputMode(mainwindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(mainwindow, 0, 0);

	glfwMakeContextCurrent(mainwindow);

	glfwSetKeyCallback(mainwindow, key_callback);
}

void renderer::setVertexData( float *vertexdata, uint_t numvertices )
{
	// make and bind the VAO
	glGenVertexArrays(1, &gVAO);
	glBindVertexArray(gVAO);

	// make and bind the VBO
	glGenBuffers(1, &gVBO);
	glBindBuffer(GL_ARRAY_BUFFER, gVBO);

	// Make a cube out of triangles (two triangles per side)
	GLfloat *vertexData = vertexdata;
	numverts = numvertices;

	glBufferData(GL_ARRAY_BUFFER, numverts*5*sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vert"));
	glVertexAttribPointer(gProgram->attrib("vert"), 3, GL_FLOAT, GL_FALSE,
			5*sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(gProgram->attrib("vertTexCoord"));
	glVertexAttribPointer(gProgram->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,
			5*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	// unbind the VAO
	glBindVertexArray(0);
}

/*
================
GLFW_RenderFrame

Frame Renderer
================
*/
void renderer::drawFrame( void )
{
	// clear everything
	glClearColor(0, 0, 0, 1); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// bind the program (the shaders)
	gProgram->use();

	// set the "camera" uniform
	gProgram->setUniform("camera", gCamera.matrix());

	// set the "model" uniform in the vertex shader, based on the gDegreesRotated global
	gProgram->setUniform("model", glm::rotate(glm::mat4(), glm::radians(gDegreesRotated), glm::vec3(0,1,0)));

	// bind the texture and set the "tex" uniform in the fragment shader
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gTexture->object());
	gProgram->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

	// bind the VAO (the triangle)
	glBindVertexArray(gVAO);

	// draw the VAO
	glDrawArrays(GL_TRIANGLES, 0, numverts);

	// unbind the VAO, the program and the texture
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	gProgram->stopUsing();

	// New Frame Buffer
	glfwSwapBuffers(mainwindow);
}

/*
================
renderer::renderloop

render loop
================
*/
void renderer::renderloop( void )
{
	double lastTime = glfwGetTime();
	while ( !glfwWindowShouldClose(mainwindow) ) {
		// process pending events
		glfwPollEvents();

		// update the scene based on the time elapsed since last update
		double thisTime = glfwGetTime();
		update((float)(thisTime - lastTime));
		lastTime = thisTime;

		// draw one frame
		drawFrame();

		// check for errors
		GLenum error = glGetError();
		if(error != GL_NO_ERROR) {
			con_printf( "OpenGL Error %i (%s)\n",
				error, glewGetErrorString(error) );
		}

		//exit program if escape key is pressed
		//if(glfwGetKey(mainwindow, GLFW_KEY_ESCAPE))
		//	glfwSetWindowShouldClose(mainwindow, GL_TRUE);
	}
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
	delete gTexture;
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
	// GLEW for some reason makes 1280 error which is cleared here
	err = glGetError();
	if(err != 1280) {
		con_printf( "glew Error %i (%s)\n",err, glewGetErrorString(err) );
	}
}
