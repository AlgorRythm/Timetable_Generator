
#include "Core/Course.h"
#include "Module/PersistentKSP/TimeTable3.h"
#include "Module/BruteForce/TimeTable2.h"
#include "Module/Hash/TimeTable1.h"


void Hash(CourseTable& course_table, bool _print_time_table)
{
	cout << "Generator1 has been started\n\n";
	TimeTable1 gen1;
	gen1.generator(course_table, _print_time_table);
}

void BruteForce(CourseTable& course_table, bool _print_time_table)
{
	cout << "Generator2 has been started\n\n";
	TimeTable2 gen2;
	gen2.generator(course_table, _print_time_table);
}

void PersistentKSP(CourseTable& course_table, bool _print_time_table)
{
	cout << "Generator3 has been started\n\n";
	gen3::Generate_TimeTable(course_table, _print_time_table);
}


int main()
{
	CourseTable course_table;

	int cmd = 0;
	string cmd_guide = "";
	cmd_guide += "0입력: Program exit\n";
	cmd_guide += "1입력: Generator1 (Hash)\n";
	cmd_guide += "2입력: Generator2 (Brute Force)\n";
	cmd_guide += "3입력: Generator3 (Persistent Heap, KSP)\n";
	cmd_guide += "\n명령어를 입력하시오: ";

	while (cout << cmd_guide, cin >> cmd) {
		cout << "\n==================================================\n";
		if (cmd == 0) { cout << "프로그램을 종료합니다" << '\n'; break; }
		else if (cmd < 1 || cmd>3) { cout << "잘못된 명령어 입니다. 다시 입렵해주세요" << '\n'; }
		else {
			if (cmd == 1) Hash(course_table, false);
			else if (cmd == 2) BruteForce(course_table, false);
			else if (cmd == 3) PersistentKSP(course_table, false);
		}
		cout << "==================================================\n\n\n";
	}





	return 0;
}

/*
15
0
5 RGC0003-29 CSE4029-01 CSE4035-01 CSE4086-01 CSE2013-02
2 CSE2013-03 CSE4036-01
1 CSE4034-01
1 CSE4041-01
0
*/

/*
15
-
86,33,37,57,23
24,38
36,
42
-
30


15
86,33,37
57,23
24,38
36,
42
-
30

15
86,33,37
57,23
24,38
36,
42
-
30

21
-
88,224,412,80,53,49,61
44,234,12,23,45,76
252,123,34,99
128,129,32
111,278,249
30000


15
-
88,224,412,80,53,49,61
44,234,12,23,45,76
252,123,34,99
128,129,32
111,278,249
15

*/