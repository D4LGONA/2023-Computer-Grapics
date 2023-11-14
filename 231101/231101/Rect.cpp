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
