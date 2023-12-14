#version 330 core

//--- in_Position: attribute index 0 
//--- in_Color: attribute index 1

layout (location = 0) in vec3 in_Position; //--- 위치 변수: attribute position 0
layout (location = 1) in vec4 in_color; //--- 노말값 변수: attribute position 1
layout (location = 2) in vec3 in_Normal; //--- 노말값 변수: attribute position 1
out vec3 FragPos; //--- 객체의 위치값을 프래그먼트 세이더로 보낸다.
out vec3 Normal;
out vec4 Color;
uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main(void) 
{
	gl_Position = projection * view * transform * vec4(in_Position, 1.0f);
	FragPos = vec3(transform * vec4(in_Position, 1.0f));
	Normal =  vec3(projection * view * transform * vec4(in_Normal, 1.0f));
	Color = in_color;
}