#pragma once
#include "../../Core/Course.h"
#include "../../Core/Utility.h"
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
    int memory_cnt1 = 0;	// vector<pair<int, int>>
    int memory_cnt2 = 0;	// int
    int memory_cnt3 = 0;	// char
    int memory_cnt4 = 0;	// bool

public:
    int essential_num;
    int input_num;
    int credit;
    int table_num = 1;
    int output_num;
    int make_cnt = 0;
    vector<pair<int, int>> input_courses;   // vector (�����ȣ, ��ȣ��)
    vector<int> essential_courses;
    vector<vector<pair<int, int>>> combi;   // combi ����
    vector<vector<pair<int, int>>> result;  // ���� ���
private:
    bool isVisited[10000] = { false, };
    stack<int> st;
public:
    void generator(CourseTable& _course_table, bool _print_time_table);
    void input_data();
    void brute_force(CourseTable& _course_table);
    void Combination(vector<pair<int, int>>& cmb, int r, int index, int depth);
    bool is_Table_Conflict(CourseTable& _course_table, vector<pair<int, int>>& com_courses);
    bool isMustInclude(vector<pair<int, int>>& arr);
};

// =====================================================
//                  initial ���� �Լ�
// =====================================================
void TimeTable2::generator(CourseTable& _course_table, bool _print_time_table = true) {
    // INPUT
    input_data();

    // init CNT
    compare_cnt = 0;
    clock_t start = clock();
    // brute-force
    brute_force(_course_table);
    // calculate CLOCK
    clock_t finish = clock();
    clock_time = (double)(finish - start);


    // Pring OUTPUT
    cout << "\n���� �ð�: " << clock_time << "ms" << endl;
    cout << "�� Ƚ��: " << compare_cnt << "ȸ" << endl;
    cout << "�޸� ��뷮: " << (double)(memory_cnt1 * sizeof(pair<int, int>) + memory_cnt2 * sizeof(int) + memory_cnt3 * sizeof(char) + memory_cnt4 * sizeof(bool)) / (double)125000 << "mb" << endl;
    cout << "������ �ð�ǥ: " << result.size() << "��" << endl;

    vector<string> final_courses;
    string tmp_str1, tmp_str2;
    if (_print_time_table) {
        for (int i = 0; i < result.size(); i++) {
            tmp_str1 = string("[") + "�ð�ǥ" + to_string(table_num++) + "],";
            tmp_str2 = ",";

            for (int j = 0; j < result[i].size(); j++) {
                tmp_str1 += string("{") + _course_table.Get_Course(result[i][j].first).Get_Course_Name() + "-" + _course_table.Get_Course(result[i][j].first).Get_Course_ID().division + "},";
                tmp_str2 += string("{") + _course_table.Get_Course(result[i][j].first).Get_Course_ID().basic_id + "-" + _course_table.Get_Course(result[i][j].first).Get_Course_ID().division + " / "
                    + _course_table.Get_Course(result[i][j].first).Get_Faculty_In_Charge() + "},";
            }
            final_courses.push_back(tmp_str1);
            final_courses.push_back(tmp_str2);

            if (table_num == output_num + 1) break;
        }

        if (final_courses.empty())final_courses.push_back("�ش� Input���� ������ �� �ִ� �ð�ǥ�� �����ϴ�!");
        cout << "\n�ð�ǥ�� \'./Documents/TimeTable/BruteForce.csv\'�� ����Ǿ����ϴ�" << '\n';
        CsvManager::Write_Csv("./Documents/TimeTable/BruteForce.csv", final_courses);
    }
}

