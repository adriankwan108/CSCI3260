#version 460

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;

uniform mat4 MVP;  //projection

uniform mat4 ModelMatrix;		//transform
//uniform int figure=0;

out vec3 theColor;
out vec3 vs_position;


void main()
{
	vec4 v = vec4(position, 1.0);
	
	gl_Position = MVP * ModelMatrix * vec4(position,1.f) ;
	
	theColor = vertexColor;
}

