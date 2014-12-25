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
void renderframe( GLFWwindow* window );
void GLEW_Init( void );

const glm::vec2 SCREEN_SIZE(800, 600);

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

// returns a new tdogl::Program created from the given vertex and fragment shader filenames
static tdogl::Program* LoadShaders(const char* vertFilename, const char* fragFilename) {
    std::vector<tdogl::Shader> shaders;
    shaders.push_back(tdogl::Shader::shaderFromFile(vertFilename, GL_VERTEX_SHADER));
    shaders.push_back(tdogl::Shader::shaderFromFile(fragFilename, GL_FRAGMENT_SHADER));
    return new tdogl::Program(shaders);
}


// returns a new tdogl::Texture created from the given filename
static tdogl::Texture* LoadTexture(const char* filename) {
    tdogl::Bitmap bmp = tdogl::Bitmap::bitmapFromFile(filename);
    bmp.flipVertically();
    return new tdogl::Texture(bmp);
}

// update the scene based on the time elapsed since last update
void renderer::update(float secondsElapsed)
{
	GLFWwindow	*w = mainwindow;

	// check for close keys
	if ( glfwGetKey(w,GLFW_KEY_ESCAPE) || glfwGetKey(w,GLFW_KEY_ENTER) )
		// close window
		glfwSetWindowShouldClose(w, GL_TRUE);

	//move position of camera based on WASD keys, and XZ keys for up and down
	const float moveSpeed = 500.0; //units per second
	if(glfwGetKey(w, 'S')){
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.forward());
	} else if(glfwGetKey(w, 'W')){
		gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.forward());
	}
	if(glfwGetKey(w, 'A')){
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.right());
	} else if(glfwGetKey(w, 'D')){
		gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.right());
	}
	if(glfwGetKey(w, 'Z')){
		gCamera.offsetPosition(secondsElapsed * moveSpeed * -gCamera.up());
	} else if(glfwGetKey(w, 'X')){
		gCamera.offsetPosition(secondsElapsed * moveSpeed * gCamera.up());
	}
	if ( glfwGetKey(w,'G') ) {
		const glm::vec3& pos = gCamera.position();
		con_printf( "pos: %f, %f, %f\n", pos.x, pos.y, pos.z );
	}

	//rotate camera based on mouse movement
	const float mouseSensitivity = 0.2f;
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

	CreateInstances();

	// setup gCamera
	gCamera.setPosition(glm::vec3(0,0,0));
	gCamera.lookAt(glm::vec3(1,0,0));
	gCamera.setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);
	gCamera.setNearAndFarPlanes(1.0f, 5000.0f);

	// setup lights
	Light spotlight;
	spotlight.position = glm::vec4(380,-1000,650,1);
	spotlight.intensities = glm::vec3(2,2,2); //strong white light
	spotlight.attenuation = 0.1f;
	spotlight.ambientCoefficient = 0.0f; //no ambient light
	spotlight.coneAngle = 15.0f;
	spotlight.coneDirection = glm::vec3(0,0,-1);

	Light directionalLight;
	directionalLight.position = glm::vec4(30, -1400, 380, 0); //w == 0 indications a directional light
	directionalLight.intensities = glm::vec3(0.4,0.3,0.1); //weak yellowish light
	directionalLight.ambientCoefficient = 0.06f;

	gLights.push_back(spotlight);
	gLights.push_back(directionalLight);

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
}

