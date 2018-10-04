//
//  Ship.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "common.h"
#include "Ship.h"
#include "utils/Angel.h"
using namespace Angel;

//Ship constructor
Ship::Ship(){
	state.cur_location = vec2(0, 0);
	state.angle = 0;
	state.pointing = vec2(0, -1);
	state.velocity = vec2(0, 0);
};

//Called everytime an animation tick happens
void Ship::update_state(){
	float dt = 0.033; vec2 a;
	if (state.thruster_on) {
		a = _ACC * state.pointing; //thursts forward
	}

	//if a's x or y is less than -3 or greater than 3, clamp it to 3 (which is _ACC)
	if (a.x > 3) { a.x = 3; }
	if (a.x < -3) { a.x = -3; }
	if(a.y > 3) { a.y = 3; }
	if(a.y < -3) { a.y = -3; }

	//clamp speed within -10 and 10
	if (state.velocity.x > 10) {state.velocity.x = 10;}
	if (state.velocity.x < -10) {state.velocity.x = -10;}
	if (state.velocity.y > 10) {state.velocity.y = 10;}
	if (state.velocity.y < -10) {state.velocity.y = 10;}

	state.velocity = state.velocity + (a * dt); //update velocity according to current accerlataion
	state.velocity *= _DAMPING; //damping by 0.40
	state.cur_location = state.cur_location + (state.velocity * dt); //update current position according to current velocity

	//wrapping around the ship and preventing to get out of the boundary
	if (state.cur_location.x > 1.0) { state.cur_location.x = -1; }
	if (state.cur_location.x < -1.0) { state.cur_location.x = 1; }
	if (state.cur_location.y > 1.0) { state.cur_location.y = -1; }
	if (state.cur_location.y < -1.0) { state.cur_location.y = 1; }
}

//Initialize the gl state and variables
void Ship::gl_init(GLuint &shipvao){
  //Ship
  //!!!!!!!!Populate ship_vert and ship_color
	/*ship_vert[0] = vec2(0, 0);
	ship_vert[1] = vec2(0, 0.02);
	ship_vert[2] = vec2(0.03, 0.03);
	ship_vert[3] = vec2(0, 0);
	ship_vert[4] = vec2(0, 0.02);
	ship_vert[5] = vec2(-0.03, 0.03);
	ship_vert[6] = vec2(0, 0);
	ship_vert[7] = vec2(0, -0.1);
	ship_vert[8] = vec2(-0.03, 0.03);
	ship_vert[9] = vec2(0, 0);
	ship_vert[10] = vec2(0, -0.1);
	ship_vert[11] = vec2(0.03, 0.03);*/

	ship_vert[0] = vec2(0, 0);
	ship_vert[1] = vec2(0, -0.06);
	ship_vert[2] = vec2(0.05, 0.075);
	ship_vert[3] = vec2(0, 0);
	ship_vert[4] = vec2(0, -0.06);
	ship_vert[5] = vec2(-0.05, 0.075);
	ship_vert[6] = vec2(0, 0);
	ship_vert[7] = vec2(0, 0.06);
	ship_vert[8] = vec2(-0.05, 0.075);
	ship_vert[9] = vec2(0, 0);
	ship_vert[10] = vec2(0, 0.06);
	ship_vert[11] = vec2(0.05, 0.075);

	//Ship's flame
	ship_vert[12] = vec2(0, 0.06);
	ship_vert[13] = vec2(-0.025, 0.080);
	ship_vert[14] = vec2(0.025, 0.080);
	ship_vert[15] = vec2(-0.025, 0.080);
	ship_vert[16] = vec2(0.025, 0.080);
	ship_vert[17] = vec2(0, 0.11);

	for(int i = 0; i < 12; i++){ ship_color[i] = vec3(1, 0.2, 0.2); }
	for (int i = 0; i < 6; i++) { ship_color[i+12] = vec3(0, 1, 0); };

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
  glBufferData( GL_ARRAY_BUFFER, sizeof(ship_vert) + sizeof(ship_color), NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(ship_vert), ship_vert );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, sizeof(ship_vert), sizeof(ship_color), ship_color );
  
  glEnableVertexAttribArray(GLvars.vpos_location);
  glEnableVertexAttribArray(GLvars.vcolor_location );
  
  glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(ship_vert)) );
  
  glBindVertexArray(0);
  //shipvao = GLvars.vao;

}

//Draw a ship
void Ship::draw(mat4 proj){
  
  glUseProgram( GLvars.program );
  glBindVertexArray( GLvars.vao );
 
  proj = proj * Translate(state.cur_location.x, state.cur_location.y, 0); //translate position
  proj = proj * RotateZ(state.angle); //rotate ship by current angle
  
  //If you have a modelview matrix, pass it with proj
  glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, proj );
  
  glDrawArrays(GL_TRIANGLES, 0, 12);

  if(state.thruster_on){
	glDrawArrays(GL_TRIANGLES, 12, 6);
  }
  
  glBindVertexArray(0);
  glUseProgram(0);

}
