#pragma once

float cube_verticies[] = {
	// left face
	-0.5f,  0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f, -0.5f,  0.5f,
	-0.5f,  0.5f,  0.5f,
	-0.5f,  0.5f, -0.5f,
		  
	// front face
	-0.5f, -0.5f, 0.5f, 1.0f / 4.0f, 1.0f / 3.0f,
	0.5f, -0.5f, 0.5f, 2.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, 0.5f, 1.0f / 4.0f, 2.0f / 3.0f,
	0.5f, -0.5f, 0.5f, 2.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, 0.5f, 1.0f / 4.0f, 2.0f / 3.0f,
	0.5f,  0.5f, 0.5f, 2.0f / 4.0f, 2.0f / 3.0f,

	// right face
	0.5f,  0.5f, -0.5f, 3.0f / 4.0f, 2.0f / 3.0f, // top right
	0.5f, -0.5f, -0.5f, 3.0f / 4.0f, 1.0f / 3.0f, // bot right
	0.5f, -0.5f,  0.5f, 2.0f / 4.0f, 1.0f / 3.0f, // bot left
	0.5f, -0.5f,  0.5f, 2.0f / 4.0f, 1.0f / 3.0f, // bot left
	0.5f,  0.5f,  0.5f, 2.0f / 4.0f, 2.0f / 3.0f, // top left
	0.5f,  0.5f, -0.5f, 3.0f / 4.0f, 2.0f / 3.0f, // top right

	// back face
	-0.5f, -0.5f, -0.5f, 4.0f / 4.0f, 1.0f / 3.0f,
	0.5f, -0.5f, -0.5f, 3.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, -0.5f, 4.0f / 4.0f, 2.0f / 3.0f,
	0.5f, -0.5f, -0.5f, 3.0f / 4.0f, 1.0f / 3.0f,
	-0.5f,  0.5f, -0.5f, 4.0f / 4.0f, 2.0f / 3.0f,
	0.5f,  0.5f, -0.5f, 3.0f / 4.0f, 2.0f / 3.0f,

	// top face
	-0.5f, 0.5f, -0.5f, 1.0f / 4.0f, 3.0f / 3.0f, // top left
	-0.5f, 0.5f, 0.5f,  1.0f / 4.0f, 2.0f / 3.0f, // bot left
	0.5f, 0.5f, 0.5f,   2.0f / 4.0f, 2.0f / 3.0f, // bot right
	0.5f, 0.5f, 0.5f,   2.0f / 4.0f, 2.0f / 3.0f, // bot right
	0.5f, 0.5f, -0.5f,  2.0f / 4.0f, 3.0f / 3.0f, // top right
	-0.5f, 0.5f, -0.5f, 1.0f / 4.0f, 3.0f / 3.0f, // top left

	// bot face
	-0.5f, -0.5f, -0.5f, 1.0f / 4.0f, 1.0f / 3.0f, // top left 
	-0.5f, -0.5f, 0.5f,  1.0f / 4.0f, 0.0f / 3.0f, // bot left 
	0.5f,  -0.5f, 0.5f,  2.0f / 4.0f, 0.0f / 3.0f, // bot right
	0.5f,  -0.5f, 0.5f,  2.0f / 4.0f, 0.0f / 3.0f, // bot right
	0.5f,  -0.5f, -0.5f, 2.0f / 4.0f, 1.0f / 3.0f, // top right
	-0.5f, -0.5f, -0.5f, 1.0f / 4.0f, 1.0f / 3.0f, // top left 
		   
};
