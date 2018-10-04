//
//  Ship.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "common.h"
#include "Asteroid.h"
#include "utils/Angel.h"
using namespace Angel;

//Asteroid constructor
Asteroid::Asteroid() {
	state.cur_location = vec2(0, 0);
	state.angle = 0;
	srand(time(0)); //generate more random numbers
	float x = rand() % 16 / 100.0; 
	float y = rand() % 16 / 100.0;
	switch (rand() % 4) {
		case 0: //generates direction in quad 3
			state.velocity = vec2(x, y);
			break;
		case 1: //generates direction in quad 1
			state.velocity = vec2(x, -y);
			break;
		case 2: //generates direction in quad 2
			state.velocity = vec2(-x, y);
			break;
		case 3: //generates direction in quad 4
			state.velocity = vec2(-x, -y);
			break;
	}
};

//Called everytime an animation tick happens
void Asteroid::update_state() {
	float dt = 0.033; //one frame per sec

	//rotating asteroid
	state.angle += 0.5;

	//updating location
	state.cur_location = state.cur_location + (state.velocity * dt);

	//wrapping around the asteriod and preventing to get out of the boundary
	if (state.cur_location.x > 1.0) { state.cur_location.x = -1; }
	if (state.cur_location.x < -1.0) { state.cur_location.x = 1; }
	if (state.cur_location.y > 1.0) { state.cur_location.y = -1; }
	if (state.cur_location.y < -1.0) { state.cur_location.y = 1; }

}

//Initialize the gl state and variables
void Asteroid::gl_init(GLuint &asteroidvao) {
	//Asteroid
	//!!!!!!!!Populate asteroid_vert and asteroid_color
	asteroid_vert[0] = vec2(0, 0);
	asteroid_vert[1] = vec2(0, -0.15);
	asteroid_vert[2] = vec2(-0.03, -0.1);
	asteroid_vert[3] = vec2(0, 0);
	asteroid_vert[4] = vec2(0, -0.15);
	asteroid_vert[5] = vec2(0.04, -0.15);
	asteroid_vert[6] = vec2(0, 0);
	asteroid_vert[7] = vec2(0.04, -0.15);
	asteroid_vert[8] = vec2(0.055, -0.14);
	asteroid_vert[9] = vec2(0, 0);
	asteroid_vert[10] = vec2(0.055, -0.14);
	asteroid_vert[11] = vec2(0.085, 0);
	asteroid_vert[12] = vec2(0, 0);
	asteroid_vert[13] = vec2(0.085, 0);
	asteroid_vert[14] = vec2(0.075, 0.09);
	asteroid_vert[15] = vec2(0, 0);
	asteroid_vert[16] = vec2(0.075, 0.09);
	asteroid_vert[17] = vec2(-0.075, 0.105);
	asteroid_vert[18] = vec2(0, 0);
	asteroid_vert[19] = vec2(-0.075, 0.105);
	asteroid_vert[20] = vec2(-0.075, -0.01);
	asteroid_vert[21] = vec2(0, 0);
	asteroid_vert[22] = vec2(-0.075, 0.105);
	asteroid_vert[23] = vec2(-0.01, 0.125);
	asteroid_vert[24] = vec2(0, 0);
	asteroid_vert[25] = vec2(0.075, 0.09);
	asteroid_vert[26] = vec2(.019, 0.112);
	asteroid_vert[27] = vec2(0, 0);
	asteroid_vert[28] = vec2(-0.01, 0.125);
	asteroid_vert[29] = vec2(.019, 0.112);
	asteroid_vert[30] = vec2(0, 0);
	asteroid_vert[31] = vec2(-0.03, -0.1);
	asteroid_vert[32] = vec2(-0.05, -0.08);


	for (int i = 0; i < 99; i++) { asteroid_color[i] = vec3(1, 1, 1); }

	std::string vshader = shader_path + "vshader_Ship.glsl";
	std::string fshader = shader_path + "fshader_Ship.glsl";

	GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
	GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

	GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(GLvars.vertex_shader, 1, (const GLchar**)&vertex_shader_source, NULL);
	glCompileShader(GLvars.vertex_shader);
	check_shader_compilation(vshader, GLvars.vertex_shader);

	GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(GLvars.fragment_shader, 1, (const GLchar**)&fragment_shader_source, NULL);
	glCompileShader(GLvars.fragment_shader);
	check_shader_compilation(fshader, GLvars.fragment_shader);

	GLvars.program = glCreateProgram();
	glAttachShader(GLvars.program, GLvars.vertex_shader);
	glAttachShader(GLvars.program, GLvars.fragment_shader);

	glLinkProgram(GLvars.program);
	check_program_link(GLvars.program);

	glBindFragDataLocation(GLvars.program, 0, "fragColor");

	GLvars.vpos_location = glGetAttribLocation(GLvars.program, "vPos");
	GLvars.vcolor_location = glGetAttribLocation(GLvars.program, "vColor");
	GLvars.M_location = glGetUniformLocation(GLvars.program, "M");

	// Create a vertex array object
	glGenVertexArrays(1, &GLvars.vao);
	//Set GL state to use vertex array object
	glBindVertexArray(GLvars.vao);

	//Generate buffer to hold our vertex data
	glGenBuffers(1, &GLvars.buffer);
	//Set GL state to use this buffer
	glBindBuffer(GL_ARRAY_BUFFER, GLvars.buffer);

	//Create GPU buffer to hold vertices and color
	glBufferData(GL_ARRAY_BUFFER, sizeof(asteroid_vert) + sizeof(asteroid_color), NULL, GL_STATIC_DRAW);
	//First part of array holds vertices
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(asteroid_vert), asteroid_vert);
	//Second part of array hold colors (offset by sizeof(triangle))
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(asteroid_vert), sizeof(asteroid_color), asteroid_color);

	glEnableVertexAttribArray(GLvars.vpos_location);
	glEnableVertexAttribArray(GLvars.vcolor_location);

	glVertexAttribPointer(GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
	glVertexAttribPointer(GLvars.vcolor_location, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(asteroid_vert)));

	glBindVertexArray(0);
	asteroidvao = GLvars.vao;

}

//Draw a asteroid
void Asteroid::draw(mat4 proj) {

	glUseProgram(GLvars.program);
	glBindVertexArray(GLvars.vao);

	proj = proj * Translate(state.cur_location.x, state.cur_location.y, 0);
	proj = proj * RotateZ(state.angle);

	//If you have a modelview matrix, pass it with proj
	glUniformMatrix4fv(GLvars.M_location, 1, GL_TRUE, proj);

	//Draw something
	glDrawArrays(GL_TRIANGLES, 0, 99);

	glBindVertexArray(0);
	glUseProgram(0);

}
