#include "stdafx.h"
#include "Rect.h"
#include "BB.h"

void Rect::Update()
{
	// 움직이는 걸 어떻게 구현할지 생각해 보면,

	Object::Update(); // 변환 행렬 다시 정해주는거
	
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
