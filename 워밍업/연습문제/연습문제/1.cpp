//#include <iostream>
//#include <random>
//#include <vector>
//using namespace std;
//
//random_device rd;
//mt19937 dre(rd());
//uniform_int_distribution uidN{ 0,2 };
//
//int main()
//{
//	char c;
//	int n = 3;
//	int det1, det2;
//	int sum = 0;
//	int tmp1 = 0, tmp2 = 0;
//	int t = 1;
//
//	vector<vector<int>> arr1;
//	vector<vector<int>> arr1T;
//	vector<vector<int>> arr2;
//	vector<vector<int>> arr2T;
//
//	for (int i = 0; i < n; ++i)
//	{
//		arr1.push_back({ uidN(dre), uidN(dre) , uidN(dre) });
//		arr2.push_back({ uidN(dre), uidN(dre) , uidN(dre) });
//	}
//
//	for (int i = 0; i < n; ++i)
//	{
//		for (int j = 0; j < n; ++j)
//		{
//			cout << arr1[i][j] << " ";
//		}
//		cout << endl;
//	}
//
//	cout << endl;
//
//	for (int i = 0; i < n; ++i)
//	{
//		for (int j = 0; j < n; ++j)
//		{
//			cout << arr2[i][j] << " ";
//		}
//		cout << endl;
//	}
//
//	while (true)
//	{
//		cin >> c;
//
//		switch (c)
//		{
//		case 'm': // 행렬의 곱셈
//			sum = 0;
//			// [0][0] [0][1] [0][2]	[0][0] [0][1] [0][2]
//			// [1][0] [1][1] [1][2] [1][0] [1][1] [1][2]
//			// [2][0] [2][1] [2][2] [2][0] [2][1] [2][2]
//			// 00*00 + 01*10 + 02*20
//			// 00*01 + 01*11 + 02*21
//			// 00*02 + 01+12 + 02*22
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					for (int k = 0; k < n; ++k)
//					{
//						sum += arr1[i][k] * arr2[k][j];
//					}
//					cout << sum << " ";
//					sum = 0;
//				}
//				cout << endl;
//			}
//
//			break;
//
//		case 'a': // 행렬의 덧셈
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					cout << arr1[i][j] + arr2[i][j] << " ";
//				}
//				cout << endl;
//			}
//			break;
//
//		case 'd': // 행렬의 뺄셈
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					cout << arr1[i][j] - arr2[i][j] << " ";
//				}
//				cout << endl;
//			}
//			break;
//
//		case 'r': // 행렬식
//			tmp1 = 0; tmp2 = 0;
//			t = 1;
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (j + i >= n)
//					{
//						t *= arr1[j][j + i - 3];
//					}
//					else
//					{
//						t *= arr1[j][j + i];
//					}
//				}
//				tmp1 += t;
//				t = 1;
//			}
//			
//			t = 1;
//			for (int i = n - 1; i >= 0; --i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (i - j < 0)
//					{
//						t *= arr1[j][i - j + 3];
//					}
//					else
//					{
//						t *= arr1[j][i - j];
//					}
//				}
//				tmp2 += t;
//				t = 1;
//			}
//
//			det1 = tmp1 - tmp2;
//			cout << det1 << endl;
//
//			//
//
//			tmp1 = 0; tmp2 = 0;
//			t = 1;
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (j + i >= n)
//					{
//						t *= arr2[j][j + i - 3];
//					}
//					else
//					{
//						t *= arr2[j][j + i];
//					}
//				}
//				tmp1 += t;
//				t = 1;
//			}
//
//			t = 1;
//			for (int i = n - 1; i >= 0; --i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (i - j < 0)
//					{
//						t *= arr2[j][i - j + 3];
//					}
//					else
//					{
//						t *= arr2[j][i - j];
//					}
//				}
//				tmp2 += t;
//				t = 1;
//			}
//
//			det2 = tmp1 - tmp2;
//			cout << det2 << endl;
//			break;
//
//		case 't': // 전치행렬 + 그 행렬식
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					cout << arr1[j][i] << " ";
//				}
//				cout << endl;
//			}
//
//			cout << endl;
//
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					cout << arr2[j][i] << " ";
//				}
//				cout << endl;
//			}
//
//			// 행렬식
//			tmp1 = 0; tmp2 = 0;
//			t = 1;
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (j + i >= n)
//					{
//						t *= arr1[j][j + i - 3];
//					}
//					else
//					{
//						t *= arr1[j][j + i];
//					}
//				}
//				tmp1 += t;
//				t = 1;
//			}
//
//			t = 1;
//			for (int i = n - 1; i >= 0; --i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (i - j < 0)
//					{
//						t *= arr1[j][i - j + 3];
//					}
//					else
//					{
//						t *= arr1[j][i - j];
//					}
//				}
//				tmp2 += t;
//				t = 1;
//			}
//
//			det1 = tmp1 - tmp2;
//			cout << det1 << endl;
//
//			//
//
//			tmp1 = 0; tmp2 = 0;
//			t = 1;
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (j + i >= n)
//					{
//						t *= arr2[j][j + i - 3];
//					}
//					else
//					{
//						t *= arr2[j][j + i];
//					}
//				}
//				tmp1 += t;
//				t = 1;
//			}
//
//			t = 1;
//			for (int i = n - 1; i >= 0; --i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					if (i - j < 0)
//					{
//						t *= arr2[j][i - j + 3];
//					}
//					else
//					{
//						t *= arr2[j][i - j];
//					}
//				}
//				tmp2 += t;
//				t = 1;
//			}
//
//			det2 = tmp1 - tmp2;
//			cout << det2 << endl;
//			break;
//
//		case 'h': // 4x4 행렬 변환
//			arr1.push_back({ 0 });
//			arr2.push_back({ 0 });
//			for (int i = 0; i < n; ++i)
//			{
//				if (i + 1 == n)
//				{
//					arr1[n].push_back(1);
//					arr2[n].push_back(1);
//				}
//				else
//				{
//					arr1[n].push_back(0);
//					arr2[n].push_back(0);
//				}
//			}
//			for (int i = 0; i < n; ++i)
//			{
//				arr1[i].push_back(0);
//				arr2[i].push_back(0);
//			}
//			n++;
//			break;
//
//		case 's': // 재설정
//			for (int i = 0; i < n; ++i)
//			{
//				arr1[i].clear();
//				arr2[i].clear();
//			}
//			arr1.clear();
//			arr2.clear();
//
//			n = 3;
//
//			for (int i = 0; i < n; ++i)
//			{
//				arr1.push_back({ uidN(dre), uidN(dre) , uidN(dre) });
//				arr2.push_back({ uidN(dre), uidN(dre) , uidN(dre) });
//			}
//			break;
//
//		case 'p': // 출력ㅠㅠ
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					cout << arr1[i][j] << " ";
//				}
//				cout << endl;
//			}
//
//			cout << endl;
//
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					cout << arr2[i][j] << " ";
//				}
//				cout << endl;
//			}
//			break;
//
//		case 'q':
//			exit(0);
//			break;
//
//		case '1': // ?
//			break;
//		case '2':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 2;
//					arr2[i][j] = arr2[i][j] * 2;
//				}
//			}
//			break;
//		case '3':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 3;
//					arr2[i][j] = arr2[i][j] * 3;
//				}
//			}
//			break;
//		case '4':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 4;
//					arr2[i][j] = arr2[i][j] * 4;
//				}
//			}
//			break;
//		case '5':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 5;
//					arr2[i][j] = arr2[i][j] * 5;
//				}
//			}
//			break;
//		case '6':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 6;
//					arr2[i][j] = arr2[i][j] * 6;
//				}
//			}
//			break;
//		case '7':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 7;
//					arr2[i][j] = arr2[i][j] * 7;
//				}
//			}
//			break;
//		case '8':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 8;
//					arr2[i][j] = arr2[i][j] * 8;
//				}
//			}
//			break;
//		case '9':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 9;
//					arr2[i][j] = arr2[i][j] * 9;
//				}
//			}
//			break;
//		case '0':
//			for (int i = 0; i < n; ++i)
//			{
//				for (int j = 0; j < n; ++j)
//				{
//					arr1[i][j] = arr1[i][j] * 0;
//					arr2[i][j] = arr2[i][j] * 0;
//				}
//			}
//			break;
//
//		default:
//			cout << "잘못된 입력입니다." << endl;
//			break;
//		}
//	}
//}
