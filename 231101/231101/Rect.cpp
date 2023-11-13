#include "stdafx.h"
#include "Rect.h"

void Rect::Update()
{
	// 움직이는 걸 어떻게 구현할지 생각해 보면,
	speed += gravity;
	Move(1, speed);
	

	Object::Update(); // 변환 행렬 다시 정해주는거
	
}
