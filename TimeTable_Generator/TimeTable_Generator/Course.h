#pragma once

#include "FileManager.h"
#include "Utility.h"

#include <iostream>
#include <vector>
#include <string>
using namespace std;


class Course
{
public:
	struct CourseNumber {
		string basic_number, division;
		CourseNumber() :CourseNumber(string(), string()) {}
		CourseNumber(string _basic_number, string _division) :basic_number(_basic_number), division(_division) {}
	};

	struct LectureTime {
		string day;
		int start_time, end_time;
		LectureTime() :LectureTime(string(), 0, 0) {}
		LectureTime(string _day, int _start_time, int _end_time) :day(_day), start_time(_start_time), end_time(_end_time) {}
	};


private:
	string _Curriculum;						// 교과과정
	CourseNumber _Course_Number;			// 학수번호
	string _Course_Name;					// 교과목명
	int _Credit;							// 학점
	string _Faculty_In_Charge;				// 담당교수
	vector<LectureTime> _Lecture_Times;		// 강의시간


public:
	Course(string _curriculum, CourseNumber _course_number, string _course_name, int _credit, string _faculty_in_charge, vector<LectureTime> _lecture_days) :
		_Curriculum(_curriculum), _Course_Number(_course_number), _Course_Name(_course_name), _Credit(_credit), _Faculty_In_Charge(_faculty_in_charge), _Lecture_Times(_lecture_days) {}

	friend ostream& operator << (ostream& _os, const Course& _line) {		// 출력 연산자 오버로딩
		_os << _line._Curriculum << "," << _line._Course_Number.basic_number << "," << _line._Course_Number.division << "," << _line._Course_Name << "," << _line._Credit << "," << _line._Faculty_In_Charge;
		for (auto& _time : _line._Lecture_Times) _os << "," << _time.day << "," << _time.start_time << "," << _time.end_time;
		return (_os);
	}
	friend ofstream& operator << (ofstream& _ofs, const Course& _line) {		// 파일 출력 연산자 오버로딩
		_ofs << _line._Curriculum << "," << _line._Course_Number.basic_number << "," << _line._Course_Number.division << "," << _line._Course_Name << "," << _line._Credit << "," << _line._Faculty_In_Charge;
		for (auto& _time : _line._Lecture_Times) _ofs << "," << _time.day << "," << _time.start_time << "," << _time.end_time;
		return (_ofs);
	}

	string Get_Curriculum() { return _Curriculum; }
	CourseNumber Get_Course_Number() { return _Course_Number; }
	string Get_Course_Name() { return _Course_Name; }
	int Get_Credit() { return _Credit; }
	string Get_Faculty_In_Charge() { return _Faculty_In_Charge; }
	vector<LectureTime> Get_Lecture_Times() { return _Lecture_Times; }
};




class CourseTable
{
public: enum InitType { LoadSavedTable, CreateNewTable };	// { 기존 데이터를 불러오기, 새로운 데이터 만들기 }
private:
	vector<Course> _Course_List;
	vector<vector<int>> _Unconflict_Course;


public:
	CourseTable(InitType _init_type = CreateNewTable) :_Course_List(vector<Course>()), _Unconflict_Course(vector<vector<int>>()) { _init_type == LoadSavedTable ? Load_Saved_Table() : (Construct_List(), Construct_Adjacent_List()); }
	~CourseTable() { _Course_List.clear(); _Unconflict_Course.clear(); }

	vector<int>& operator[](int _course_number) { return _Unconflict_Course[_course_number]; }
	Course& Get_Course(int _course_number) { return _Course_List[_course_number]; }
	vector<int>& Get_Unconflict_Course(int _course_number) { return _Unconflict_Course[_course_number]; }


private:
	void Load_Saved_Table()	// 기존에 저장된 파일로 부터 데이터를 불러온다 (course_list.csv, unconflict_course.csv)
	{
		util::BroadCast("Course List 불러오는 중...");
		for (auto& _file_line : CsvManager::Read_Csv("course_list.csv", CsvManager::Except, regex(R"(,)"))) {
			auto _course_number = Course::CourseNumber(_file_line[1], _file_line[2]);
			auto _lecture_times = vector<Course::LectureTime>();
			for (int i = 6; i < _file_line.size(); i += 3) _lecture_times.push_back(Course::LectureTime(_file_line[i], stoi(_file_line[i + 1]), stoi(_file_line[i + 2])));
			_Course_List.push_back(Course(_file_line[0], _course_number, _file_line[3], stoi(_file_line[4]), _file_line[5], _lecture_times));
		}
		util::BroadCast("\rCourse List 불러오기 완료!!\n");

		util::BroadCast("Course Table 불러오는 중...");
		for (auto& _file_line : CsvManager::Read_Csv("unconflict_course.csv", CsvManager::Except, regex(R"(,)"))) {
			_Unconflict_Course.push_back(vector<int>());
			for (auto& _idx : _file_line) _Unconflict_Course.back().push_back(stoi(_idx));
		}
		util::BroadCast("\rCourse Table 불러오기 완료!!\n\n");
	}

