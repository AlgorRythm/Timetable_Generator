#pragma once

#include "../../Core/Course.h"
#include "../../Core/Utility.h"
#include <vector>
#include<sstream>
#include <tuple>
#include <queue>
#include <algorithm>
#include <stack>
#include <ctime>	// �߰�

using namespace std;

class TimeTable1
{
public:
	int essential_cnt;
	int input_cnt;
	int credit;
	int table_num = 1;
	int output_num;
	int cmp_cnt = 0;
	vector<tuple <int, int>> input_courses;	// ����� �Է� courses list => (int �����ȣ, int ��ȣ��)
	vector<int> essential_courses;			// �ʼ� ���� list => (int �����ȣ)
	vector<int> accumulate_courses;			// ���� ���� list
private:

public:
	void generator(CourseTable& _course_table) {
		cout << "����: ";
		cin >> credit;

		input_data();

		clock_t start, finish;
		start = clock();
		
		cout << "================================" << endl;
		if (Check_Essential_Conflict(_course_table) == 1)	// essential ���� �浹 üũ
			HashFunctoin(_course_table);					// ������ Hash
		finish = clock();
		cout << "================================" << endl;
		cout << "����ð�: " << finish - start << "ms" << endl;
		cout << "��Ƚ��: " << cmp_cnt << "ȸ" << endl;
	}

	// main function
	int HashFunctoin(CourseTable& _course_table) {
		int now_credit = 0;
		int essential_credit = 0;
		int now_print_num = 0;

		// accumulate_courses �ʱ�ȭ
		for (int i = 0; i < essential_cnt; i++) {
			accumulate_courses.push_back(get<0>(input_courses[i]));
			now_credit += 3;
			essential_credit += 3;
		}

		// ���� credit�� essential�� �����ѵ� �浹�ϸ� ������ ����ó�� �ص�.
		// ���� credit�� essential�� �����ϴٸ� �ϳ��ۿ� �����Ƿ�, ����ϰ� �ٷ� ������.
		if (now_credit == credit) {
			print_table(_course_table, accumulate_courses);
			return 0;
		}
		else if (now_credit >= credit) {		// now_credit >= credit ��� �߰��ϱ�
			cout << "�ʼ� ������ �Է��� �������� �����ϴ�." << endl;
			return 0;
		}

		// Is_Course_Conflict �� �ڿ� conflict �ȳ��� �߰���.
		MakingTable(_course_table, accumulate_courses, essential_cnt, essential_credit);
	}

	// ������ Table Ȯ���ϴ� �Լ�
	void MakingTable(CourseTable& _course_table, vector<int> _accumulate_courses, int currnet_index, int _now_credit) {
		for (int i = currnet_index; i < input_cnt; i++) {
			auto nowC = _course_table.Get_Course(get<0>(input_courses[i]));

			if (Is_Course_Conflict(_course_table, _accumulate_courses, nowC) == false) {
				_accumulate_courses.push_back(get<0>(input_courses[i]));
				_now_credit += 3;

				if (_now_credit == credit)
					print_table(_course_table, _accumulate_courses);
				else
					MakingTable(_course_table, _accumulate_courses, i + 1, _now_credit);

				_accumulate_courses.pop_back();
				_now_credit -= 3;
			}
		}
	}

	// essential ���񿡼� conflict������ üũ�ϴ� �Լ� => conflict �߻��� �ٷ� ����
	int Check_Essential_Conflict(CourseTable& _course_table) {
		for (int i = 0; i < essential_cnt; i++) {
			auto c1 = _course_table.Get_Course(get<0>(input_courses[i])).Get_Course_Number();
			for (int j = i + 1; j < essential_cnt; j++) {
				auto c2 = _course_table.Get_Course(get<0>(input_courses[j])).Get_Course_Number();
				++cmp_cnt;
				if (_course_table.Is_Conflict(c1, c2)) {
					cout << "�ʼ� ����" << c1.basic_number << "�� " << c2.basic_number << "�� �浹�Ͽ� �ð�ǥ�� ������ �� �����ϴ�." << endl;
					return 0;
				}
			}
		}
		return 1;
	}
	
	// ���� course(accumulate)�� ���� �Է��� course�� conflict���� üũ�ϴ� �Լ�
	bool Is_Course_Conflict(CourseTable& _course_table, vector<int> _accumulate_courses, Course& c) {
		auto c1 = c.Get_Course_Number();
		for (int i = 0; i < _accumulate_courses.size(); i++) {
			auto c2 = _course_table.Get_Course(_accumulate_courses[i]).Get_Course_Number();
			++cmp_cnt;
			if (_course_table.Is_Conflict(c1, c2)) {
				//cout << "**Is_Course_Conflict**" << _course_table.Get_Course(c1).Get_Course_Name() << _course_table.Get_Course(c1).Get_Course_Number().division << ", " << _course_table.Get_Course(c2).Get_Course_Name() << _course_table.Get_Course(c2).Get_Course_Number().division << endl;
				return true;
			}
		}
		return false;
	}
	
	// ����ڿ��� ���� �Է¹޴� �Լ�
	void input_data() {
		string stringBuffer;
		string str;

		cout << "�ʼ�: ";
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
		cout << "����� �ð�ǥ ����: ";
		cin >> output_num;

		input_cnt = input_courses.size();
		essential_cnt = essential_courses.size();
	}

	// ������ timetable ����ϴ� �Լ�	
	void print_table(CourseTable& _course_table, vector<int> _accumulate_courses) {
		cout << "[ " << "�ð�ǥ" << table_num++ << " ] ";
		for (int i = 0; i < _accumulate_courses.size(); i++) {
			cout << _course_table.Get_Course(_accumulate_courses[i]).Get_Course_Name() << "-" << _course_table.Get_Course(_accumulate_courses[i]).Get_Course_Number().division << " ";
		}
		cout << endl;
	}



	// �׽�Ʈ��: accumulate_courses ����ϴ� �Լ�
	void print_accumulate_courses() {
		cout << "<<accumulate_courses ���>>" << endl;
		cout << "size: " << accumulate_courses.size() << endl;
		for (int i = 0; i < accumulate_courses.size(); i++) {
			cout << accumulate_courses[i] << " ";
		}
		cout << endl;
	}

	// �׽�Ʈ��: input_courses ����ϴ� �Լ�
	void print_input_courses() {
		cout << "<<input_courses ���>>" << endl;
		cout << "size: " << input_cnt << endl;
		for (int i = 0; i < input_courses.size(); i++) {
			cout << get<0>(input_courses[i]) << "(" << get<1>(input_courses[i]) << ") ";
		}
		cout << endl;
	}

	// �׽�Ʈ��: essential_courses ����ϴ� �Լ�
	void print_essential_courses() {
		cout << "<<essential_courses ���>>" << endl;
		cout << "size: " << essential_cnt << endl;
		for (int i = 0; i < essential_courses.size(); i++) {
			cout << essential_courses[i] << " ";
		}
		cout << endl;
	}
};