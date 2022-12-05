#pragma once

#include "../../Core/Course.h"
#include "../../Core/Utility.h"
#include "Graph.h"
#include "PriorityValue.h"
#include <vector>
using namespace std;


class TimeTable3
{
public:
	enum Priority { First, Second, Third, Fourth, Fifth, Must };	// 우선순위 목록

private:

public:
	void Construct_Graph(CourseTable &_course_table, vector<pair<TimeTable3::Priority, Course::CourseNumber>> &choice_list)
	{
		for (auto& _course_number : choice_list) cout << _course_number.first << ": " << _course_table.Get_Course(_course_number.second) << '\n';

		for (int i = 0; i < choice_list.size(); i++) {
			auto c1 = _course_table.Get_Course(choice_list[i].second).Get_Course_Number();

			for (int j = i + 1; j < choice_list.size(); j++) {
				auto c2 = _course_table.Get_Course(choice_list[j].second).Get_Course_Number();

				if (_course_table.Is_Conflict(c1, c2)) cout << c1.basic_number << "-" << c1.division << "is conflict with " << c2.basic_number << "-" << c2.division << '\n';
				//if (!_course_table.Is_Conflict(c1, c2)) cout << c1.basic_number << "-" << c1.division << "is not conflict with " << c2.basic_number << "-" << c2.division << '\n';
			}
		}

		Graph< PriorityValue> _temp_graph;


	}

};