#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
#include "user_service.h"
#include "user.h"

using namespace std;

// 공백 제거 //
string trim(const string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");

    if (start == string::npos || end == string::npos) return "";
    return str.substr(start, end - start + 1);
}

void showMainMenu() {
    cout << "\n--------Classroom Booking Program--------" << endl;
    cout << "1. login" << endl;
    cout << "2. accession" << endl;
    cout << "3. exit" << endl;
    cout << ">> ";
}

void userPrompt(const User& user) {
    while (true) {
        cout << "\n[User Mode] ID: " << user.getId() << endl;
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
            // 강의실 목록 출력
        } else if (input == "2") {
            // 강의실 예약
        } else if (input == "3") {
            // 예약 취소
        } else if (input == "4") {
            
            while (true) {
                cout << "Would you like to log out? ";
                string confirm;
                getline(cin, confirm);
                confirm = trim(confirm);

                if (confirm == "Y") {
                    cout << ">> Logged out. Goodbye, " << user.getId() << "!" << endl;
                    return;
                } else if (confirm == "N") {
                    break;
                } else {
                    cout << ".!! You can only enter upper case Y or N." << endl;
                }
            }
        } else {
            cout << ".!! Enter the index number in the menu." << endl;
        }
    }
}

void adminPrompt(const User& user) {
    while (true) {
        cout << "\n[Admin Mode] ID: " << user.getId() << endl;
        cout << "--------Main for manager--------" << endl;
        cout << "1. reservation list and change" << endl;
        cout << "2. classroom situation and change" << endl;
        cout << "3. logout" << endl;
        cout << ">> ";

        string input;
        getline(cin, input);
        input = trim(input);

        if (input == "1") {
            // 예약 목록 및 변경
        } else if (input == "2") {
            // 강의실 현황 조회 및 변경
        } else if (input == "3") {
            
            while (true) {
                cout << "Would you like to log out? (Y/N) : ";
                string confirm;
                getline(cin, confirm);
                confirm = trim(confirm);

                if (confirm == "Y") {
                    cout << ">> Logged out. Goodbye, " << user.getId() << "!" << endl;
                    return;
                } else if (confirm == "N") {
                    break;
                } else {
                    cout << ".!! You can only enter upper case Y or N." << endl;
                }
            }
        } else {
            cout << ".!! Enter the index number in the menu." << endl;
        }
    }
}

int main() {
    while (true) {
        showMainMenu();
        string input;
        getline(cin, input);
        input = trim(input);

        if (input == "1") {
            User user = userLogin();
            if (user.getId() == "") {
                continue;
            }
            if (user.getIsAdmin()) {
                adminPrompt(user);
            } else {
                userPrompt(user);
            }
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
        }
        else {
            cout << ".!! Enter the index number in the menu." << endl;
        }
    }
    return 0;
}
