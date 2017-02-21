#pragma once
#include"stdafx.h"

#include<sstream>
#include<string>
#include<fstream>
#include<vector>
#include<iterator>
#include<algorithm>

using namespace std;

class Filer
{
protected:
	const char delim;

	vector<vector<string>> PreLoad(const string &file_name)
	{
		vector<vector<string>> ret;

		ifstream ifs(file_name);
		if (ifs)
		{
			string line;
			while (getline(ifs, line))
			{
				stringstream temp(line);
				
				vector<string> words;
				string word;

				while (getline(temp, word, this->delim))
				{
					words.push_back(word);
				}

				ret.push_back(words);
			}
		}

		return ret;
	}

	void PreWrite(const string &file_name, const vector<vector<string>> &data_list)
	{
		ofstream ofs(file_name);
		
		auto func = [&](const vector<string> &input)
		{
			copy(begin(input), end(input), ostream_iterator<string>(ofs,&this->delim));
			ofs << endl;
		};

		for_each(begin(data_list), end(data_list), func);
	}

public:
	Filer(const char &delim = ',') :delim(delim) {}

	virtual bool WriteFile(const string &file_name) = 0;
	virtual bool LoadFile(const string &file_name) = 0;
};