void renderer::setVertexData( float *vertexData, uint_t numvertices )
{
	// set all the elements of gWoodenCrate
	gMap.shaders = LoadShaders("vertex-shader.txt", "fragment-shader.txt");
	gMap.drawType = GL_TRIANGLES;
	gMap.drawStart = 0;
	gMap.drawCount = numvertices;
	gMap.texture = LoadTexture("wooden-crate.jpg");
	gMap.shininess = 80.0;
	gMap.specularColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glGenBuffers(1, &gMap.vbo);
	glGenVertexArrays(1, &gMap.vao);

	// bind the VAO
	glBindVertexArray(gMap.vao);

	// bind the VBO
	glBindBuffer(GL_ARRAY_BUFFER, gMap.vbo);

	// Make a cube out of triangles (two triangles per side)
	glBufferData(GL_ARRAY_BUFFER, numvertices*5*sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

	// connect the xyz to the "vert" attribute of the vertex shader
	glEnableVertexAttribArray(gMap.shaders->attrib("vert"));
	glVertexAttribPointer(gMap.shaders->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), NULL);

	// connect the uv coords to the "vertTexCoord" attribute of the vertex shader
	glEnableVertexAttribArray(gMap.shaders->attrib("vertTexCoord"));
	glVertexAttribPointer(gMap.shaders->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  8*sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

	// connect the normal to the "vertNormal" attribute of the vertex shader
	glEnableVertexAttribArray(gMap.shaders->attrib("vertNormal"));
	glVertexAttribPointer(gMap.shaders->attrib("vertNormal"), 3, GL_FLOAT, GL_TRUE,  8*sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

	// unbind the VAO
	glBindVertexArray(0);
}

// convenience function that returns a translation matrix
glm::mat4 translate(GLfloat x, GLfloat y, GLfloat z) {
	return glm::translate(glm::mat4(), glm::vec3(x,y,z));
}


// convenience function that returns a scaling matrix
glm::mat4 scale(GLfloat x, GLfloat y, GLfloat z) {
	return glm::scale(glm::mat4(), glm::vec3(x,y,z));
}


//create all the `instance` structs for the 3D scene, and add them to `gInstances`
void renderer::CreateInstances()
{
	ModelInstance dot;
	dot.asset = &gMap;
	dot.transform = glm::mat4();
	gInstances.push_back(dot);
}

template <typename T>
void SetLightUniform(tdogl::Program* shaders, const char* propertyName, size_t lightIndex, const T& value) {
    std::ostringstream ss;
    ss << "allLights[" << lightIndex << "]." << propertyName;
    std::string uniformName = ss.str();

    shaders->setUniform(uniformName.c_str(), value);
}

//renders a single `ModelInstance`
void renderer::RenderInstance(const ModelInstance& inst)
{
	ModelAsset* asset = inst.asset;
	tdogl::Program* shaders = asset->shaders;

	//bind the shaders
	shaders->use();

	//set the shader uniforms
	shaders->setUniform("camera", gCamera.matrix());
	shaders->setUniform("model", inst.transform);
	shaders->setUniform("materialTex", 0); //set to 0 because the texture will be bound to GL_TEXTURE0
	shaders->setUniform("materialShininess", asset->shininess);
	shaders->setUniform("materialSpecularColor", asset->specularColor);
	shaders->setUniform("cameraPosition", gCamera.position());
	shaders->setUniform("numLights", (int)gLights.size());

	for(size_t i = 0; i < gLights.size(); ++i){
		SetLightUniform(shaders, "position", i, gLights[i].position);
		SetLightUniform(shaders, "intensities", i, gLights[i].intensities);
		SetLightUniform(shaders, "attenuation", i, gLights[i].attenuation);
		SetLightUniform(shaders, "ambientCoefficient", i, gLights[i].ambientCoefficient);
		SetLightUniform(shaders, "coneAngle", i, gLights[i].coneAngle);
		SetLightUniform(shaders, "coneDirection", i, gLights[i].coneDirection);
	}

	//bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, asset->texture->object());

	//bind VAO and draw
	glBindVertexArray(asset->vao);
	glDrawArrays(asset->drawType, asset->drawStart, asset->drawCount);

	//unbind everything
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	shaders->stopUsing();
}

// draws a single frame
void renderer::Render()
{
	// clear everything
	glClearColor(0, 0, 0, 1); // black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// render all the instances
	std::list<ModelInstance>::const_iterator it;
	for(it = gInstances.begin(); it != gInstances.end(); ++it){
		RenderInstance(*it);
	}

	// swap the display buffers (displays what was just drawn)
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
		Render();

		// check for errors
		GLenum error = glGetError();
		if(error != GL_NO_ERROR) {
			con_printf( "OpenGL Error %i (%s)\n",
				error, glewGetErrorString(error) );
		}
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
	//delete gProgram;
	//delete gTexture;
	//shaders.pop_back();
	//shaders.pop_back();

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
