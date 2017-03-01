#pragma once

#include"../QLearning/QLClass.h"
#include"../../NearestNeighbor/LSH/LSH/LSH.h"
#include<MathPlus.h>

//FuncR = std::function<double(const S &s)>;
//FuncT = std::function<S(const S &s, const A &a)>;
//FuncAs = std::function<vector<A>(const S &s)>;
//FuncLoad = std::function<vector<SAQ>(const vector<vector<string>>&)>;
//FuncWrite = function<vector<vector<string>>(const QTable&)>;
//SToLSH = function<vector<DataType>(const S&)>;
//LSHToS = function<S(vector<DataType>&)>;
template<typename S,typename A,typename DataType>
class QLTLClass :public QLClass<S, A>
{
public:
	using DataList = vector<DataType>;
	using SToLSH = function<DataList(const S&)>;
	using LSHToS = function<S(const DataList&)>;

private:
	LSH::LSHClass<DataType> lsh;

	SToLSH s_to_lsh;
	LSHToS lsh_to_s;


public:

	QLTLClass(const int &d)
		:lsh(d)
	{}

	QLTLClass(const double &lr, const double &r, const double &e,const int &d)
		:QLClass(lr, r, e),lsh(d) {}

	void QUpDate(const S &org_s, const A &a)override
	{

		auto coord = s_to_lsh(org_s);
		auto nears = this->lsh.query(coord);

		if (nears.empty())
		{
			this->QLClass::QUpDate(org_s, a);
		}
		else
		{
			enum
			{
				Org,
				Near,
				NUM,
			};

			S s2[NUM];

			A a2[NUM];

			Q maxQ[NUM];

			Q value[NUM];

			auto near = **begin(nears);
			auto SG = 1. / (1 + MathPlus::Distance::Distance(coord, near));

			auto &update = this->q_table[make_pair(org_s, a)];

			Q &q = this->q_table[make_pair(org_s, a)];

			s2[Org]=this->T(org_s, a);
			s2[Near] = lsh_to_s(near);

			for (int i = Org; i <= Near; ++i)
			{
				a2[i] = BestAction(s2[i]);
				maxQ[i] = this->q_table[make_pair(s2[i], a2[i])];
			}

			value[Org] = (1 - this->learn_rate)*q + this->learn_rate*(this->R(s2[Org]) + this->r*maxQ[Org]);
			value[Near] = maxQ[Near];

			q = (1 - SG)*maxQ[Org] + SG*maxQ[Near];

			this->lsh.add(s_to_lsh(org_s));
		}
	}
	
	void SetFunc
		(
			const FuncR &func_r,
			const FuncT &func_t,
			const FuncAs &func_as,
			const FuncLoad &func_load,
			const FuncWrite &func_write,
			const SToLSH &s_to_lsh,
			const LSHToS &lsh_to_s
			)
	{
		this->QLClass<S, A>::SetFunc(func_r, func_t, func_as, func_load, func_write);
		this->s_to_lsh = s_to_lsh;
		this->lsh_to_s = lsh_to_s;
	}
};