#include "stdafx.h"
#include "Sphere.h"

void Sphere::Update()
{
	Move(0, dir.x);
	Move(1, dir.y);
	Move(2, dir.y);

	Object::Update(); // ��ȯ ��� �ٽ� �����ִ°�
}
