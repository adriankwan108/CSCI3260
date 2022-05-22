#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include <iostream>
#include <fstream>

using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
GLuint vao[3];
GLuint target2_vao;
GLuint tree_vao;
GLuint leaf_vao;

int x_press_num = 0;
int y_press_num = 0;
int rotate_press = 0;
int scale_num = 0;
float delta = 0.1f;
int wheel = 0;


int flag=0;
int loc;


bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int x, int y)
{
	//TODO:
	if (key == 'a')
	{
		//loc = glGetUniformLocation(programID, "figure");
		//glUniform1i(loc, 1);
		x_press_num += 1;
	}

	if (key == 'd')
	{
		x_press_num -= 1;
	}

	if (key == 'w')
	{
		y_press_num -= 1;
	}

	if (key == 's')
	{
		y_press_num += 1;
	}

	if (key == 'q')
	{
		rotate_press -= 1;
	}

	if (key == 'e')
	{
		rotate_press += 1;
	}

	if (key == 'p')
	{
		flag = 1;
	}

	if (key == 't')
	{
		flag = 0;
	}

	if (key == '+')
	{
		scale_num += 1;
	}

	if (key == '-')
	{
		scale_num -= 1;
	}

	if (key == '+')
	{
		scale_num += 1;
	}

	if (key == '-')
	{
		scale_num -= 1;
	}

	if ( key == 'k')
	{
		wheel += 1;
	}
	if (key == 'm')
	{
		wheel -= 1;
	}
}

