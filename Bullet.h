//
//  Ship.h
//  Asteroids
//
//  Created by Brian Summa on 10/2/2018.
//
//

#ifndef __Asteroids__Bullet__
#define __Asteroids__Bullet__

#include "common.h"

class Bullet{

  //Placeholders so everything compiles.  Customize for your ship
	vec2 bullet_vert[2];
	vec3 bullet_color[2];

  //Record of the bullet's state
  struct {
    vec2 cur_location;   //Current position of the center
    float angle;         //Rotation angle
    vec2 pointing;       //Vector pointing to the front of the bullet
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

  Bullet();

  void update_state();
  
  void gl_init(GLuint &bulletvao);
  
  void draw(mat4 proj);
  
};


#endif /* defined(__Asteroids__Bullet__) */
