//
//  Bullet.cpp
//  Asteroids
//
//  Created by Brian Summa on 10/2/2018.
//
//

#include "common.h"
#include "Bullet.h"
#include "utils/Angel.h"
using namespace Angel;

//Ship constructor
Bullet::Bullet(){
	state.cur_location = vec2(0, 0);
	state.angle = 0;
	state.pointing = vec2(0, -1);
	state.velocity = vec2(0, 0);
};

//Called everytime an animation tick happens
void Bullet::update_state(){
	float dt = 0.033;
	state.cur_location = state.cur_location + (state.velocity * dt); //update current position according to current velocity
}

//Initialize the gl state and variables
void Bullet::gl_init(GLuint &bulletvao){
  //Bullet
  //!!!!!!!!Populate bullet_vert and bullet_color
	bullet_vert[0] = vec2(0, 0);
	bullet_vert[1] = vec2(0, 0);

	for(int i = 0; i < 2; i++){ bullet_color[i] = vec3(0, 1, 1); }

  std::string vshader = shader_path + "vshader_Ship.glsl";
  std::string fshader = shader_path + "fshader_Ship.glsl";
  
  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());
  
  GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(GLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(GLvars.vertex_shader);
  check_shader_compilation(vshader, GLvars.vertex_shader);
  
  GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(GLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(GLvars.fragment_shader);
  check_shader_compilation(fshader, GLvars.fragment_shader);
  
  GLvars.program = glCreateProgram();
  glAttachShader(GLvars.program, GLvars.vertex_shader);
  glAttachShader(GLvars.program, GLvars.fragment_shader);
  
  glLinkProgram(GLvars.program);
  check_program_link(GLvars.program);
  
  glBindFragDataLocation(GLvars.program, 0, "fragColor");
  
  GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
  GLvars.vcolor_location = glGetAttribLocation(GLvars.program, "vColor" );
  GLvars.M_location = glGetUniformLocation(GLvars.program, "M" );
  
  // Create a vertex array object
  glGenVertexArrays( 1, &GLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GLvars.vao );
  
  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GLvars.buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );
  
  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, sizeof(bullet_vert) + sizeof(bullet_color), NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(bullet_vert), bullet_vert );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(bullet_vert), sizeof(bullet_color), bullet_color );
  
  glEnableVertexAttribArray(GLvars.vpos_location);
  glEnableVertexAttribArray(GLvars.vcolor_location );
  
  glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(bullet_vert)) );
  
  glBindVertexArray(0);
  //shipvao = GLvars.vao;

}

//Draw a ship
void Bullet::draw(mat4 proj){
  
  glUseProgram( GLvars.program );
  glBindVertexArray( GLvars.vao );
 
  proj = proj * Translate(state.cur_location.x, state.cur_location.y, 0); //translate position
  proj = proj * RotateZ(state.angle); //rotate ship by current angle
  
  //If you have a modelview matrix, pass it with proj
  glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj );
  
  glDrawArrays(GL_POINT, 0, 2);
  
  glBindVertexArray(0);
  glUseProgram(0);

}
