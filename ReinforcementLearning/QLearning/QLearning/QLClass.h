#pragma once


//http://qiita.com/icoxfog417/items/242439ecd1a477ece312


#include"stdafx.h"

#include"Filer.h"

#include<functional>
#include<memory>
#include<vector>
#include<algorithm>
#include<random>
#include<iterator>
#include<iostream>
#include<map>
#include<utility>
#include<Randomer.h>

	using namespace std;

template<typename S,typename A>
class QLClass :public Randomer,Filer
{
public:
	using SA = pair<S, A>;
	using SAQ = pair<SA, double>;

	using FuncR = std::function<double(const S &s)>;
	using FuncT = std::function<S(const S &s, const A &a)>;
	using FuncAs = std::function<vector<A>(const S &s)>;
	using FuncLoad = std::function<vector<SAQ>(const vector<vector<string>>&)>;
	using FuncWrite = function<vector<vector<string>>(const map<SA, double>&)>;
private:
	map<SA, double> q_table;

	FuncT T;
	FuncR R;
	FuncAs As;

	FuncLoad load;
	FuncWrite write;

	double learn_rate;
	double r;
	double e;


	A RandAction(const S &s)
	{
		auto pos_a = As(s);

		shuffle(begin(pos_a), end(pos_a), this->mt);
		return pos_a.front();
	}

	void QUpDate(const S &s, const A &a)
	{
		auto s2 = this->T(s, a);
		auto a2 = BestAction(s2);
		double maxE = q_table[make_pair(s2, a2)];

		auto &q = this->q_table[make_pair(s, a)];
		auto &q2 = this->q_table[make_pair(s2, a2)];
		q = (1 - learn_rate)*q + this->learn_rate*(this->R(s2) + this->r*maxE);
	}

public:
	QLClass()
		:QLClass(0.7, 0.9, 0.4)
	{}

	QLClass(const double &lr, const double &r, const double &e)
		:learn_rate(lr), r(r), e(e) {}

	~QLClass() {}

	A Learn(const S &s)
	{
		uniform_real_distribution<> prop;

		A a;

		if (prop(this->mt) < this->e)
		{
			a = this->RandAction(s);
		}
		else
		{
			a = this->BestAction(s);
		}
		this->QUpDate(s, a);

		return a;
	}

	A BestAction(const S &s)
	{
		auto pos_a = As(s);

		A best_a = pos_a.front();
		auto maxQ = this->q_table[make_pair(s, best_a)];

		for (auto &a : pos_a)
		{
			auto q = this->q_table[make_pair(s, a)];
			if (q > maxQ)
			{
				best_a = a;
				maxQ = q;
			}
		}

		return best_a;
	}

	void Disp()
	{
		for (auto &i : this->q_table)
		{
			auto s = i.first.first;
			auto a = i.first.second;

			auto q = i.second;

			cout << "[" << s << "," << a << "]:" << q << endl;
		}
	}

	void SetFunc
		(
			const FuncR &func_r,
			const FuncT &func_t,
			const FuncAs &func_as,
			const FuncLoad &func_load,
			const FuncWrite &func_write
			)
	{
		this->R = func_r;
		this->T = func_t;
		this->As = func_as;
		this->load = func_load;
		this->write = func_write;
	}

	bool LoadFile(const string &file_name)
	{
		auto data_list = this->PreLoad(file_name);

		auto Qs = this->load(data_list);

		for (auto &i : Qs)
		{
			this->q_table[i.first] = i.second;
		}

		return !data_list.empty();
	}

	bool WriteFile(const string &file_name)
	{
		auto data_list = this->write(this->q_table);

		this->PreWrite(file_name, data_list);

		return true;
	}
};