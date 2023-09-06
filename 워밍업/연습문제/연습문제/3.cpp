#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <regex>
using namespace std;

int main()
{
	vector<string> v;
	string tmp;
	ifstream in("data.txt");

	if (!in.is_open())
		cout << "��?" << endl;

	while (getline(in, tmp))
	{
		v.push_back(tmp);
		regex pattern("\\s+"); // ���� ���� ��
		v.back() = regex_replace(tmp, pattern, " ");
	}

	for (int i = 0; i < v.size(); ++i)
		cout << v[i] << endl;

	char c;
	bool d = false, e = false, f = false;

	while (true)
	{
		cin >> c;

		switch (c)
		{
		case 'd': // d
			for (int i = 0; i < v.size(); ++i)
				reverse(v[i].begin(), v[i].end());
			break;

		case 'e': // ��۷� ������ �� �Ф�
			if (e)
			{

			}
			else
			{
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
			
		case 'g':
			break;

		case 'h':
			break;

		case '+':
			
			break;
			
		case '-':
			
			break;

		case 'q':
			break;

		case 'p':
			for (int i = 0; i < v.size(); ++i)
				cout << v[i] << endl;
			break;

		default:
			cout << "�߸��� �Է��Դϴ�" << endl;
			break;
		}
	}

}