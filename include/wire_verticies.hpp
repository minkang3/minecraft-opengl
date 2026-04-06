const float LINE_THICKNESS = 0.007f;

float wire_verticies[] = {
	// top front
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f - LINE_THICKNESS,
	-0.5f, 0.5f, 0.5f - LINE_THICKNESS,
	0.5f, 0.5f, 0.5f - LINE_THICKNESS,
	0.5f, 0.5f, 0.5f,
		  
	// top left
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, 0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, 0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	// top back
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f + LINE_THICKNESS,
	0.5f, 0.5f, -0.5f + LINE_THICKNESS,
	-0.5f, 0.5f, -0.5f + LINE_THICKNESS,
	-0.5f, 0.5f, -0.5f,

	// top right
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f - LINE_THICKNESS, 0.5f, 0.5f,
	0.5f - LINE_THICKNESS, 0.5f, 0.5f,
	0.5f - LINE_THICKNESS, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,

	// front top
	0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f - LINE_THICKNESS, 0.5f,
	-0.5f, 0.5f - LINE_THICKNESS, 0.5f,
	0.5f, 0.5f - LINE_THICKNESS, 0.5f,
	0.5f, 0.5f, 0.5f,

	// front right
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f - LINE_THICKNESS, -0.5f, 0.5f,
	0.5f - LINE_THICKNESS, -0.5f, 0.5f,
	0.5f - LINE_THICKNESS, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	// front bot
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f + LINE_THICKNESS, 0.5f,
	-0.5f, -0.5f + LINE_THICKNESS, 0.5f,
	0.5f, -0.5f + LINE_THICKNESS, 0.5f,
	0.5f, -0.5f, 0.5f,

	// front left
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,
	-0.5f + LINE_THICKNESS, 0.5f, 0.5f,
	-0.5f + LINE_THICKNESS, 0.5f, 0.5f,
	-0.5f + LINE_THICKNESS, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,

	// bot forward
	0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
	-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
	0.5f, -0.5f, 0.5f - LINE_THICKNESS,
	0.5f, -0.5f, 0.5f,

	// bot left
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,

	// bot back
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f - LINE_THICKNESS,
	0.5f, -0.5f, -0.5f - LINE_THICKNESS,
	-0.5f, -0.5f, -0.5f - LINE_THICKNESS,
	-0.5f, -0.5f, -0.5f,

	// bot right
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f - LINE_THICKNESS, -0.5f, 0.5f,
	0.5f - LINE_THICKNESS, -0.5f, 0.5f,
	0.5f - LINE_THICKNESS, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	// back top
	-0.5f, 0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f - LINE_THICKNESS, -0.5,
	0.5f, 0.5f - LINE_THICKNESS, -0.5,
	-0.5f, 0.5f - LINE_THICKNESS, -0.5,
	-0.5f, 0.5f, -0.5f,

	// back left
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, -0.5f, -0.5f,
	-0.5f + LINE_THICKNESS, 0.5f, -0.5f,
	-0.5f, 0.5f, -0.5f,

	// back bot
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f + LINE_THICKNESS, -0.5f,
	0.5f, -0.5f + LINE_THICKNESS, -0.5f,
	-0.5f, -0.5f + LINE_THICKNESS, -0.5f,
	-0.5f, -0.5f, -0.5f,
		  
	// back right
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f - LINE_THICKNESS, 0.5f, -0.5f,
	0.5f - LINE_THICKNESS, 0.5f, -0.5f,
	0.5f - LINE_THICKNESS, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,

	// left top
	-0.5f, 0.5f, 0.5f,
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f - LINE_THICKNESS, -0.5f,
	-0.5f, 0.5f - LINE_THICKNESS, -0.5f,
	-0.5f, 0.5f - LINE_THICKNESS, 0.5f,
	-0.5f, 0.5f, 0.5f,

	// left bot
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f + LINE_THICKNESS, -0.5f,
	-0.5f, -0.5f + LINE_THICKNESS, -0.5f,
	-0.5f, -0.5f + LINE_THICKNESS, 0.5f,
	-0.5f, -0.5f, 0.5f,

	// left front
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
	-0.5f, -0.5f, 0.5f - LINE_THICKNESS,
	-0.5f, 0.5f, 0.5f - LINE_THICKNESS,
	-0.5f, 0.5f, 0.5f,

	// left back
	-0.5f, 0.5f,  -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f + LINE_THICKNESS,
	-0.5f, -0.5f, -0.5f + LINE_THICKNESS,
	-0.5f, 0.5f,  -0.5f + LINE_THICKNESS,
	-0.5f, 0.5f,  -0.5f,

	// right top
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,
	0.5f, 0.5f - LINE_THICKNESS, -0.5f,
	0.5f, 0.5f - LINE_THICKNESS, -0.5f,
	0.5f, 0.5f - LINE_THICKNESS, 0.5f,
	0.5f, 0.5f, 0.5f,

	// right bot
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f + LINE_THICKNESS, -0.5f,
	0.5f, -0.5f + LINE_THICKNESS, -0.5f,
	0.5f, -0.5f + LINE_THICKNESS, 0.5f,
	0.5f, -0.5f, 0.5f,

	// right front
	0.5f, 0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f - LINE_THICKNESS,
	0.5f, -0.5f, 0.5f - LINE_THICKNESS,
	0.5f, 0.5f, 0.5f - LINE_THICKNESS,
	0.5f, 0.5f, 0.5f,

	// right back
	0.5f, 0.5f,  -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f + LINE_THICKNESS,
	0.5f, -0.5f, -0.5f + LINE_THICKNESS,
	0.5f, 0.5f,  -0.5f + LINE_THICKNESS,
	0.5f, 0.5f,  -0.5f,
};
