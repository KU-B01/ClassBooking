#include <iostream>
#include <string>
#include "prompt.h"
#include "user.h"
#include "user_service.h"
#include "util.h"
// 시작 프롬프트
// 관리자 프롬프트, 사용자 프롬프트 함수

using namespace std;

void showMainMenu() {
    cout << "\n--------Classroom Booking Program--------" << endl;
    cout << "1. login" << endl;
    cout << "2. accession" << endl;
    cout << "3. exit" << endl;
    cout << ">> ";
}

void userPrompt(const User& user) {
    while (true) {
        cout << "\n[User: " << user.getId() << "]" << endl;
        cout << "--------Main--------" << endl;
        cout << "1. classroom list" << endl;
        cout << "2. reserve classroom" << endl;
        cout << "3. cancel reservation" << endl;
        cout << "4. logout" << endl;
        cout << ">> ";

        string input;
        getline(cin, input);
        input = trim(input);

        if (input == "1") {
            // 호출
        } else if (input == "2") {
            // 호출
        } else if (input == "3") {
            // 호출
        } else if (input == "4") {
            if (Logout()) return;
        } else {
            cout << ".!! Enter the index number in the menu." << endl;
        }
    }
}

void adminPrompt(const User& user) {
    while (true) {
        cout << "\n[Admin: " << user.getId() << "]" << endl;
        cout << "--------Main for manager--------" << endl;
        cout << "1. reservation list and change" << endl;
        cout << "2. classroom situation and change" << endl;
        cout << "3. logout" << endl;
        cout << ">> ";

        string input;
        getline(cin, input);
        input = trim(input);

        if (input == "1") {
            // 호출
        } else if (input == "2") {
            // 호출
        } else if (input == "3") {
            if (Logout()) return;
        } else {
            cout << ".!! Enter the index number in the menu." << endl;
        }
    }
}