	void Construct_List()	// mDrims 데이터 파일을 읽고 Course List를 새로 구성한다
	{
		util::BroadCast("Course List 생성 중...");
		auto _Add_Course = [&](string _file_name) {	// mDrims 데이터 파일을 읽고 Course List를 생성하는 함수
			for (auto& _file_line : CsvManager::Read_Csv(_file_name, CsvManager::Contain, regex(R"("\S{1,}"|[^,]{1,})"))) {
				if (_file_line.size() < 6) continue;

				auto _course_number = util::Split(_file_line[1], util::Except, regex(R"(-)"));
				auto _course_name = string();
				auto _lecture_times = vector<Course::LectureTime>();

				for (auto& _name_part : util::Split(_file_line[2], util::Contain, regex(R"([^,"]{1,})"))) _course_name += _name_part + "/"; _course_name.pop_back();
				for (auto& _time_part : util::Split(_file_line[5], util::Contain, regex(R"([^,"]{1,})"))) {
					auto _lecture_day = util::Split(_time_part, util::Except, regex(R"(\d{1,}.\d{1,}-\d{1,}.\d{1,}/\d{1,}:\d{1,}-\d{1,}:\d{1,})"));
					for (auto& _block : util::Split(_time_part, util::Contain, regex(R"(\d{1,})"))) _lecture_day.push_back(_block);
					_lecture_times.push_back(Course::LectureTime(_lecture_day[0], stoi(_lecture_day[5]) * 60 + stoi(_lecture_day[6]), stoi(_lecture_day[7]) * 60 + stoi(_lecture_day[8])));
				}
				_Course_List.push_back(Course(_file_line[0], Course::CourseNumber(_course_number[0], _course_number[1]), _course_name, stoi(_file_line[3]), _file_line[4], _lecture_times));
			}
		};

		for (auto& _file_name : vector<string>{ "major.csv" ,"common.csv", "basic.csv" }) _Add_Course(_file_name);

		CsvManager::Write_Csv("course_list.csv", _Course_List);
		util::BroadCast("\rCourse List 생성 완료...\n");
	}

	void Construct_Adjacent_List()	// 새로 구성된 Course List로 Unconflict Course를 구성한다
	{
		util::BroadCast("Course Table 생성 중...");
		auto _Is_Conflict = [&](Course& _course_i, Course& _course_j) {	// 두 과목의 충돌 여부 (학수번호 앞자리, 강의 시간)
			if (_course_i.Get_Course_Number().basic_number == _course_j.Get_Course_Number().basic_number) return true;
			for (auto& _time_i : _course_i.Get_Lecture_Times())
				for (auto& _time_j : _course_j.Get_Lecture_Times()) if (_time_i.day == _time_j.day && _time_i.start_time < _time_j.end_time && _time_i.end_time > _time_j.start_time) return true;
			return false;
		};

		_Unconflict_Course.assign(_Course_List.size(), vector<int>());
		for (int i = 0; i < _Course_List.size(); i++) {
			for (int j = i + 1; j < _Course_List.size(); j++) {
				if (!_Is_Conflict(_Course_List[i], _Course_List[j])) { _Unconflict_Course[i].push_back(j); _Unconflict_Course[j].push_back(i); }
			}
		}
		CsvManager::Write_Csv("unconflict_course.csv", _Unconflict_Course);
		util::BroadCast("\rCourse Table 생성 완료...\n\n");
	}


public:
	size_t Size() { return _Course_List.size(); }	// Course 개수를 return

	vector<Course> Get_Course_List() { return _Course_List; }	// Course List return
	
	vector<vector<int>> Get_Course_Adjacent_List() { return _Unconflict_Course; }	// Adjacent List for Unconflict Course return
};