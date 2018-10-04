//
//  Asteroid.h
//  Asteroids
//
//

#ifndef __Asteroids__Asteroid__
#define __Asteroids__Asteroid__

#include "common.h"


#define _MAX_SPEED 1
#define _DAMPING 0.98
#define _ACC 1
#define _ROT 5

class Asteroid {

	//Placeholders so everything compiles.  Customize for your asteroid
	vec2 asteroid_vert[99];
	vec3 asteroid_color[99];

	//Record of the asteroid's state
	struct {
		vec2 cur_location;   //Current position of the center
		float angle;         //Rotation angle
		vec2 pointing;       //Vector pointing to the front of the ship
							 //This function will be helpful to keep track of the direction the ship
							 //is pointing
		mat2 RotateZ2D(const GLfloat theta) {
			GLfloat angle = DegreesToRadians * theta;
			mat2 c;
			c[0][0] = c[1][1] = cos(angle);
			c[1][0] = sin(angle);
			c[0][1] = -c[1][0];
			return c;
		}
		vec2 velocity;       //Velocity
	} state;

	//OpenGL variables for a ship
	struct {
		GLuint vao;           //Vertex array object
		GLuint program;       //shader program
		GLuint buffer;        //Vertex buffer objects
		GLuint vertex_shader, fragment_shader;  //Shaders
		GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
		GLint M_location;     //Reference to matrix in shader
	} GLvars;


public:


	Asteroid();

	void update_state();

	void gl_init(GLuint &asteroidvao);

	void draw(mat4 proj);

};


#endif /* defined(__Asteroids__Asteroid__) */