// =====================================================
//           ����ڿ��� ������ ���� �Է¹޴� �Լ�
// =====================================================
void TimeTable2::input_data() {
    vector<string> message = { "����", "�ʼ�", "1����", "2����", "3����","4����", "5����", "������ �ð�ǥ ����" };
    int message_index = 0;

    string stringBuffer;
    string str;
    int essencial_cnt = 0;
    int cnt = 0;

    cout << message[message_index++] << ": ";
    cin >> credit;
    cout << message[message_index++] << ": ";
    cin >> str;
    istringstream ss0(str);
    while (str != "-" && getline(ss0, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 0));
        input_num++;
        memory_cnt1++;
    }
    essential_num = input_num;
    cout << message[message_index++] << ": ";
    cin >> str;
    istringstream ss1(str);
    while (getline(ss1, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 1));
        input_num++;
        memory_cnt1++;
    }
    cout << message[message_index++] << ": ";
    cin >> str;
    istringstream ss2(str);
    while (getline(ss2, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 2));
        input_num++;
        memory_cnt1++;
    }
    cout << message[message_index++] << ": ";
    cin >> str;
    istringstream ss3(str);
    while (getline(ss3, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 3));
    }
    cout << message[message_index++] << ": ";
    cin >> str;
    istringstream ss4(str);
    while (getline(ss4, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 4));
        input_num++;
        memory_cnt1++;
    }
    cout << message[message_index++] << ": ";
    cin >> str;
    istringstream ss5(str);
    while (getline(ss5, stringBuffer, ',')) {
        if (stringBuffer == "-") break;
        input_courses.push_back(make_pair(stoi(stringBuffer), 5));
        input_num++;
        memory_cnt1++;
    }
    cout << message[message_index++] << ": ";
    cin >> output_num;
}

// =====================================================
//       course table�� conflict�ϴ��� üũ�ϴ� �Լ�
// =====================================================
bool TimeTable2::is_Table_Conflict(CourseTable& _course_table, vector<pair<int, int>>& com_courses) {
    for (int i = 0; i < com_courses.size(); i++) {
        compare_cnt++;
        auto c1 = _course_table.Get_Course(com_courses[i].first).Get_Course_ID();
        for (int j = i + 1; j < com_courses.size(); j++) {
            compare_cnt++;
            auto c2 = _course_table.Get_Course(com_courses[j].first).Get_Course_ID();
            if (_course_table.Is_Conflict(c1, c2)) {
                return true;
            }
        }
    }
    return false;
}

bool TimeTable2::isMustInclude(vector<pair<int, int>>& arr) {
    int must_cnt = 0;
    memory_cnt2++;

    for (int i = 0; i < arr.size(); i++) {
        compare_cnt++;
        if (arr[i].second == 0) must_cnt++;
    }
    if (must_cnt == essential_num) return true;
    else return false;
}

// =====================================================
//              Brute Force �����ϴ� �Լ�
// =====================================================
void TimeTable2::brute_force(CourseTable& _course_table) {
    int now_credit = 0;
    int essential_credit = 0;
    memory_cnt2 += 2;
    /*for (int i=0; i< input_courses.size(); i++)
        cout <<  input_courses[i].first << " ";
    cout << endl;*/

    // ������ ��� ������ �����ϴ� �Լ�
    vector<pair<int, int>> cmb;
    Combination(cmb, credit / 3, 0, 0);

    // �ð�ǥ 
    bool check = false;
    memory_cnt4++;
    for (int i = 0; i < combi.size(); i++) {
        if (is_Table_Conflict(_course_table, combi[i]) == false && isMustInclude(combi[i]) == true) {
            result.push_back(combi[i]);
            memory_cnt1++;
            if (++make_cnt == output_num) break;
        }
    }
}

/**
// =====================================================
//  Combination ���� ������� ��� ����� ���� ����ϴ� �Լ�
// =====================================================
 arr = ��� �迭           comb = ��� �迭
 r = ���� �̾ƾ� �� ����    index = comb �� �ε���
 depth = ��� �迭���� ���� ���Ҹ� �����ϴ� �ε���
 */
void TimeTable2::Combination(vector<pair<int, int>>& cmb, int r, int index, int depth) { // depth == ��ȭ���� n����
    compare_cnt++;
    if (r == 0) { // �̾ƾ��� ������ ��� �� ��쿡�� comb �� ����ִ� �� ��� ���
        combi.push_back(cmb);
        memory_cnt1++;
        cmb.pop_back();
    }
    else if (depth == input_courses.size()) {   // ���� ���� �ε����� ��� �迭�� ���������� �� ��� return
        return;
    }
    else {
        // arr[depth] �� ���� ���
        if (cmb.size() <= index) {
            cmb.push_back(make_pair(input_courses[depth].first, input_courses[depth].second));
            memory_cnt1++;
        }
        else {
            cmb[index].first = input_courses[depth].first;
            cmb[index].second = input_courses[depth].second;
        }
        Combination(cmb, r - 1, index + 1, depth + 1); // n-1Cr-1: Ư�� ���Ҹ� ���� ���

        // arr[depth] �� ���� ���� ���
        Combination(cmb, r, index, depth + 1); // n-1Cr: Ư�� ���Ҹ� ���� ���� ���
    }
}