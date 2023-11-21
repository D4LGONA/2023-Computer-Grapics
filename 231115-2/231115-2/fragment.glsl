#version 330 core

//--- out_Color: ���ؽ� ���̴����� �Է¹޴� ���� ��
//--- FragColor: ����� ������ ������ ������ ���۷� ���� ��.

in vec3 FragPos; //--- ���ؽ� ���̴����Լ� ���� ����
in vec3 Normal;
out vec4 FragColor; //--- ���� ���
uniform vec3 lightColor; // ���� ��
uniform vec3 lightPos; // ���� ��ġ
uniform vec3 objectColor; // ��ü��
uniform vec3 viewPos; // �߰�: �������� ��ġ
uniform bool isLight;
uniform bool isAmbient;

void main(void) 
{
	if(isLight)
	{
		if(isAmbient)
		{
			vec3 ambientLight = {0.3f, 0.3f, 0.3f}; //--- �ֺ� ���� ����
			vec3 ambient = ambientLight * lightColor; //--- �ֺ� ���� ��

			vec3 normalVector = normalize(Normal);
			vec3 lightDir = normalize(lightPos - FragPos.xyz); // ����: ���� ���� ���� ����
			float diffuseLight = max(dot(normalVector, lightDir), 0.0f); //--- N�� L�� ���� ������ ���� ����: ���� ����
			vec3 diffuse = diffuseLight * lightColor; //--- ��� �ݻ� ������: ����ݻ簪 * ��������
			
			int shininess = 128; //--- ���� ���
			vec3 viewDir = normalize(viewPos - FragPos.xyz); //--- �������� ����
			vec3 reflectDir = reflect (-lightDir, normalVector); //--- �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���
			float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V�� R�� ���������� ���� ����: ���� ����
			specularLight = pow(specularLight, shininess); //--- shininess ���� ���־� ���̶���Ʈ�� ������ش�.
			vec3 specular = specularLight * lightColor; //--- �ſ� �ݻ� ������: �ſ�ݻ簪 * ��������
			vec3 result = (ambient + diffuse + specular) * objectColor; //--- ���� ���� ������ �ȼ� ����: (�ֺ�+����ݻ�+�ſ�ݻ�����)*��ü ����
			FragColor = vec4 (result, 1.0);
		}
		else
		{
			vec3 normalVector = normalize(Normal);
			vec3 lightDir = normalize(lightPos - FragPos.xyz); // ����: ���� ���� ���� ����
			float diffuseLight = max(dot(normalVector, lightDir), 0.0f); //--- N�� L�� ���� ������ ���� ����: ���� ����
			vec3 diffuse = diffuseLight * lightColor; //--- ��� �ݻ� ������: ����ݻ簪 * ��������
			
			int shininess = 128; //--- ���� ���
			vec3 viewDir = normalize(viewPos - FragPos.xyz); //--- �������� ����
			vec3 reflectDir = reflect (-lightDir, normalVector); //--- �ݻ� ����: reflect �Լ� - �Ի� ������ �ݻ� ���� ���
			float specularLight = max (dot (viewDir, reflectDir), 0.0); //--- V�� R�� ���������� ���� ����: ���� ����
			specularLight = pow(specularLight, shininess); //--- shininess ���� ���־� ���̶���Ʈ�� ������ش�.
			vec3 specular = specularLight * lightColor; //--- �ſ� �ݻ� ������: �ſ�ݻ簪 * ��������
			vec3 result = (diffuse + specular) * objectColor; //--- ���� ���� ������ �ȼ� ����: (�ֺ�+����ݻ�+�ſ�ݻ�����)*��ü ����
			FragColor = vec4 (result, 1.0);
		}
	}
	else
	{
		FragColor = vec4 (objectColor, 1.0);
	}
}