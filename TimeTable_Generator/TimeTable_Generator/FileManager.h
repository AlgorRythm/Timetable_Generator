#pragma once

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <string>
using namespace std;


template <typename dataType>
ostream& operator<<(ostream& _os, const vector<dataType>& _vec) {
	for (int i = 0; i < (int)_vec.size() - 1; i++) _os << _vec[i] << ",";
	if (_vec.size() != 0) _os << _vec.back();
	return _os;
}

template <typename dataType>
ofstream& operator<<(ofstream& _ofs, const vector<dataType>& _vec) {
	for (int i = 0; i < (int)_vec.size() - 1; i++) _ofs << _vec[i] << ",";
	if (_vec.size() != 0) _ofs << _vec.back();
	return _ofs;
}


class CsvManager
{
public:
	enum SplitType { Contain, Except };

	static vector<string> Split(string _text, SplitType _split_type, regex _format = regex(R"()"))	// split by regex
	{
		sregex_token_iterator iter, end;
		if (_split_type == Contain) iter = sregex_token_iterator(_text.begin(), _text.end(), _format);
		else if (_split_type == Except) iter = sregex_token_iterator(_text.begin(), _text.end(), _format, -1);
		return vector<string>(iter, end);
	}

	static vector<vector<string>> Read_Csv(string _file_name, SplitType _split_type, regex _format = regex(R"()"))	// read csv
	{
		vector<vector<string>> _file_data;
		string _string_buffer;

		ifstream _ifs(_file_name);
		while (!_ifs.eof()) {
			getline(_ifs, _string_buffer);
			auto _splited_line = Split(_string_buffer, _split_type, _format);
			if (_splited_line.size() > 0 && _splited_line[0] != "") _file_data.push_back(_splited_line);
		}
		_ifs.close();

		return _file_data;
	}

	template <typename dataType>
	static bool Write_Csv(string _file_name, vector<dataType> _file_data)	// write csv
	{
		ofstream _ofs(_file_name);
		if (_ofs.is_open()) {
			for (auto& _line : _file_data) _ofs << _line << '\n';
			_ofs.close();
			return true;
		}
		return false;
	}
};