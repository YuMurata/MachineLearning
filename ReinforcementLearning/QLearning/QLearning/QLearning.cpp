// QLearning.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"


#include"QLClass.h"

#include<conio.h>
#include<vector>

using namespace std;

int main(void) 
{
	const vector<int> action_list
		(
	{
		0,1,
	}
	);

	const vector<int> state_list
		(
	{
		0,1,2,3,
	}
	);

	const vector<double> state_reward
			(
		{
			1,2,3,40,
		}
		);

	auto func_r = [&state_reward](const int &s)
	{
		return state_reward[s];
	};

	auto func_t = [&state_list](const int &s, const int &a)
	{
		auto d = a == 0 ? -1 : 1;
		return (s + d + 4) % 4;
	};

	auto func_as = [&action_list](const int &s)
	{
		return action_list;
	};

	

	auto func_load = [](const vector<vector<string>> &data_list)
	{
		pair<int, int> sa;
		double q;

		vector<QLClass<int,int>::SAQ> ret(size(data_list));

		auto func = [](const vector<string> &input)
		{
			QLClass<int, int>::SAQ ret;
			ret.first.first = stoi(input[0]);
			ret.first.second = stoi(input[1]);
			ret.second = stod(input[2]);

			return ret;
		};

		transform(begin(data_list), end(data_list), begin(ret), func);

		return ret;
	};

	auto func_write = [](const map<QLClass<int,int>::SA,double> &q_table)
	{
		string s;
		string a;
		string q;

		vector<vector<string>> ret(size(q_table));

		auto func = [](const pair<QLClass<int, int>::SA, double> &saq)
		{
			vector<string> ret({to_string(saq.first.first),to_string(saq.first.second),to_string(saq.second)});
			return ret;
		};
		
		transform(begin(q_table), end(q_table), begin(ret), func);

		return ret;
	};

	QLClass<int,int> obj;
	
	obj.SetFunc(func_r, func_t, func_as,func_load,func_write);
	
	int s = state_list[0];
	int a;
	
	for (int i = 0; i < 100; ++i)
	{
		a=obj.Learn(s);
		s = func_t(s, a);
	}

	obj.Disp();

	obj.WriteFile("test.txt");

	_getch();
}

