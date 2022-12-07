#pragma once
#pragma once

#include "../../Core/Course.h"
#include "../../Core/Utility.h"
#include <sstream>
#include <tuple>
#include <queue>
#include <algorithm>
#include <ctime>

using namespace std;

class TimeTable1
{
public:
	typedef long long lint;
	int essential_cnt;
	int input_cnt;
	int credit;
	int table_num = 1;
	int make_cnt = 0;
	int output_num;
	int cmp_cnt = 0;
	vector<tuple <int, int>> input_courses;	// 사용자 입력 courses list => (int 과목번호, int 선호도)
	vector<int> essential_courses;			// 필수 과목 list => (int 과목번호)
	vector<int> accumulate_courses;			// 누적 과목 list
	vector<string> final_courses;			// 최종 출력용 list


	lint mcnt1 = 0;	// input_courses
	lint mcnt2 = 0;	// essential_courses
	lint mcnt3 = 0;	// accumulate_courses
	lint mcnt4 = 0;	// final_courses

	void generator(CourseTable& _course_table, bool _print_time_table) {
		vector<string> message = { "학점", "필수", "1순위", "2순위", "3순위","4순위", "5순위", "생성할 시간표 개수" };
		int message_index = 0;
		cout << message[message_index++] << ": ";
		cin >> credit;

		input_data();

		clock_t start, finish;
		start = clock();
		if (Check_Essential_Conflict(_course_table) == 1)	// essential 과목 충돌 체크
			HashFunctoin(_course_table);					// 나머지 Hash
		finish = clock();

		cout << "\n실행 시간: " << finish - start << "ms" << endl;
		cout << "비교 횟수: " << cmp_cnt << "회" << endl;
		cout << "메모리 사용량: " << (double)(mcnt1 * sizeof(tuple<int, int>) + mcnt2 * sizeof(int) + mcnt3 * sizeof(int) + mcnt4 * sizeof(char)) / (double)125000 << "mb" << endl;
		cout << "생성된 시간표: " << final_courses.size() << "개" << endl << endl;
		if (_print_time_table) print_total_table();
	}

	// main function
	int HashFunctoin(CourseTable& _course_table) {
		int now_credit = 0;
		int essential_credit = 0;
		int now_print_num = 0;

		// accumulate_courses 초기화
		for (int i = 0; i < essential_cnt; i++) {
			accumulate_courses.push_back(get<0>(input_courses[i]));
			mcnt3++;
			now_credit += 3;
			essential_credit += 3;
		}

		// 만약 credit과 essential이 동일한데 충돌하면 위에서 예외처리 해둠.
		// 만약 credit과 essential이 동일하다면 하나밖에 없으므로, 출력하고 바로 종료함.
		if (now_credit == credit) {
			add_total_table(_course_table, accumulate_courses);
			return 0;
		}
		else if (now_credit >= credit) {		// now_credit >= credit 경우 추가하기
			cout << "필수 과목이 입력한 학점보다 많습니다." << endl;
			return 0;
		}

		// Is_Course_Conflict 한 뒤에 conflict 안나면 추가함.
		MakingTable(_course_table, accumulate_courses, essential_cnt, essential_credit);
	}

	// 가능한 Table 확인하는 함수
	void MakingTable(CourseTable& _course_table, vector<int> _accumulate_courses, int currnet_index, int _now_credit) {
		for (int i = currnet_index; i < input_cnt; i++) {
			auto nowC = _course_table.Get_Course(get<0>(input_courses[i]));

			if (Is_Course_Conflict(_course_table, _accumulate_courses, nowC) == false) {
				_accumulate_courses.push_back(get<0>(input_courses[i]));
				mcnt3++;
				_now_credit += 3;

				if (_now_credit == credit)
					add_total_table(_course_table, _accumulate_courses);
				else
					MakingTable(_course_table, _accumulate_courses, i + 1, _now_credit);

				_accumulate_courses.pop_back();
				_now_credit -= 3;
			}
		}
	}

	// essential 과목에서 conflict나는지 체크하는 함수 => conflict 발생시 바로 종료
	int Check_Essential_Conflict(CourseTable& _course_table) {
		for (int i = 0; i < essential_cnt; i++) {
			auto c1 = _course_table.Get_Course(get<0>(input_courses[i])).Get_Course_ID();
			for (int j = i + 1; j < essential_cnt; j++) {
				auto c2 = _course_table.Get_Course(get<0>(input_courses[j])).Get_Course_ID();
				++cmp_cnt;
				if (_course_table.Is_Conflict(c1, c2)) {
					cout << "필수 과목" << c1.basic_id << "과 " << c2.basic_id << "가 충돌하여 시간표가 존재할 수 없습니다." << endl;
					return 0;
				}
			}
		}
		return 1;
	}

