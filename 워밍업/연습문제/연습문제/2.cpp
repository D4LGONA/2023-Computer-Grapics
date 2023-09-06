//#include <fstream>
//#include <iostream>
//#include <vector>
//#include <string>
//#include <regex>
//using namespace std;
//
//int main()
//{
//	vector<string> v;
//	string tmp;
//	ifstream in( "data.txt" );
//	int word_count = 0;
//	int number_count = 0;
//	int capital_count = 0;
//
//	if (!in.is_open())
//		cout << "엥?" << endl;
//
//	while (getline(in,tmp))
//	{
//		v.push_back(tmp);
//		regex pattern("\\s+"); // 공백 빼는 것
//		regex wordRegex("\\b\\w+\\b"); // 단어 개수 세기
//		regex numberRegex("\\b\\d+\\b"); // 숫자 개수 세기
//		regex capitalRegex("\\b\\w*[A-Z]+\\w*\\b");
//		sregex_iterator wordIterator(v.back().begin(), v.back().end(), wordRegex);
//		sregex_iterator numberIterator(v.back().begin(), v.back().end(), numberRegex);
//		sregex_iterator capitalIterator(v.back().begin(), v.back().end(), capitalRegex);
//		sregex_iterator end;
//
//		word_count += distance(wordIterator, end);
//		number_count += distance(numberIterator, end);
//		capital_count += distance(capitalIterator, end);
//		v.back() = regex_replace(tmp, pattern, " ");
//	}
//
//	for (int i = 0; i < v.size(); ++i)
//		cout << v[i] << endl;
//
//	cout << word_count - number_count << endl;
//	cout << number_count << endl;
//	cout << capital_count << endl;
//}