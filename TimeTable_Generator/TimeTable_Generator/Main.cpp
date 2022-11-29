
#include "Course.h"

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



	return 0;
}