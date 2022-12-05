#pragma once

#include "../../Core/Course.h"
#include "../../Core/Utility.h"
#include <vector>
#include<sstream>
#include <tuple>
#include <queue>
#include <algorithm>
#include <stack>

using namespace std;

class TimeTable1
{
public:
	int essential_cnt;
	int input_cnt;
	int credit;
	int table_num = 1;
	int output_num;
	vector<tuple <int, int>> input_courses;	// 사용자 입력 courses list => (int 과목번호, int 선호도)
	vector<int> essential_courses;			// 필수 과목 list => (int 과목번호)
	vector<int> accumulate_courses;			// 누적 과목 list
private:

public:
	void generator(CourseTable& _course_table) {
		cout << "학점: ";
		cin >> credit;

		input_data();

		//print_input_courses();
		//print_essential_courses();

		// essential 과목 충돌 체크
		Check_Essential_Conflict(_course_table);

		// 나머지 Brute-Force
		HashFunctoin(_course_table);
	}

	// 사용자에게 과목 입력받는 함수
	void input_data() {
		string stringBuffer;
		string str;

		cout << "필수: ";
		cin >> str;
		istringstream ss0(str);
		while (getline(ss0, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 0));
			essential_courses.push_back(stoi(stringBuffer));
		}

		cout << "1: ";
		cin >> str;
		istringstream ss1(str);
		while (getline(ss1, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 1));
		}

		cout << "2: ";
		cin >> str;
		istringstream ss2(str);
		while (getline(ss2, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 2));
		}

		cout << "3: ";
		cin >> str;
		istringstream ss3(str);
		while (getline(ss3, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 3));
		}

		cout << "4: ";
		cin >> str;
		istringstream ss4(str);
		while (getline(ss4, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 4));
		}

		cout << "5: ";
		cin >> str;
		istringstream ss5(str);
		while (getline(ss5, stringBuffer, ',')) {
			if (stringBuffer == "-") {
				break;
			}
			input_courses.push_back(make_tuple(stoi(stringBuffer), 5));
		}
		cout << "출력할 시간표 갯수: ";
		cin >> output_num;

		input_cnt = input_courses.size();
		essential_cnt = essential_courses.size();
	}

	// essential 과목에서 conflict나는지 체크하는 함수 => conflict 발생시 바로 종료
	void Check_Essential_Conflict(CourseTable& _course_table) {
		for (int i = 0; i < essential_cnt; i++) {
			auto c1 = _course_table.Get_Course(get<0>(input_courses[i])).Get_Course_Number();
			for (int j = i + 1; j < essential_cnt; j++) {
				auto c2 = _course_table.Get_Course(get<0>(input_courses[j])).Get_Course_Number();
				if (_course_table.Is_Conflict(c1, c2)) {
					cout << "필수 과목" << c1.basic_number << "과 " << c2.basic_number << "가 충돌하여 시간표가 존재할 수 없습니다." << endl;
					exit(200);
				}
			}
		}
	}
	
	// 누적 course(accumulate)과 현재 course가 conflict한지 체크하는 함수
	bool Is_Course_Conflict(CourseTable& _course_table, Course& c) {
		auto c1 = c.Get_Course_Number();
		for (int i = 0; i < accumulate_courses.size(); i++) {
			auto c2 = _course_table.Get_Course(accumulate_courses[i]).Get_Course_Number();
			if (_course_table.Is_Conflict(c1, c2)) {
				//cout << "**Is_Course_Conflict**" << _course_table.Get_Course(c1).Get_Course_Name() << _course_table.Get_Course(c1).Get_Course_Number().division << ", " << _course_table.Get_Course(c2).Get_Course_Name() << _course_table.Get_Course(c2).Get_Course_Number().division << endl;
				return true;
			}
		}
		return false;
	}


	// main function
	void HashFunctoin(CourseTable& _course_table) {
		int now_credit = 0;
		int essential_credit = 0;
		int now_print_num = 0;

		for (int i = 0; i < essential_cnt; i++) {	// 초기화
			accumulate_courses.push_back(get<0>(input_courses[i]));
			now_credit += 3;
			essential_credit += 3;
		}
		//cout << "now_credit: " << now_credit << endl;
		//cout << "essential_credit: " << essential_credit << endl;

		// 만약 credit과 essential이 동일한데 충돌하면 위에서 예외처리 해둠.
		// 만약 credit과 essential이 동일하다면 하나밖에 없으므로, 출력하고 바로 종료함.
		if (now_credit == credit) {
			print_table(_course_table);
			exit(200);
		}
		else if (now_credit >= credit) {		// now_credit >= credit 경우 추가하기
			exit(200);
		}



		// Is_Course_Conflict 한 뒤에 conflict 안나면 추가함.
		for (int i = essential_cnt - 1; i < input_cnt-1; i++) {
			//// 만약 essential이 없었다면 하나 생성해줌.
			//if (i == -1 && essential_cnt == 0) {
			//	accumulate_courses.push_back(get<0>(input_courses[0]));
			//	i++;
			//	now_credit += 3;
			//}
			for (int j = i+1; j < input_cnt; j++) {
				auto tmp_j = _course_table.Get_Course(get<0>(input_courses[j]));
				if (Is_Course_Conflict(_course_table, tmp_j) == false) {
					accumulate_courses.push_back(get<0>(input_courses[j]));
					now_credit += 3;
				}
				else {
					continue;
				}

				if (now_credit == credit) {
					print_table(_course_table);
					accumulate_courses.pop_back();
					now_credit -= 3;
					//continue;
					if (now_credit < essential_credit) { exit(200); }
				}
			}
			if (now_credit < essential_credit) { exit(200); }
			while (now_credit != essential_credit) {
				accumulate_courses.pop_back();
				now_credit -= 3;
			}
			//continue;
		}

		 
		//for (int i = essential_cnt; i < input_cnt; i++) {
		//	//auto tmp_c = _course_table.Get_Course(get<0>(input_courses[i]));
		//	//if (Is_Course_Conflict(_course_table, tmp_c) == false) {	// 충돌 안하면 push
		//	//	accumulate_courses.push_back(get<0>(input_courses[i]));
		//	//	now_credit += 3;
		//	//}
		//	//// credit 일치 체크
		//	//if (now_credit == credit) {
		//	//	cout << "****";
		//	//	print_table(_course_table);
		//	//	accumulate_courses.pop_back();
		//	//	now_credit -= 3;
		//	//	if (now_credit < essential_credit) { exit(200); }
		//	//}

		//	for (int j = i + 1; j < input_cnt; j++) {
		//		auto tmp_c = _course_table.Get_Course(get<0>(input_courses[j]));
		//		if (Is_Course_Conflict(_course_table, tmp_c) == false) {	// 충돌 안하면 push
		//			accumulate_courses.push_back(get<0>(input_courses[i]));
		//			now_credit += 3;
		//		}
		//		else {
		//			continue;
		//		}
		//		// credit 일치 체크
		//		if (now_credit == credit) {
		//			print_table(_course_table);
		//			accumulate_courses.pop_back();
		//			now_credit -= 3;
		//			if (now_credit < essential_credit) { exit(200); }
		//		}
		//	}
		//	accumulate_courses.pop_back();
		//	now_credit -= 3;
		//	if (now_credit < essential_credit) { exit(200); }
		//}
	}


	// credit이 같은지 판단하는 함수
	bool Is_Same_Credit(int& now_credit) {
		if (now_credit == credit) { return true; }
		else { return false; }
	}

	// 생성된 timetable 출력하는 함수
	void print_table(CourseTable& _course_table) {
		cout << "[ " << "시간표" << table_num++ << " ] ";
		for (int i = 0; i < accumulate_courses.size(); i++) {
			cout << _course_table.Get_Course(accumulate_courses[i]).Get_Course_Name() << "-" << _course_table.Get_Course(accumulate_courses[i]).Get_Course_Number().division << " ";
		}
		cout << endl;
		output_num--;

		if (output_num == 0) {
			exit(200);
		}
	}

	// 테스트용: accumulate_courses 출력하는 함수
	void print_accumulate_courses() {
		cout << "<<accumulate_courses 출력>>" << endl;
		cout << "size: " << input_cnt << endl;
		for (int i = 0; i < accumulate_courses.size(); i++) {
			cout << accumulate_courses[i] << endl;
		}
	}

	// 테스트용: input_courses 출력하는 함수
	void print_input_courses() {
		cout << "<<input_courses 출력>>" << endl;
		cout << "size: " << input_cnt << endl;
		for (int i = 0; i < input_courses.size(); i++) {
			cout << get<0>(input_courses[i]) << ", " << get<1>(input_courses[i]) << endl;
		}
	}

	// 테스트용: essential_courses 출력하는 함수
	void print_essential_courses() {
		cout << "<<essential_courses 출력>>" << endl;
		cout << "size: " << essential_cnt << endl;
		for (int i = 0; i < essential_courses.size(); i++) {
			cout << essential_courses[i] << endl;
		}
	}
};