#include "user_service.h"
#include "user.h"
#include "util.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
// 로그인, 회원가입, 로그아웃 기능 함수
// ID, PW 문법 규칙 검사 함수

using namespace std;

// ID 문법 검사
bool isValidId(const string& id) {
    regex idRegex("^[a-z0-9]{3,20}$");
    return regex_match(id, idRegex);
}

// PW 문법 검사
bool isValidPassword(const string& pw) {
    if (pw.length() < 4 || pw.length() > 20) return false;
    if (pw.find(' ') != string::npos) return false;

    bool hasAlpha = false, hasDigit = false;
    for (char c : pw) {
        if (isalpha(c)) hasAlpha = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasAlpha && hasDigit;
}

// 로그인
User userLogin() {
    string id, pw;

    while (true) {
        cout << "ID: ";
        getline(cin, id);
        if (id == "back") return User();

        cout << "PW: ";
        getline(cin, pw);
        if (pw == "back") return User();

        if (!isValidId(id) || !isValidPassword(pw)) {
            cout << ".!! ID or PW format is incorrect." << endl;
            continue;
        }

        ifstream inFile("user.txt");
        if (!inFile) {
            cout << ".!! Cannot open user file." << endl;
            return User();
        }

        string fileId, filePw;
        int adminFlag;
        while (inFile >> fileId >> filePw >> adminFlag) {
            if (fileId == id && filePw == pw) {
                inFile.close();
                return User(id, pw, adminFlag == 1);
            }
        }
        inFile.close();

        cout << ".!! ID or PW is incorrect." << endl;
    }
}

// 회원가입
void userSignup() {
    string id, pw;

    while (true) {
        cout << "ID: ";
        getline(cin, id);
        if (id == "back") return;

        if (!isValidId(id)) {
            cout << ".!! Incorrect form: use only lowercase English and number." << endl;
            continue;
        }

        ifstream checkFile("user.txt");
        if (!checkFile) {
            cout << ".!! Cannot open user file." << endl;
            return;
        }

        string fileId, filePw;
        int adminFlag;
        bool isDuplicate = false;

        while (checkFile >> fileId >> filePw >> adminFlag) {
            if (fileId == id) {
                isDuplicate = true;
                break;
            }
        }
        checkFile.close();

        if (isDuplicate) {
            cout << ".!! This ID already exists. Try a different one." << endl;
            continue;
        }

        break;
    }

    while (true) {
        cout << "PW: ";
        getline(cin, pw);
        if (pw == "back") return;

        if (!isValidPassword(pw)) {
            cout << ".!! Incorrect form: use English and number. You can use special characters, but can't use space character." << endl;
            continue;
        }

        break;
    }

    ofstream outFile("user.txt", ios::app);
    if (!outFile) {
        cout << ".!! Cannot open user file to write." << endl;
        return;
    }

    outFile << id << "\t" << pw << "\t0" << endl;
    outFile.close();

    cout << ".>> Signup completed successfully." << endl;
}

bool Logout() {
    string confirm;
    while (true) {
        cout << "Would you like to log out? ";
        getline(cin, confirm);
        confirm = trim(confirm);

        if (confirm == "Y") return true;
        else if (confirm == "N") return false;
        else cout << ".!! You can only enter upper case 'Y' or 'N'." << endl;
    }
}

