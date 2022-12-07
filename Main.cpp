
#include "Core/Course.h"
#include "Module/PersistentKSP/TimeTable3.h"
#include "Module/BruteForce/TimeTable2.h"
#include "Module/Hash/TimeTable1.h"



void BruteForce(CourseTable& course_table, bool _print_time_table)
{
	cout << "Generator1 has been started\n\n";
	TimeTable2 gen2;
	gen2.generator(course_table, _print_time_table);
}

void Hash(CourseTable& course_table, bool _print_time_table)
{
	cout << "Generator2 has been started\n\n";
	TimeTable1 gen1;
	gen1.generator(course_table, _print_time_table);
}


void PersistentKSP(CourseTable& course_table, bool _print_time_table)
{
	cout << "Generator3 has been started\n\n";
	gen3::Generate_TimeTable(course_table, _print_time_table);
}


int main()
{
	CourseTable course_table;

	bool print = true;
	int cmd = 0;
	
	string cmd_guide = "";
	cmd_guide += "0입력: Program exit\n";
	cmd_guide += "1입력: Generator1 (Brute Force)\n";
	cmd_guide += "2입력: Generator2 (Hash)\n";
	cmd_guide += "3입력: Generator3 (Persistent Heap, KSP)\n";
	cmd_guide += "\n명령어를 입력하시오: ";

	while (cout << cmd_guide, cin >> cmd) {
		cout << "\n==================================================\n";
		if (cmd == 0) { cout << "프로그램을 종료합니다" << '\n'; break; }
		else if (cmd < 1 || cmd>3) { cout << "잘못된 명령어 입니다. 다시 입렵해주세요" << '\n'; }
		else {
			if (cmd == 1) BruteForce(course_table, print);
			else if (cmd == 2) Hash(course_table, print);
			else if (cmd == 3) PersistentKSP(course_table, print);
		}
		cout << "==================================================\n\n\n";
	}

	return 0;
}