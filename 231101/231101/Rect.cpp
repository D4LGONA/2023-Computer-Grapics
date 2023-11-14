#include "stdafx.h"
#include "Rect.h"
#include "BB.h"

void Rect::Update()
{
	// �����̴� �� ��� �������� ������ ����,

	Object::Update(); // ��ȯ ��� �ٽ� �����ִ°�
	
}

void Rect::MoveRect()
{
	speed.y += gravity;

	if (abs(GetRot().z) < FLT_EPSILON)
	{
		speed.x = 0.0f;
	}
	else
	{
		while (GetRot().z < 0.0f)
			Rot(2, 360.0f);

		while (GetRot().z > 360.0f)
			Rot(2, -360.0f);

		if (!isStop)
		{
			if (GetRot().z > 0.0f && GetRot().z < 180.0f)
				speed.x -= accel;
			else
				speed.x += accel;
		}
	}

	Move(0, speed.x);
	Move(1, speed.y);
}

void Rect::MoveRectSlide(glm::vec3 dest)
{
	this->dest = dest;
	dir = this->dest - this->bb->vCenterPos;
}
