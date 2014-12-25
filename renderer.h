/*
 * renderer.h - renderer subsystem
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

#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "tdogl/Program.h"
#include "tdogl/Texture.h"
#include "tdogl/Camera.h"

/*
 Represents a textured geometry asset

 Contains everything necessary to draw arbitrary geometry with a single texture:

  - shaders
  - a texture
  - a VBO
  - a VAO
  - the parameters to glDrawArrays (drawType, drawStart, drawCount)
 */
struct ModelAsset {
	tdogl::Program* shaders;
	tdogl::Texture* texture;
	GLuint vbo;
	GLuint vao;
	GLenum drawType;
	GLint drawStart;
	GLint drawCount;
	GLfloat shininess;
	glm::vec3 specularColor;

	ModelAsset() :
		shaders(NULL),
		texture(NULL),
		vbo(0),
		vao(0),
		drawType(GL_TRIANGLES),
		drawStart(0),
		drawCount(0),
		shininess(0.0f),
		specularColor(1.0f, 1.0f, 1.0f)
	{}
};

/*
 Represents an instance of an `ModelAsset`

 Contains a pointer to the asset, and a model transformation matrix to be used when drawing.
 */
struct ModelInstance {
	ModelAsset* asset;
	glm::mat4 transform;

	ModelInstance() :
		asset(NULL),
		transform()
	{}
};

/*
 Represents a point light
 */
struct Light {
	glm::vec4 position;
	glm::vec3 intensities; //a.k.a. the color of the light
	float attenuation;
	float ambientCoefficient;
	float coneAngle;
	glm::vec3 coneDirection;
};

class renderer
{
public:
	void	init( const char *name );
	void	createwindow( const char *name );
	void	drawFrame( void );
	void	renderloop( void );
	void	shutdown( void );
	void	update(float secondsElapsed);
	void	setVertexData( float *vertexdata, uint_t numvertices );
	// constructor
	renderer( const char *name=NULL )
	{
		if (name) init( name );
		else init( "OpenGL window" );
	}
	~renderer()
	{
		shutdown();
	}
protected:
	void	CreateInstances();
	void	RenderInstance(const ModelInstance& inst);
	void	Render();
	// vars
	GLFWwindow* mainwindow;

	tdogl::Camera gCamera;
	//std::vector<tdogl::Shader> shaders;
	ModelAsset gMap;
	std::list<ModelInstance> gInstances;
	std::vector<Light> gLights;
};

#endif //RENDERER_H
