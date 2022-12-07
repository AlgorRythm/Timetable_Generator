#pragma once

#include "../../Core/Course.h"
#include "../../Core/Utility.h"

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class InputManager
{
private:
	using WeightType = long long;
	using Integer = long long;
	using Bits = long long;


	static void Input_Type2(CourseTable& course_table, int& target_credit, int& number_of_timetable, vector<int>& course_index, vector<int>& course_lank, vector<Course>& course_info)	// 과목을 인덱스로 입력 받음
	{
		cin.ignore();	// 단독 실행시 이 부분 삭제가 필요

		string string_buffer;
		vector<string> message = { "학점", "필수", "1순위", "2순위", "3순위","4순위", "5순위", "생성할 시간표 개수" };
		int message_index = 0;
		auto get_string = [&]() {cout << message[message_index++] << ": "; getline(cin, string_buffer); };

		get_string(); target_credit = stoi(string_buffer);
		for (int lank = -1; lank < 5; lank++) {
			get_string();
			for (auto& item : util::Split(string_buffer, util::Contain, regex(R"(\d{1,})"))) {
				auto index = stoi(item);
				course_index.push_back(index);
				course_lank.push_back(lank);
				course_info.push_back(course_table.Get_Course(index));
			}
		}
		get_string(); number_of_timetable = stoi(string_buffer);
	}


public:
	static bool Get_Input(CourseTable &course_table, int& number_of_course, int& number_of_essential, int& number_of_timetable, Bits& common_conflict
		, vector<Bits>& course_conflict, vector<WeightType>& course_weight, vector<Course>& course_info, int& target_credit, int& compare_cnt)
	{
		vector<Integer> x_value(5, 1);
		for (int i = 1; i < 5; i++) x_value[i] = x_value[i - 1] * 100;
		auto Lank_To_Weight = [&](int _lank) {return _lank == -1 ? (Integer)(-1) : x_value[_lank]; };

		vector<int> course_index(1), course_lank(1);
		course_info.assign(1, Course());
		Input_Type2(course_table, target_credit, number_of_timetable, course_index, course_lank, course_info);

		number_of_course = (int)course_info.size() - 1;
		number_of_essential = 0;
		if (number_of_course < 1 || number_of_course > 60) { cout << "\"범위를 벗어났습니다(1 ~ 60)\""; return false; }	// 입력이 잘못되었다면 false 리턴
		course_conflict.assign(number_of_course + 2, (Bits)0);
		course_weight.assign(number_of_course + 1, 0);

		for (int i = 1; i <= number_of_course; i++) {
			if (course_lank[i] == -1) { number_of_essential++; course_weight[i] = -1; }
			else course_weight[i] = Lank_To_Weight(course_lank[i]);
			for (int j = i + 1; j <= number_of_course; j++) {
				if (++compare_cnt, course_table.Is_Conflict(course_index[i], course_index[j])) {
					course_conflict[i] |= ((Bits)1 << j);
					course_conflict[j] |= ((Bits)1 << i);
				}
			}
		}

		bool essential_conflict = false;
		common_conflict = (Bits)0;
		for (int i = 1; i <= number_of_course; i++) {	// 필수 과목 끼리의 충돌 여부 확인
			if (course_lank[i] == -1) {
				if (++compare_cnt, !(common_conflict & ((Bits)1 << i))) common_conflict |= course_conflict[i];
				else essential_conflict = true;
			}
		}

		for (int i = 1; i <= number_of_course; i++)	// 필수 과목과 충돌하는 과목
			if (course_lank[i] != -1) course_conflict[i] |= common_conflict;

		/*cout << "\n[필수 과목]" << (number_of_essential == 0 ? "\n- 필수로 선택해야하는 과목이 없음" : "") << '\n';
		for (int i = 1; i <= number_of_course; i++)
			if (course_lank[i] == -1) cout << course_info[i] << '\n';

		cout << "\n[선택 가능한 일반 과목]" << '\n';
		for (int i = 1; i <= number_of_course; i++)
			if (course_lank[i] != -1 && !(common_conflict & ((Bits)1 << i))) cout << "- 우선도" << course_lank[i] + 1 << ": " << course_info[i] << '\n';

		cout << "\n[선택 불가능한 일반 과목 (필수 과목과 겹침)]" << '\n';
		for (int i = 1; i <= number_of_course; i++)
			if (course_lank[i] != -1 && (common_conflict & ((Bits)1 << i))) cout << course_info[i] << '\n';*/

		if (essential_conflict) { cout << "\n\"잘못된 입력: 필수 과목 사이에서 시간이 겹칩니다!!\"" << '\n'; return false; }

		return true;
	}	
};