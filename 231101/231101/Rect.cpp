#include "stdafx.h"
#include "Rect.h"

void Rect::Update()
{
	// �����̴� �� ��� �������� ������ ����,
	speed += gravity;
	Move(1, speed);
	

	Object::Update(); // ��ȯ ��� �ٽ� �����ִ°�
	
}
