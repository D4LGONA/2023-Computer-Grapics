//#include <iostream>
//#include <Windows.h>
//#include <random>
//#include <thread>
//using namespace std;
//using std::thread;
//
//random_device rd;
//mt19937 dre(rd());
//uniform_int_distribution uid{ 0, 25 };
//uniform_int_distribution uid2{ 1, 14 };
//
//struct CARD
//{
//	char c;
//	bool isOpen = false;
//	bool isRight = true;
//	int count = 0;
//	int color = 15;
//};
//
//auto programStartTime = chrono::high_resolution_clock::now();
//CARD map[5][5];
//string input1, input2;
//int cnt = 0;
//bool play = true;
//int endcnt = 0;
//
//void reset()
//{
//	programStartTime = chrono::high_resolution_clock::now();
//	pair<pair<char, int>, int> arr[8];
//	for (int i = 0; i < 8; ++i)
//	{
//		arr[i].first.first = uid(dre) + 'A';
//		arr[i].second = uid2(dre);
//		for (int j = 0; j < i; ++j)
//		{
//			if (arr[j].first.first == arr[i].first.first || arr[j].second == arr[i].second)
//			{
//				i--;
//				break;
//			}
//		}
//		arr[i].first.second = 0;
//	}
//
//	map[0][0].c = ' ';
//	map[0][1].c = '1';
//	map[0][2].c = '2';
//	map[0][3].c = '3';
//	map[0][4].c = '4';
//
//	map[1][0].c = 'a';
//	map[2][0].c = 'b';
//	map[3][0].c = 'c';
//	map[4][0].c = 'd';
//
//	for (int i = 1; i < 5; ++i)
//	{
//		for (int j = 1; j < 5; ++j)
//		{
//			while (true)
//			{
//				uniform_int_distribution u{ 0,7 };
//				int temp = u(dre);
//				if (arr[temp].first.second != 2)
//				{
//					map[i][j].c = arr[temp].first.first;
//					map[i][j].color = arr[temp].second;
//					map[i][j].isRight = false;
//					map[i][j].isOpen = false;
//					arr[temp].first.second++;
//					break;
//				}
//			}
//		}
//	}
//}
//
//void drawMap()
//{
//	for (int i = 0; i < 5; ++i)
//	{
//		for (int j = 0; j < 5; ++j)
//		{
//			if (map[i][j].isRight)
//			{
//				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), map[i][j].color);
//				cout << map[i][j].c << " ";
//				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
//			}
//			else if (map[i][j].isOpen)
//			{
//				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), map[i][j].color);
//				cout << map[i][j].c << " ";
//				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
//			}
//			else
//				cout << "* ";
//		}
//		cout << endl;
//	}
//}
//
//bool isEnd()
//{
//	bool end = true;
//	for (int i = 0; i < 5; ++i)
//	{
//		for (int j = 0; j < 5; ++j)
//			if (!map[i][j].isRight)
//			{
//				end = false;
//				break;
//			}
//	}
//	return end;
//}
//
//void func1() // 그리는 쓰레드
//{
//	while (true)
//	{
//		auto currentTime = chrono::high_resolution_clock::now();
//		auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - programStartTime).count();
//		system("cls");
//		cout << "시간: " << elapsedTime <<" 시도 횟수: " << cnt << endl << flush;
//		drawMap();
//		this_thread::sleep_for(chrono::seconds(1));
//		
//	}
//}
//
//void func2() // 입력받는 쓰레드
//{
//	while (true)
//	{
//		for (int i = 0; i < 5; ++i)
//			for (int j = 0; j < 5; ++j)
//				map[i][j].isOpen = false;
//		
//		cin >> input1;
//		if (input1[0] == 'r')
//		{
//			reset();
//			cnt = 0;
//			continue;
//		}
//		else if (input1[0] >= 'a' && input1[0] <= 'd' && input1[1] >= '1' && input1[1] <= '4')
//		{
//			map[input1[0] - ('a' - 1)][input1[1] - '0'].isOpen = true;
//			cin >> input2;
//			cnt++;
//			if (!(input2[0] >= 'a' && input2[0] <= 'd' && input2[1] >= '1' && input2[1] <= '4') || input1 == input2)
//			{
//				map[input1[0] - ('a' - 1)][input1[1] - '0'].isOpen = false;
//				continue;
//			}
//
//			if (map[input1[0] - ('a' - 1)][input1[1] - '0'].c == map[input2[0] - ('a' - 1)][input2[1] - '0'].c)
//			{
//				map[input1[0] - ('a' - 1)][input1[1] - '0'].isOpen = false;
//				map[input1[0] - ('a' - 1)][input1[1] - '0'].isRight = true;
//				map[input2[0] - ('a' - 1)][input2[1] - '0'].isRight = true;
//			}
//			else
//			{
//				map[input1[0] - ('a' - 1)][input1[1] - '0'].isOpen = true;
//				map[input2[0] - ('a' - 1)][input2[1] - '0'].isOpen = true;
//				Sleep(1000);
//			}
//		}
//		else if (input1[0] == 'e')
//		{
//			// 한번에 풀리는 코드입니다.
//			for (int i = 0; i < 5; ++i)
//			{
//				for (int j = 0; j < 5; ++j)
//				{
//					map[i][j].isRight = true;
//				}
//			}
//			cnt++;
//			play = false;
//		}
//		else if (input1[0] == 'q')
//		{
//			exit(0);
//		}
//		else
//			cnt++;
//		if (!play)
//		{
//			auto currentTime = chrono::high_resolution_clock::now();
//			auto elapsedTime = chrono::duration_cast<chrono::seconds>(currentTime - programStartTime).count();
//			system("cls");
//			cout << "시간: " << elapsedTime << " 시도 횟수: " << cnt << endl << flush;
//			drawMap();
//			exit(0);
//		}
//	}
//}
//
//int main()
//{
//	reset();
//	thread t1(func1);
//	thread t2(func2);
//
//	t1.join();
//	t2.join();
//	
//	return 0;
//}