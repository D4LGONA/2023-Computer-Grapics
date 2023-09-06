//#include <iostream>
//using namespace std;
//
//double dist(int x, int y, int z)
//{
//	return (sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)));
//}
//
//struct Point
//{
//	int x, y, z;
//	double length;
//};
//
//int main()
//{
//	Point* arr[10] {nullptr};
//	char c;
//	int x, y, z;
//	int count = 0;
//	int cnt = 0;
//
//	Point* max = nullptr;
//	Point* min = nullptr;
//
//
//	while (true)
//	{
//		cin >> c;
//
//		switch (c)
//		{
//		case '+':
//			cin >> x >> y >> z;
//			if (count == 10)
//			{
//				
//			}
//			else
//				arr[count++] = new Point{ x,y,z, dist(x,y,z)};
//			break;
//
//		case '-':
//			delete arr[--count];
//			arr[count] = nullptr;
//			break;
//			
//		case 'e':
//			cin >> x >> y >> z;
//			for (int i = 9; i > 0; --i)
//			{
//				Point* tmp = arr[i];
//				arr[i] = arr[i - 1];
//				arr[i - 1] = tmp;
//			}
//			break;
//			
//		case 'd':
//			for (int i = 0; i < 10; ++i)
//			{
//				if (arr[i] != nullptr)
//				{
//					delete arr[i];
//					arr[i] = nullptr;
//					break;
//				}
//				if (i == 9)
//					cout << "배열 안에 좌표가 없음." << endl;
//			}
//			break;
//			
//		case 'l': // d
//			cnt = 0;
//
//			for (int i = 0; i < 10; ++i)
//			{
//				if (arr[i] != nullptr)
//					cnt++;
//			}
//
//			cout << cnt;
//			break;
//			
//		case 'c': // d
//			count = 0;
//			for (int i = 0; i < 10; ++i)
//			{
//				delete arr[i];
//				arr[i] = nullptr;
//			}
//			break;
//			
//		case 'm': // d
//			for (int i = 0; i < 10; ++i)
//			{
//				if (i == 0) max = arr[i];
//				else
//				{
//					if (arr[i] != nullptr && arr[i]->length > max->length)
//						max = arr[i];
//				}
//			}
//			cout << max->x << ", " << max->y << ", " << max->z << endl;
//			max = nullptr;
//			break;
//
//		case 'n': // d
//			for (int i = 0; i < 10; ++i)
//			{
//				if (i == 0) min = arr[i];
//				else
//				{
//					if (arr[i] != nullptr && arr[i]->length < min->length)
//						min = arr[i];
//				}
//			}
//			cout << min->x << ", " << min->y << ", " << min->z << endl;
//			min = nullptr;
//			break;
//
//		case 'a': // d
//			qsort(arr, 10, sizeof(Point*),
//				[](const void* a, const void* b)->int {
//					if ((*(Point*)a).length > (*(Point*)b).length)
//						return 1;
//					else if ((*(Point*)a).length < (*(Point*)b).length)
//						return -1;
//					else return 0;
//				});
//			break;
//
//		case 's': // d
//			qsort(arr, 10, sizeof(Point*),
//				[](const void* a, const void* b)->int {
//					if ((*(Point*)a).length > (*(Point*)b).length)
//						return -1;
//					else if ((*(Point*)a).length < (*(Point*)b).length)
//						return 1;
//					else return 0;
//				});
//			break;
//
//		case 'p': // d
//			for (int i = 0; i < 10; ++i)
//			{
//				if(arr[i] != nullptr)
//					cout << arr[i]->x << ", " << arr[i]->y << ", " << arr[i]->z << endl;
//			}
//			break;
//
//		case 'q':
//			exit(0);
//			break;
//
//		default:
//			cout << "잘못된 입력입니다." << endl;
//			break;
//		}
//
//	}
//}