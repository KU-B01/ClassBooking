#include <iostream>// 입출력 기능 사용
#include <fstream>// 파일 입출력 사용
#include <sstream> // 문자열 스트림 처리용
#include <string>  // 문자열 타입 사용
#include <vector> // 동적 배열 벡터 사용
#include <iomanip>// 출력 정렬용 (setw 쓸 때 필요)
using namespace std;

//JW - class user은 user.cpp에 user도 vertor로 받지않음
// user, int main을 제외하곤 중요한 것은 크게 건들지 않음
// trim을 이용해서 str을 받아 선택자를 처리
#include <algorithm> // for trim
#include "user.h"
#include "user_service.h"
#include "prompt.h"
#include "util.h"

// 예약 구조체
struct Reservation {
    string user_id;
    string room;
    string day;
    string start_time;
    string end_time;
};

// 강의실 구조체
struct Classroom {
    string room;
    bool is_available;
    string available_start;
    string available_end;
};

vector<Reservation> reservations;
vector<Classroom> classrooms;

void printClassroomList();
void printTimeTable(const string& room);
void reserveClassroom(const string& userId);
void cancelReservation(const string& userId);
void showAndEditClassroom();

int main() {
    while (true) {
        showMainMenu();
        string input;
        getline(cin, input);
        input = trim(input);

        if (input == "1") {
            User user = userLogin();
            if (user.getId().empty()) continue;
            if (user.getIsAdmin()) adminPrompt(user);
            else userPrompt(user);
        }
        else if (input == "2") {
            userSignup();
        }
        else if (input == "3") {
            cout << "If you want to quit this program, enter 'quit'." << endl;
            string confirm;
            getline(cin, confirm);
            confirm = trim(confirm);
            if (confirm == "quit") {
                cout << "Program terminated." << endl;
                break;
            } else {
                cout << ".!! Enter the index number in the menu." << endl;
            }
        } else {
            cout << ".!! Enter the index number in the menu." << endl;
        }
    }
    return 0;
}
