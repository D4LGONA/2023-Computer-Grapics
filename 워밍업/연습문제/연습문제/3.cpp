#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
using namespace std;

//string func()
//{
//	smatch match;
//	int num = stoi(match[0].str()); 
//	num += 1; 
//	return to_string(num);
//}

int main()
{
	vector<string> v;
	string tmp;
	ifstream in("data.txt");

	if (!in.is_open())
		cout << "엥?" << endl;

	while (getline(in, tmp))
	{
		v.push_back(tmp);
		regex pattern("\\s+"); // 공백 빼는 것
		v.back() = regex_replace(tmp, pattern, " ");
	}

	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << endl;

	char c;
	bool d = false, e = false, f = false;
	char input1, input2;

	while (true)
	{
		cin >> c;

		switch (c)
		{
		case 'd': // d
			for (int i = 0; i < v.size(); ++i)
				reverse(v[i].begin(), v[i].end());
			break;

		case 'e': // 토글로 만들어야 해 ㅠㅠ
			if (e)
			{
				e = !e;
				regex pattern("@{2,}");
				for (int i = 0; i < v.size(); ++i)
				{
					v[i] = regex_replace(v[i], pattern, "");
				}
				
			}
			else
			{
				e = !e;
				for (int i = 0; i < v.size(); ++i)
				{
					string temp = "";
					for (int j = 0; j < v[i].size(); ++j)
					{
						if (j != 0 &&(j % 3 == 0))
						{
							temp += "@@";
							temp += v[i][j];
						}
						else
							temp += v[i][j];
					}
					v[i] = temp;
				}
			}
			break;

		case 'f': // d
			for (int i = 0; i < v.size(); ++i)
			{
				string::iterator tmp = v[i].begin();
				for (string::iterator j = v[i].begin(); j != v[i].end(); ++j)
				{
					if (*j == ' ')
					{
						reverse(tmp, j);
						tmp = j + 1;
					}
					else if (j == v[i].end() - 1)
					{
						if (isalpha(v[i].back()) || isdigit(v[i].back()))
							reverse(tmp, v[i].end());
						else
							reverse(tmp, j);
					}
				}
			}
			break;
			
		case 'g': // 문자 내부의 특정 문자를 다른 문자로 바꾸기 d
			cin >> input1 >> input2;

			for (int i = 0; i < v.size(); ++i)
			{
				for (int j = 0; j < v[i].size(); ++j)
				{
					if (v[i][j] == input1)
						v[i][j] = input2;
				}
			}
			break;

		case 'h': // 어디까지 같은 문자인지 출력
			for (int i = 0; i < v.size(); ++i)
			{
				for (int j = 0; j < v[i].size() / 2; ++j)
				{
					if (v[i][j] == v[i][v[i].size() - j - 1])
						cout << v[i][j];
					else break;
				}
				cout << endl;
			}
			break;

		case '+':
		{
			int tmpidx = 0;
			for (int i = 0; i < v.size(); ++i)
			{
				int j = 0;
				tmpidx = 0;
				while (true)
				{
					if (j == v[i].size())
					{
						if (isdigit(v[i][j - 1]))
						{
							string str = v[i].substr(tmpidx, j - tmpidx);
							v[i].erase(tmpidx, j - tmpidx);
							int k = stoi(str);
							k += 1;
							v[i].insert(tmpidx, to_string(k));
							if (k % 10 == 0) j += 1;
						}
						tmpidx = j + 1;

						break;
					}

					if (v[i][j] == ' ')
					{
						if (isdigit(v[i][j - 1]))
						{
							string str = v[i].substr(tmpidx, j - tmpidx);
							v[i].erase(tmpidx, j - tmpidx);
							int k = stoi(str);
							k += 1;
							v[i].insert(tmpidx, to_string(k));
							if (k % 10 == 0) j += 1;
						}
						tmpidx = j + 1;
					}
					j++;
				}
			}
			break;
			/*regex numpattern("\\b\\d+\\b");
			for (int i = 0; i < v.size(); ++i)
			{
				v[i] = regex_replace(v[i], numpattern, [](const smatch& match) -> string
					{
						int num = stoi(match.str());
						num += 1;
						return to_string(num);
					});
			}
			break;*/
		}
			
		case '-':
		{
			int tmpidx = 0;
			for (int i = 0; i < v.size(); ++i)
			{
				int j = 0;
				tmpidx = 0;
				while (true)
				{
					if (j == v[i].size())
					{
						if (isdigit(v[i][j - 1]))
						{
							string str = v[i].substr(tmpidx, j - tmpidx);
							v[i].erase(tmpidx, j - tmpidx);
							int k = stoi(str);
							if(k > 0)
								k -= 1;
							v[i].insert(tmpidx, to_string(k));
							if (k % 9 == 0) j -= 1;
						}
						tmpidx = j + 1;

						break;
					}

					if (v[i][j] == ' ')
					{
						if (isdigit(v[i][j - 1]))
						{
							string str = v[i].substr(tmpidx, j - tmpidx);
							v[i].erase(tmpidx, j - tmpidx);
							int k = stoi(str);
							if (k > 0)
								k -= 1;
							v[i].insert(tmpidx, to_string(k));
							if (k % 9 == 0 && k != 0) 
								j -= 1;
						}
						tmpidx = j + 1;
					}
					j++;
				}
			}
			break;
		}
			
			break;

		case 'q':
			exit(0);
			break;

		case 'p':
			for (int i = 0; i < v.size(); ++i)
				cout << v[i] << endl;
			break;

		default:
			cout << "잘못된 입력입니다" << endl;
			break;
		}
	}

}