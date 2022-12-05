
#include "Core/Course.h"
#include "Module/BccCleaning/TimeTable3.h"
#include "Module/BruteForce/TimeTable2.h"
#include "Module/Hash/TimeTable1.h"

void Test_TimeTable1(CourseTable& course_table)
{
	TimeTable1 timetable1;
	timetable1.generator(course_table);
}

void Test_TimeTable2(CourseTable& course_table) {
	TimeTable2 table;
	table.generator(course_table);
}


int main()
{
	CourseTable course_table;

	// 1. Hash
	Test_TimeTable1(course_table);
	// 2. Brute Force
	Test_TimeTable2(course_table);
	// 3. 

	return 0;
}