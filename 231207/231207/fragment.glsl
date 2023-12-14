#version 330 core

//--- out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- FragColor: ����� ������ ������ ������ ���۷� ���� ��.

in vec3 FragPos; //--- ���ؽ� ���̴����Լ� ���� ����
in vec4 Color;
in vec3 Normal;
out vec4 FragColor; //--- ���� ���
uniform vec3 lightColor; // ���� ��
uniform vec3 lightPos; // ���� ��ġ
uniform vec3 viewPos; // �߰�: �������� ��ġ
uniform bool isLight;
uniform float ambientLight;

void main(void) 
{
	if(isLight)
	{
		vec3 ambient = ambientLight * lightColor; // �ֺ� ���� ��

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
		vec3 ambient = ambientLight * lightColor; // �ֺ� ���� ��

		vec3 result = ambient * Color.rgb;
		FragColor = vec4(result, Color.a);
	}
}
