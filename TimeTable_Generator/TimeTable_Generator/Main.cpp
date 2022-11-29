
#include "Course.h"

int main()
{

	CourseTable course_table;	// 테이블 생성 (자동으로 mdrims에서 받은 엑셀 파일을 기반으로 테이블 생성)
	//CourseTable course_table(CourseTable::InitType::LoadSavedTable);	// 저장된 테이블 불러오기 (split 과정 때문에 오히려 새로 만드는 것보다 느림)


	cout << "테이블 크기: " << course_table.Size() << '\n';


	// Course는 Get_Course() 멤머 함수로 가져올 수 있다.
	Course course0 = course_table.Get_Course(0);
	Course course1 = course_table.Get_Course(1);
	Course course2 = course_table.Get_Course(2);
	for (int i = 3; i < 6; i++) cout << course_table.Get_Course(i) << '\n';		// 테스트용으로 3개만 출력
	cout << '\n';


	// Course와 충돌하지 않는 Course리스트(번호)를 가져오는 방법은 두 가지이다.
	vector<int> unconflict_course_with_course0;
	unconflict_course_with_course0 = course_table[0];							// 인덱스 연산자 방식
	unconflict_course_with_course0 = course_table.Get_Unconflict_Course(0);		// 멤버 함수 방식
	for (int i = 0; i < 10; i++) cout << course_table[20][i] << ' ';			// 테스트용으로 10개만 출력 (20번 Course와 충돌하지 않는 Course번호 10개 출력)
	cout << "\n\n";



	return 0;
}