	// 누적 course(accumulate)과 현재 입력한 course가 conflict한지 체크하는 함수
	bool Is_Course_Conflict(CourseTable& _course_table, vector<int> _accumulate_courses, Course& c) {
		auto c1 = c.Get_Course_ID();
		for (int i = 0; i < _accumulate_courses.size(); i++) {
			auto c2 = _course_table.Get_Course(_accumulate_courses[i]).Get_Course_ID();
			++cmp_cnt;
			if (_course_table.Is_Conflict(c1, c2)) {
				//cout << "**Is_Course_Conflict**" << _course_table.Get_Course(c1).Get_Course_Name() << _course_table.Get_Course(c1).Get_Course_Number().division << ", " << _course_table.Get_Course(c2).Get_Course_Name() << _course_table.Get_Course(c2).Get_Course_Number().division << endl;
				return true;
			}
		}
		return false;
	}

	// 사용자에게 과목 입력받는 함수
	void input_data() {
		vector<string> message = { "학점", "필수", "1순위", "2순위", "3순위","4순위", "5순위", "생성할 시간표 개수" };
		int message_index = 1;

		string stringBuffer;
		string str;

		cout << message[message_index++] << ": ";
		cin >> str;
		istringstream ss0(str);
		while (getline(ss0, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 0));
			mcnt1++;
			essential_courses.push_back(stoi(stringBuffer));
			mcnt2++;
		}

		cout << message[message_index++] << ": ";
		cin >> str;
		istringstream ss1(str);
		while (getline(ss1, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 1));
			mcnt1++;
		}

		cout << message[message_index++] << ": ";
		cin >> str;
		istringstream ss2(str);
		while (getline(ss2, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 2));
			mcnt1++;
		}

		cout << message[message_index++] << ": ";
		cin >> str;
		istringstream ss3(str);
		while (getline(ss3, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 3));
			mcnt1++;
		}

		cout << message[message_index++] << ": ";
		cin >> str;
		istringstream ss4(str);
		while (getline(ss4, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 4));
			mcnt1++;
		}

		cout << message[message_index++] << ": ";
		cin >> str;
		istringstream ss5(str);
		while (getline(ss5, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 5));
			mcnt1++;
		}
		cout << message[message_index++] << ": ";
		cin >> output_num;

		input_cnt = input_courses.size();
		essential_cnt = essential_courses.size();
	}

	// 생성된 timetable 저장하는 함수	
	void add_total_table(CourseTable& _course_table, vector<int> _accumulate_courses) {
		if (make_cnt++ < output_num)
		{
			string tmp_str = string("[ ") + "시간표" + to_string(table_num) + " ] ";
			table_num++;

			for (int i = 0; i < _accumulate_courses.size(); i++) {
				tmp_str += "{" + _course_table.Get_Course(_accumulate_courses[i]).Get_Course_Name() + "-" + _course_table.Get_Course(_accumulate_courses[i]).Get_Course_ID().division + "} ";
			}
			final_courses.push_back(tmp_str);
			mcnt4 += tmp_str.size();
		}
	}

	// 최종 table list를 출력하는 함수
	void print_total_table() {
		if (final_courses.empty())final_courses.push_back("해당 Input으로 생성할 수 있는 시간표가 없습니다!");
		cout << "\n시간표가 \'./Documents/TimeTable/Hash.csv\'에 저장되었습니다" << '\n';
		CsvManager::Write_Csv("./Documents/TimeTable/Hash.csv", final_courses);
	}

	// 테스트용: accumulate_courses 출력하는 함수
	void print_accumulate_courses() {
		cout << "<<accumulate_courses 출력>>" << endl;
		cout << "size: " << accumulate_courses.size() << endl;
		for (int i = 0; i < accumulate_courses.size(); i++) {
			cout << accumulate_courses[i] << " ";
		}
		cout << endl;
	}

	// 테스트용: input_courses 출력하는 함수
	void print_input_courses() {
		cout << "<<input_courses 출력>>" << endl;
		cout << "size: " << input_cnt << endl;
		for (int i = 0; i < input_courses.size(); i++) {
			cout << get<0>(input_courses[i]) << "(" << get<1>(input_courses[i]) << ") ";
		}
		cout << endl;
	}

	// 테스트용: essential_courses 출력하는 함수
	void print_essential_courses() {
		cout << "<<essential_courses 출력>>" << endl;
		cout << "size: " << essential_cnt << endl;
		for (int i = 0; i < essential_courses.size(); i++) {
			cout << essential_courses[i] << " ";
		}
		cout << endl;
	}
};