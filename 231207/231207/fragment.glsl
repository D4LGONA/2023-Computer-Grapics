#version 330 core

//--- out_Color: 버텍스 세이더에서 입력받는 색상 값
//--- FragColor: 출력할 색상의 값으로 프레임 버퍼로 전달 됨.

in vec3 FragPos; //--- 버텍스 세이더에게서 전달 받음
in vec4 Color;
in vec3 Normal;
out vec4 FragColor; //--- 색상 출력
uniform vec3 lightColor; // 조명 색
uniform vec3 lightPos; // 조명 위치
uniform vec3 viewPos; // 추가: 관찰자의 위치
uniform bool isLight;
uniform float ambientLight;

void main(void) 
{
	if(isLight)
	{
		vec3 ambient = ambientLight * lightColor; // 주변 조명 값

		vec3 normalVector = normalize(Normal);
		vec3 lightDir = normalize(lightPos - FragPos.xyz);
		float diffuseLight = max(dot(normalVector, lightDir), 0.0f);
		vec3 diffuse = diffuseLight * lightColor;
		
		int shininess = 128;
		vec3 viewDir = normalize(viewPos - FragPos.xyz);
		vec3 reflectDir = reflect (-lightDir, normalVector);
		float specularLight = max(dot(viewDir, reflectDir), 0.0);
		specularLight = pow(specularLight, shininess);
		vec3 specular = specularLight * lightColor;
		
		vec3 result = (ambient + diffuse + specular) * Color.rgb;
		FragColor = vec4(result, Color.a);
	}
	else
	{
		vec3 ambient = ambientLight * lightColor; // 주변 조명 값

		vec3 result = ambient * Color.rgb;
		FragColor = vec4(result, Color.a);
	}
}
