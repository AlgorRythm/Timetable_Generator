
#include "Core/Course.h"
#include "Module/BccCleaning/TimeTable3.h"
#include "Module/BruteForce/TimeTable2.h"

void timeTable2(CourseTable& course_table) {
	TimeTable2 table;
	table.generator(course_table);
}

int main()
{
	CourseTable course_table;

	// 1.
	// 2. Brute Force
	timeTable2(course_table);

	// 3. 

	return 0;
}