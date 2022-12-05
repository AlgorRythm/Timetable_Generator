#pragma once
#include "Course.h"
#include "Utility.h"
#include <vector>
#include <sstream>
#include <tuple>
#include <queue>
#include <algorithm>
#include <stack>
#include <ctime>
using namespace std;

class TimeTable2 {
private:
    double clock_time;
    int compare_cnt;
public:
    int essential_num;
    int input_num;
    int credit;
    int table_num = 1;
    int output_num;
    vector<pair<int, int>> input_courses;   // vector (과목번호, 선호도)
    vector<int> essential_courses;
    vector<vector<pair<int, int>>> combi;   // combi 모음
private:
    bool isVisited[10000] = { false, };
    stack<int> st;
public:
    void generator(CourseTable& _course_table);
    void input_data();
    void brute_force(CourseTable& _course_table);
    void Combination(vector<pair<int, int>>& cmb, int r, int index, int depth);
    bool is_Table_Conflict(CourseTable& _course_table, vector<pair<int, int>>& com_courses);
    bool isMustInclude(vector<pair<int, int>>& arr);
};

// =====================================================
//                  initial 구조 함수
// =====================================================
void TimeTable2::generator(CourseTable& _course_table) {
    // INPUT
    input_data();
    // init CNT
    compare_cnt = 0;
    clock_t start = clock();
    // brute-force
    brute_force(_course_table);
    // calculate CLOCK
    clock_t finish = clock();
    clock_time = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << clock_time << "초" << endl;
}

// =====================================================
//           사용자에게 수강할 과목 입력받는 함수
// =====================================================
void TimeTable2::input_data() {
    string stringBuffer;
    string str;
    int essencial_cnt = 0;
    int cnt = 0;

    cout << "학점: ";
    cin >> credit;
    cout << "필수: ";
    cin >> str;
    istringstream ss0(str);
    while (str!="-" && getline(ss0, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 0));
        input_num++;
    }
    essential_num = input_num;
    cout << "1: ";
    cin >> str;
    istringstream ss1(str);
    while (getline(ss1, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 1));
        input_num++;
    }
    cout << "2: ";
    cin >> str;
    istringstream ss2(str);
    while (getline(ss2, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 2));
        input_num++;
    }
    cout << "3: ";
    cin >> str;
    istringstream ss3(str);
    while (getline(ss3, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 3));
    }
    cout << "4: ";
    cin >> str;
    istringstream ss4(str);
    while (getline(ss4, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 4));
        input_num++;
    }
    cout << "5: ";
    cin >> str;
    istringstream ss5(str);
    while (getline(ss5, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 5));
        input_num++;
    }
    cout << "출력할 시간표 개수: ";
    cin >> output_num;
}

// =====================================================
//       course table이 conflict하는지 체크하는 함수
// =====================================================
bool TimeTable2::is_Table_Conflict(CourseTable& _course_table, vector<pair<int, int>>& com_courses) {
    for (int i = 0; i < com_courses.size(); i++) {
        compare_cnt++;
        auto c1 = _course_table.Get_Course(com_courses[i].first).Get_Course_Number();
        for (int j = i+1; j < com_courses.size(); j++) {
            compare_cnt++;
            auto c2 = _course_table.Get_Course(com_courses[j].first).Get_Course_Number();
            if (_course_table.Is_Conflict(c1, c2)) {
                return true;
            }
        }
    }
    return false;
}

bool TimeTable2::isMustInclude(vector<pair<int, int>>& arr) {
    int must_cnt = 0;
    for (int i = 0; i < arr.size(); i++) {
            compare_cnt++;
        if (arr[i].second == 0) must_cnt++;
    }
    if (must_cnt == essential_num) return true;
    else return false;
}

// =====================================================
//              Brute Force 시행하는 함수
// =====================================================
void TimeTable2::brute_force(CourseTable& _course_table) {
    int now_credit = 0;
    int essential_credit = 0;
    /*for (int i=0; i< input_courses.size(); i++)
        cout <<  input_courses[i].first << " ";
    cout << endl;*/

    // 가능한 모든 조합을 생성하는 함수
    vector<pair<int, int>> cmb;
    Combination(cmb, credit/3, 0, 0);

    // 시간표 
    bool check = false;
    vector<vector<pair<int, int>>> result;
    for (int i = 0; i < combi.size(); i++) {
        if (is_Table_Conflict(_course_table, combi[i]) == false && isMustInclude(combi[i]) == true) {
            result.push_back(combi[i]);
        }
    }

    for (int i = 0; i < result.size(); i++) {
        cout << "\n[ " << "시간표" << table_num++ << " ] ";
        for (int j = 0; j < result[i].size(); j++) {
            cout << "{" << _course_table.Get_Course(result[i][j].first).Get_Course_Name() << "-" << _course_table.Get_Course(result[i][j].first).Get_Course_Number().division << "} ";
        }
        if (table_num == output_num + 1)
            break;
    }
    cout << endl;
    cout << "Compare CNT: " << compare_cnt << endl;
}

/**
// =====================================================
//  Combination 받은 수업들로 모든 경우의 수를 출력하는 함수
// =====================================================
 arr = 대상 배열           comb = 출력 배열
 r = 남은 뽑아야 할 갯수    index = comb 의 인덱스
 depth = 대상 배열에서 뽑을 원소를 결정하는 인덱스
 */
void TimeTable2::Combination(vector<pair<int, int>>& cmb, int r, int index, int depth) { // depth == 점화식의 n역할
    compare_cnt++;
    if (r == 0) { // 뽑아야할 갯수가 모두 찬 경우에는 comb 에 들어있는 값 모두 출력
        combi.push_back(cmb);
        cmb.pop_back();
    }
    else if (depth == input_courses.size()) {   // 뽑을 원소 인덱스가 대상 배열의 마지막까지 온 경우 return
         return;
    }
    else {
        // arr[depth] 를 뽑은 경우
        if (cmb.size() <= index) cmb.push_back(make_pair(input_courses[depth].first, input_courses[depth].second));
        else {
            cmb[index].first = input_courses[depth].first;
            cmb[index].second = input_courses[depth].second;
        }
        Combination(cmb, r - 1, index + 1, depth + 1); // n-1Cr-1: 특정 원소를 뽑은 경우

        // arr[depth] 를 뽑지 않은 경우
        Combination(cmb, r, index, depth + 1); // n-1Cr: 특정 원소를 뽑지 않은 경우
    }
}