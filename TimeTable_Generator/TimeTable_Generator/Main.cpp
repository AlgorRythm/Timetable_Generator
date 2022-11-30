
#include "Core/Course.h"
#include "Module/BccCleaning/TimeTable3.h"


void Test_TimeTable3(CourseTable& course_table)
{
	vector<pair<TimeTable3::Priority, Course::CourseNumber>> choice_list = {
		{TimeTable3::First, Course::CourseNumber("RGC0003-29")},
		{TimeTable3::First,Course::CourseNumber("CSE4029-01")},
		{TimeTable3::First,Course::CourseNumber("CSE4035-01")},
		{TimeTable3::First,Course::CourseNumber("CSE4086-01")},
		{TimeTable3::First,Course::CourseNumber("CSE2013-02")},

		// test conflict
		{TimeTable3::Second,Course::CourseNumber("CSE2013-03")},
		{TimeTable3::Second,Course::CourseNumber("CSE4036-01")},

		// test addition
		{TimeTable3::Must,Course::CourseNumber("CSE4041-01")},
		{TimeTable3::Third,Course::CourseNumber("CSE4034-01")},
	};

	TimeTable3 timetable_generator;

	timetable_generator.Construct_Graph(course_table, choice_list);
}


int main()
{
	CourseTable course_table;	// ���̺� ���� (�ڵ����� mdrims���� ���� ���� ������ ������� ���̺� ����)
	//CourseTable course_table(CourseTable::InitType::LoadSavedTable);	// ����� ���̺� �ҷ����� (split ���� ������ ������ ���� ����� �ͺ��� ����)

	cout << "���̺� ũ��: " << course_table.Size() << '\n';

	// Course�� Get_Course() ��� �Լ��� ������ �� �ִ�.
	Course course0 = course_table.Get_Course(0);
	Course course1 = course_table.Get_Course(1);
	Course course2 = course_table.Get_Course(2);
	for (int i = 3; i < 6; i++) cout << course_table.Get_Course(i) << '\n';		// �׽�Ʈ������ 3���� ���
	cout << '\n';


	// Course�� �浹���� �ʴ� Course����Ʈ(��ȣ)�� �������� ����� �� �����̴�.
	vector<int> unconflict_course_with_course0;
	unconflict_course_with_course0 = course_table[0];							// �ε��� ������ ���
	unconflict_course_with_course0 = course_table.Get_Unconflict_Course(0);		// ��� �Լ� ���
	for (int i = 0; i < 10; i++) cout << course_table[20][i] << ' ';			// �׽�Ʈ������ 10���� ��� (20�� Course�� �浹���� �ʴ� Course��ȣ 10�� ���)
	cout << "\n\n";


	// CourseTable ���� ����Ʈ, ���� ����Ʈ�� ���� �����ͼ� ���� �ʹٸ�
	vector<Course> course_list = course_table.Get_Course_List();				// ����Լ��� ���� ����Ʈ�� ������
	vector<vector<int>> course_adj = course_table.Get_Course_Adjacent_List();	// �ɹ��Լ��� ���� ���� ����Ʈ�� ������(���� ��ġ�� �ʴ� ���� ����)



	// �׽�Ʈ
	Test_TimeTable3(course_table);

	return 0;
}