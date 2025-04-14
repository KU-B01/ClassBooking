#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> // input parsing용
#include <iomanip>
#include <conio.h>  // kbhit, getch --> windows용, mac에서는 termios 사용
using namespace std;

// 강의실 구조체
struct Classroom {
    string room;
    bool is_available;
    string available_start;
    string available_end;
};

// 사용자 구조체
struct User {
    string id;
    string password;
    bool is_admin;
};

// 예약 구조체
struct Reservation {
    string user_id;
    string room;
    string day;
    string start_time;
    string end_time;
};

vector<Classroom> classrooms;     // 강의실 정보 저장
vector<User> users;                 // 사용자 정보 저장
vector<Reservation> reservations;   // 예약 정보
string classroomNum;                  // 입력받는 강의실 번호
bool triggered = false;     // main 아무 키 입력 무한루프문 제어

// 요일 리스트
vector<string> weekdays = { "Mon", "Tue", "Wed", "Thu", "Fri" };
// 시간대 리스트임, 1시간 단위
vector<string> times = {
    "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00"
};

////////////////////////////////////////////////////////////////////////////////////////////
//사용자 프롬프트 불러오기

// 강의실 불러오는 함수
bool loadClassrooms() {
    ifstream fin("classroom.txt");
    if (!fin) return false;
    string room, start, end;
    int avail;
    while (fin >> room >> avail >> start >> end) {
        bool available_flag = (avail != 0);  // int을 bool로 바꿈
        classrooms.push_back({ room, available_flag, start, end });
    }
    return true;
}

// 강의실 리스트 출력
void printClassroomList() {
    cout << "3F: "; for (auto& c : classrooms) if (c.room[0] == '3') cout << c.room << ", "; cout << endl;
    cout << "4F: "; for (auto& c : classrooms) if (c.room[0] == '4') cout << c.room << ", "; cout << endl;
    cout << "5F: "; for (auto& c : classrooms) if (c.room[0] == '5') cout << c.room << ", "; cout << endl;
    cout << "6F: "; for (auto& c : classrooms) if (c.room[0] == '6') cout << c.room << ", "; cout << endl;
}

// 강의실 번호를 입력받는 함수
string getClassroomNum() {
    string input;
    cout << "classroom number: ";
    getline(cin, input);  //전체 문자열을 받음

    string parsedInput = "";
    for (char ch : input) {
        if (ch != ' ' && ch != '\t' && ch != '\n') {
            parsedInput += ch;
        }
    }
    stringstream ss(parsedInput);
    ss >> classroomNum;

    return classroomNum;
}

// 강의실 번호 유효성 확인
bool checkClassroomNum(string classroomNum) {
    for (auto& c : classrooms) {
        if (classroomNum == c.room) {   //문자열 비교
            return true;
        }
    }
    // classroom.txt에 없는 강의실 번호일 경우
    cout << "The classroom you entered doesn't exist." << endl;
    return false;
}

// 시간 겹치는지 확인하는 함수
bool isTimeOverlap(const string& s1, const string& e1, const string& s2, const string& e2) {
    return !(e1 <= s2 || s1 >= e2);
}

// 예약 정보 출력 함수
void printTimeTable(string room) {
    cout << "\n             ";
    for (auto& day : weekdays) cout << setw(6) << day;
    cout << endl;
    for (int t = 0; t < 9; ++t) {
        string t1 = times[t];
        string t2 = times[t + 1];
        cout << t1 << "~" << t2 << " ";
        for (int d = 1; d <= 5; ++d) {
            bool reserved = false;
            for (auto& r : reservations) {
                if (r.room == room && stoi(r.day) == d && isTimeOverlap(r.start_time, r.end_time, t1, t2)) {
                    reserved = true;
                    break;
                }
            }
            cout << setw(6) << (reserved ? "X" : "O");
        }
        cout << endl;
    }
}

int main() {
    string anyInput = "";

    if (!loadClassrooms()) {
        cout << "Failed to load classrooms from file!" << endl;
        return 1;
    }

    printClassroomList();

    // 유효한 강의실 번호가 입력될 때까지 반복
    while (true) {
        string input = getClassroomNum();
        if (checkClassroomNum(input)) {
            printTimeTable(classroomNum);
            break;
        }
    }

    cout << "Press any key to continue..." << endl;

    // 아무 키 입력 대기
    while (true) {
        if (!triggered && _kbhit()) {
            _getch();     // 입력받음

            // 사용자 프롬프트로 돌아감
            // userPrompt();

            break;  // 대기 종료
        }
    }

    cout << "finished." << endl;
    return 0;
}
