#pragma once

#include "../../Core/Course.h"
#include "../../Core/Utility.h"
#include "WeightedGraph.h"
#include "InputManager.h"
#include <vector>
#include <ctime>
using namespace std;


namespace gen3
{
	using WeightType = long long;
	using Integer = long long;
	using Bits = long long;


	void Generate_TimeTable(CourseTable& course_table, bool _print_time_table = true)
	{
		// get input
		int number_of_course, number_of_essential, number_of_timetable, target_credit, compare_cnt = 0;
		Bits common_conflict;
		vector<Bits> course_conflict;
		vector<WeightType> course_weight;
		vector<Course> course_info;
		if (!InputManager::Get_Input(course_table, number_of_course, number_of_essential, number_of_timetable, common_conflict, course_conflict, course_weight, course_info, target_credit, compare_cnt)) { cout << "\"잘못된 입력입니다. 다시 입력해주세요!!\n\n"; return; }


		// initialize
		int source = 0, sink = (number_of_course * target_credit) + 1, graph_size = sink + 1, level = (target_credit % 3 ? (target_credit / 3) + 1 : target_credit / 3) - number_of_essential;
		function<int(int)> Conv_Index = [&](int _idx) {
			if (_idx == source) return 0;
			if (_idx == sink) return number_of_course + 1;
			return _idx % number_of_course ? _idx % number_of_course : number_of_course;
		};


		// construct graph
		WeightedGraph wGraph(graph_size, 2e18, 1e18, 0);
		for (int v = 1; v <= number_of_course; v++)
			if (course_weight[v] != -1 && (++compare_cnt, !(common_conflict & ((Bits)1 << v)))) wGraph.Add_Directed_Edge(source, v, course_weight[v]);

		for (int u = 1; u <= number_of_course; u++)
			if (course_weight[u] != -1 && (++compare_cnt, !(common_conflict & ((Bits)1 << u)))) wGraph.Add_Directed_Edge(u + ((level - 1) * number_of_course), sink, 0);

		for (int lev = 1; lev < level; lev++) {
			for (int u = 1; u <= number_of_course; u++) {
				if (course_weight[u] == -1 || (++compare_cnt, (common_conflict & ((Bits)1 << u)))) continue;
				for (int v = u + 1; v <= number_of_course; v++) {
					if (course_weight[v] == -1 || (++compare_cnt, (common_conflict & ((Bits)1 << v))) || course_conflict[u] & ((Bits)1 << v)) continue;
					wGraph.Add_Directed_Edge(u + ((lev - 1) * number_of_course), v + (lev * number_of_course), course_weight[v]);
				}
			}
		}


		// generate TimeTable
		clock_t start, finish;
		start = clock();
		auto timetable_list = wGraph.Persistent_Ksp(source, sink, number_of_timetable, course_conflict, Conv_Index, compare_cnt);
		finish = clock();


		// print result
		cout << "\n실행 시간: " << (finish - start) << "ms" << '\n';
		cout << "비교 횟수: " << (compare_cnt) << "회" << '\n';
		cout << "메모리 사용량: " << (double)wGraph.Get_Memeory() / (double)125000 << "mb" << '\n';
		cout << "생성된 시간표: " << timetable_list.size() << "개" << "\n\n";


		int list_cnt = 0;
		vector<string> final_courses;
		string tmp_str1, tmp_str2;

		if (_print_time_table) {
			cout << "\n시간표를 저장하는 중입니다...";
			for (auto& timetable : timetable_list)
			{
				tmp_str1 = string("[") + "시간표" + to_string(++list_cnt) + "],";
				tmp_str2 = ",";

				//cout << "\n[시간표" << ++list_cnt << "] - ";
				for (int i = 1; i <= number_of_course; i++)	// 필수 과목 먼저 출력
					if (course_weight[i] == -1 || (timetable.sequence & ((Bits)1 << (Bits)i))) {
						tmp_str1 += string("{") + course_info[i].Get_Course_Name() + "-" + course_info[i].Get_Course_ID().division + "},";
						tmp_str2 += string("{") + course_info[i].Get_Course_ID().basic_id + "-" + course_info[i].Get_Course_ID().division + " / " + course_info[i].Get_Faculty_In_Charge() + "},";
					}
				final_courses.push_back(tmp_str1);
				final_courses.push_back(tmp_str2);
			}

			if (final_courses.empty())final_courses.push_back("해당 Input으로 생성할 수 있는 시간표가 없습니다!");
			cout << "\n시간표가 \'./Documents/TimeTable/PersistentKSP.csv\'에 저장되었습니다" << '\n';
			CsvManager::Write_Csv("./Documents/TimeTable/PersistentKSP.csv", final_courses);
		}
	}
}