void sendDataToOpenGL()       //creation
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	
	//create a button with colour and indexing
	const GLfloat button[] =
	{
		-0.8f, 0.01f,+0.9f,    //bottom right
		-0.8f, 0.01f,+0.8f,	//top right
		-0.9f, 0.01f,+0.8f,	//top left
		-0.9f, 0.01f,+0.9f,	//bottom left
	};
	unsigned int button_indices[] = { 0,1,3,
								  1,2,3 };
	const GLfloat button_color[] =
	{
		1.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,0.0f,1.0f,
		0.5f,0.5f,0.5f
	};
	
	//generate space for vao
	glGenVertexArrays(1,&vao[0]);
	{
	//generate vbo and ebo
	GLuint button_vbo;
	GLuint button_ebo;
	GLuint button_color_vbo;
	glGenBuffers(1, &button_vbo);
	glGenBuffers(1, &button_ebo);
	glGenBuffers(1, &button_color_vbo);

		//binding
		glBindVertexArray(vao[0]);   //bind vao first, then bind and set vertex buffer
	
		glBindBuffer(GL_ARRAY_BUFFER, button_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(button), button, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, button_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(button_indices), button_indices, GL_STATIC_DRAW);

		//Configure the vertex attributes pointers
		glEnableVertexAttribArray(0);    //enable attribute index 0 as being used
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//colour
		glBindBuffer(GL_ARRAY_BUFFER, button_color_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(button_color), button_color, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//close buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0); //close vao
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}

	//create the 2d base with indexing
	const GLfloat ground[]={
		+1.0f, 0.0f, +1.0f, //near right		
		+1.0f, 0.0f, -1.0f,	//far right
		-1.0f, 0.0f, +1.0f,	//near left
		-1.0f, 0.0f, -1.0f,	//far left		
	};
	
	unsigned int ground_indice[] = {
		0,1,2,
		1,2,3
	};

	const GLfloat ground_color[] = {
		1.0f,1.0f,1.0f,
		0.0f,0.5f,0.5f,
		1.0f,1.0f,1.0f,
		0.0f,0.5f,0.5f,  
	};

	glGenVertexArrays(1,&vao[1]);
	{ //bind
		GLuint ground_vbo;
		GLuint ground_ebo;
		GLuint ground_color_vbo;

		glGenBuffers(1, &ground_vbo);
		glGenBuffers(1, &ground_ebo);
		glGenBuffers(1, &ground_color_vbo);

		glBindVertexArray(vao[1]);
		glBindBuffer(GL_ARRAY_BUFFER, ground_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ground_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indice), ground_indice, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, ground_color_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ground_color), ground_color, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//close buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0); //close vao
	}

	//draw 3D target box with indexing
	const GLfloat target[] =
	{
		 1.0f,  0.1f, -0.95f,
		 1.0f, 0.0f, -0.95f,
		 0.9f, 0.0f, -0.95f,
		 0.9f,  0.1f, -0.95f,

		 1.0f,  0.1f, -0.85f,
		 1.0f, 0.0f, -0.85f,
		 0.9f, 0.0f, -0.85f,
		 0.9f,  0.1f, -0.85f,
	};

	unsigned int target_indice[] =
	{
		0,1,2,
		0,2,3,

		0,4,3,
		4,3,7,

		4,5,6,
		4,6,7,

		2,3,7,
		2,6,7,

		1,5,4,
		1,4,0,

		1,2,6,
		1,6,5
	};

	const GLfloat target_color[] = {
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,

		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,
	};
	glGenVertexArrays(1, &vao[2]);
	{
		GLuint target_vbo;
		GLuint target_ebo;
		GLuint target_color_vbo;

		glGenBuffers(1, &target_vbo);
		glGenBuffers(1, &target_ebo);
		glGenBuffers(1, &target_color_vbo);

		glBindVertexArray(vao[2]);
		glBindBuffer(GL_ARRAY_BUFFER, target_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(target), target, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, target_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(target_indice), target_indice, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, target_color_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(target_color), target_color, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//close buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0); //close vao
	}
	

	//draw 3D target box without indexing
	const GLfloat target2[] =          
	{
		-0.8f, 0.0f,-0.8f, // triangle 1 : begin  //1
		-0.8f, 0.0f,-0.7f,					     //2   
		-0.8f, 0.1f,-0.7f, // triangle 1 : end    //3  

		-0.7f, 0.0f,-0.8f, // triangle 2 : begin  //4   
		-0.8f, 0.0f,-0.8f,                         //1  
		-0.8f, 0.1f,-0.8f, // triangle 2 : end    //5   

		-0.7f, 0.0f,-0.7f,		//6		
		-0.8f, 0.0f,-0.8f,		//1    
		-0.7f, 0.0f,-0.8f,		//4 

		-0.7f, 0.1f,-0.8f,      //7    
		-0.7f, 0.0f,-0.8f,		//4
		-0.8f, 0.1f,-0.8f,		//5

		-0.8f, 0.0f,-0.8f,		//1
		-0.8f, 0.1f,-0.7f,		//3
		-0.8f, 0.1f,-0.8f,		//5

		-0.7f, 0.0f,-0.7f,		//6
		-0.8f, 0.0f,-0.7f,		//2
		-0.8f, 0.0f,-0.8f,		//1
			
		-0.8f, 0.1f,-0.7f,		//3
		-0.8f, 0.0f,-0.7f,		//2
		-0.7f, 0.0f,-0.7f,		//6

		-0.7f, 0.1f,-0.7f,		//8
		-0.7f, 0.0f,-0.8f,		//4
		-0.7f, 0.1f,-0.8f, 		//7

		-0.7f, 0.0f,-0.8f,		//4
		-0.7f, 0.1f,-0.7f,		//8
		-0.7f, 0.0f,-0.7f,		//6

		-0.7f, 0.1f,-0.7f,		//8
		-0.7f, 0.1f,-0.8f,		//7
		-0.8f, 0.1f,-0.8f,		//5

		-0.7f, 0.1f,-0.7f,		//8
		-0.8f, 0.1f,-0.8f,		//5
		-0.8f, 0.1f,-0.7f,		//3

		-0.7f, 0.1f,-0.7f,		//8
		-0.8f, 0.1f,-0.7f,		//3
		-0.7f, 0.0f,-0.7f,		//6
	};

	const GLfloat target2_color[] = {
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
		0.822f,  0.569f,  0.201f,
		0.435f,  0.602f,  0.223f,
		0.310f,  0.747f,  0.185f,
		0.597f,  0.770f,  0.761f,
		0.559f,  0.436f,  0.730f,
		0.359f,  0.583f,  0.152f,
		0.483f,  0.596f,  0.789f,
		0.559f,  0.861f,  0.639f,
		0.195f,  0.548f,  0.859f,
		0.014f,  0.184f,  0.576f,
		0.771f,  0.328f,  0.970f,
		0.406f,  0.615f,  0.116f,
		0.676f,  0.977f,  0.133f,
		0.971f,  0.572f,  0.833f,
		0.140f,  0.616f,  0.489f,
		0.997f,  0.513f,  0.064f,
		0.945f,  0.719f,  0.592f,
		0.543f,  0.021f,  0.978f,
		0.279f,  0.317f,  0.505f,
		0.167f,  0.620f,  0.077f,
		0.347f,  0.857f,  0.137f,
		0.055f,  0.953f,  0.042f,
		0.714f,  0.505f,  0.345f,
		0.783f,  0.290f,  0.734f,
		0.722f,  0.645f,  0.174f,
		0.302f,  0.455f,  0.848f,
		0.225f,  0.587f,  0.040f,
		0.517f,  0.713f,  0.338f,
		0.053f,  0.959f,  0.120f,
		0.393f,  0.621f,  0.362f,
		0.673f,  0.211f,  0.457f,
		0.820f,  0.883f,  0.371f,
		0.982f,  0.099f,  0.879f
	};
	glGenVertexArrays(1, &target2_vao);
	{
		GLuint target2_vbo;
		GLuint target2_color_vbo;

		glGenBuffers(1, &target2_vbo);
		glGenBuffers(1, &target2_color_vbo);

		glBindVertexArray(target2_vao);
		glBindBuffer(GL_ARRAY_BUFFER, target2_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(target2), target2, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, target2_color_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(target2_color), target2_color, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//close buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0); //close vao
	}

	const GLfloat tree_stem[] =
	{
		 0.2f,  0.8f, -0.2f,
		 0.2f, 0.0f, -0.2f,
		 -0.2f, 0.0f, -0.2f,
		 -0.2f,  0.8f, -0.2f,

		 0.2f,  0.8f, +0.2f,
		 0.2f, 0.0f, +0.2f,
		 -0.2f, 0.0f, +0.2f,
		 -0.2f,  0.8f, +0.2f,
	};

	unsigned int tree_indice[] =
	{
		0,1,2,
		0,2,3,

		0,4,3,
		4,3,7,

		4,5,6,
		4,6,7,

		2,3,7,
		2,6,7,

		1,5,4,
		1,4,0,

		1,2,6,
		1,6,5
	};

	const GLfloat tree_color[] = {
		0.5f,0.35f,0.05f,
		0.5f,0.35f,0.05f,
		0.5f,0.35f,0.05f,
		0.5f,0.35f,0.05f,

		0.5f,0.35f,0.05f,
		0.5f,0.35f,0.05f,
		0.5f,0.35f,0.05f,
		0.5f,0.35f,0.05f,
	};

	glGenVertexArrays(1, &tree_vao);
	{
		GLuint tree_vbo;
		GLuint tree_ebo;
		GLuint tree_color_vbo;

		glGenBuffers(1, &tree_vbo);
		glGenBuffers(1, &tree_ebo);
		glGenBuffers(1, &tree_color_vbo);

		glBindVertexArray(tree_vao);
		glBindBuffer(GL_ARRAY_BUFFER, tree_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tree_stem), tree_stem, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tree_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tree_indice), tree_indice, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, tree_color_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(tree_color), tree_color, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//close buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0); //close vao
	}

	const GLfloat leaf[] =
	{
		 0.48f,  1.0f, -0.5f,
		 0.5f, 0.8f, -0.5f,
		 -0.5f, 0.8f, -0.5f,
		 -0.4f,  1.0f, -0.5f,

		 0.4f,  1.0f, +0.4f,
		 0.5f, 0.8f, +0.3f,
		 -0.5f, 0.8f, +0.3f,
		 -0.4f, 1.0f, +0.4f,
	};

	unsigned int leaf_indice[] =
	{
		0,1,2,
		0,2,3,

		0,4,3,
		4,3,7,

		4,5,6,
		4,6,7,

		2,3,7,
		2,6,7,

		1,5,4,
		1,4,0,

		1,2,6,
		1,6,5
	};

	const GLfloat leaf_color[] = {
		0.0f,0.35f,0.0f,
		0.0f,0.35f,0.0f,
		0.0f,0.35f,0.0f,
		0.0f,0.35f,0.0f,

		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};

	glGenVertexArrays(1, &leaf_vao);
	{
		GLuint leaf_vbo;
		GLuint leaf_ebo;
		GLuint leaf_color_vbo;

		glGenBuffers(1, &leaf_vbo);
		glGenBuffers(1, &leaf_ebo);
		glGenBuffers(1, &leaf_color_vbo);

		glBindVertexArray(leaf_vao);
		glBindBuffer(GL_ARRAY_BUFFER, leaf_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(leaf), leaf, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, leaf_ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(leaf_indice), leaf_indice, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, leaf_color_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(leaf_color), leaf_color, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		//close buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0); //close vao
	}


}

void paintGL(void)    //transformation
{
	//TODO:
	//render your objects and control the transformation here

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); //background colour
	//glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);  //depth test
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//top view OR perspective mode
	if (flag == 0)
	{
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0, 10.0, 0.01), // top view
			glm::vec3(0, 0, 0),		// and looks at the origin
			glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
		);					//tutorial notes are sucks				

		glm::mat4 Model = glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 mvp = Projection * View * Model;

		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	}
	else if (flag == 1)
	{
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		glm::mat4 View = glm::lookAt(
			glm::vec3(0.0, delta*wheel + 2.0, 2.5), // Camera is at (0,2,2.5), in World Space
			glm::vec3(0, 0, 0),		// and looks at the origin
			glm::vec3(0, 1, 0)		// Head is up (set to 0,-1,0 to look upside-down)
		);					//tutorial notes are sucks				

		glm::mat4 Model = glm::mat4(1.0f);
		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 mvp = Projection * View * Model;

		GLuint MatrixID = glGetUniformLocation(programID, "MVP");
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	}

	glm::mat4 ModelMatrix = glm::mat4(1.0f);
	unsigned ModelLoc = glGetUniformLocation(programID, "ModelMatrix");
	
	ModelMatrix = glm::translate(ModelMatrix, glm::vec3(x_press_num*delta+0.f, 0.f, y_press_num*delta+0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(1.f, 0.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians((float)(0.9 * rotate_press)+0.f), glm::vec3(0.f, 1.f, 0.f));
	ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3((float)(delta*scale_num)+1.f));
	glUniformMatrix4fv(glGetUniformLocation(programID, "ModelMatrix"), 1, GL_FALSE, &ModelMatrix[0][0]);
	
	glBindVertexArray(vao[0]);  //button
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindVertexArray(0);

	glBindVertexArray(vao[1]);   //ground
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindVertexArray(0);
	
		//target
		//gl get uniform location of Transform Matrix
		//glm::mat4 ModelMatrix = glm::mat4(1.0f);
		
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5.f), glm::vec3(1.f, 0.f, 0.f));
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(2.f), glm::vec3(0.f, 1.f, 0.f));
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		//unsigned ModelLoc = glGetUniformLocation(programID, "ModelMatrix");
		glBindVertexArray(vao[2]);   

		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(5.f), glm::vec3(1.f, 0.f, 0.f));
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(2.f), glm::vec3(0.f, 1.f, 0.f));
		//ModelMatrix = glm::rotate(ModelMatrix, glm::radians(0.f), glm::vec3(0.f, 0.f, 1.f));
		
		//glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, &ModelMatrix[0][0]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	

	   //target 2
	{	glBindVertexArray(target2_vao);  //target 2
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	//should have copy target 2 randomly if the translation could based on each object
	/*
	glBindVertexArray(target2_vao);
	for (unsigned int i = 0; i <5 ; i++)
	{
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3( ((GLfloat)(rand()%1)), ((GLfloat)(rand() % 1)), 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(ModelLoc, 1, GL_FALSE, &ModelMatrix[0][0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}
	*/

	glBindVertexArray(tree_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindVertexArray(leaf_vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glFlush();
	glutPostRedisplay();
}


void initializedGL(void) //run only once
{
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char* argv[])
{
	/*Initialization*/
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Assignment 1");
	glewInit();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	
	initializedGL();

	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();
	
}