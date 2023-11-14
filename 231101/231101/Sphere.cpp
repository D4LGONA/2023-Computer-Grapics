#include "stdafx.h"
#include "Sphere.h"

void Sphere::Update()
{
	Move(0, dir.x);
	Move(1, dir.y);
	Move(2, dir.y);

	Object::Update(); // 변환 행렬 다시 정해주는거
}
