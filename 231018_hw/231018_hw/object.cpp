#include "stdafx.h"
#include "object.h"

void object::render(GLuint vbo[])
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * pt.size(), pt.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * c.size(), c.data(), GL_DYNAMIC_DRAW);
	

	glDrawArrays(GL_LINE_LOOP, 0, pt.size());
}

void object::update()
{
	pt.clear();
	c.clear();

	for (int i = 0; i < pts.size(); ++i)
	{
		pair<float, float> tmppt = WintoOpenGL(pts[i]);
		pt.push_back({ tmppt.first, tmppt.second, 0.0f });
		c.emplace_back(color);
	}
}

void object::remove()
{
	pt.clear();
	c.clear();
	pts.clear();
}

