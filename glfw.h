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
