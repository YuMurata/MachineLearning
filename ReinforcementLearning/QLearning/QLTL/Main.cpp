// QLTL.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include"QLTLClass.h"
#include<conio.h>

using namespace std;
int main()
{
	using S = int;
	using A = int;
	using DataType = int;
	using QLTL = QLTLClass<S, A, DataType>;
	using SA = QLTL::SA;
	using Q = QLTL::Q;
	using SAQ = QLTL::SAQ;
	using DataList = QLTL::DataList;
	using SToLSH = QLTL::SToLSH;
	using LSHToS = QLTL::LSHToS;

	enum
	{
		UP,
		LEFT,
		DOWN,
		RIGHT,
	};

	const vector<S> s_list =
	{
		   0,1,2,
		 3,4,5,6,7,
		   8,9,10,
	};

	const vector<A> a_list=
	{
		UP,LEFT,DOWN,RIGHT,
	};

	auto r = [](const S &s)
	{
		Q ret;
		if (s == 3)
		{
			ret= -10;
		}
		else if(s==7)
		{
			ret= 10;
		}
		else
		{
			ret = -1;
		}

		return ret;
	};

	auto t = [&s_list](const S &s, const A &a)
	{
		int d[] = { -4,-1,+4,+1, };
		int ret = s_list[s] + d[a];

		auto not_up = ret < 0;
		auto not_down = ret >= 11;
		auto not_left = (a == LEFT) && (s_list[s] == 0 || s_list[s] == 8);
		auto not_right = (a == RIGHT) && (s_list[s] == 2 || s_list[s] == 10);

		if (s_list[s] == 3 || s_list[s] == 7)
		{
			ret = 5;
		}
		else if (not_up || not_down || not_left || not_right)
		{
			ret -= d[a];
		}

		return ret;

	};

	auto as = [&a_list](const S &s)
	{
		return a_list;
	};

	auto load = [](const vector<vector<string>> &str)
	{
		return vector<SAQ>();
	};

	auto write = [](const QLTL::QTable &saq)
	{
		return vector<vector<string>>();
	};

	auto slsh = [](const S &s)
	{
		return DataList(1, s);
	};

	auto lshs = [](const DataList &dl)
	{
		return dl.front();
	};

	QLTL obj(1);
	obj.SetFunc(r, t, as, load, write, slsh, lshs);

	int s = 5;
	int a;

	for (int i = 0; i < 100; ++i)
	{
		a = obj.Learn(s);
		s = t(s, a);
	}

	obj.Disp();
	
	_getch();
	return 0;
}

