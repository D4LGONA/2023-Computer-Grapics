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
	if(dir.y != 0)
		speed.x += (accel * (dir.x / dir.y));
	speed.y += gravity;

	Move(1, speed.y);
	Move(0, speed.x);
}

void Rect::MoveRectSlide(glm::vec3 dest)
{
	this->dest = dest;
	dir = this->dest - this->bb->vCenterPos;
}
