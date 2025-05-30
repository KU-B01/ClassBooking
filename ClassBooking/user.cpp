#include "user.hpp"
#include "models.hpp"
#include "util.hpp"
#include "time.hpp"

#include <iostream>
#include <fstream>
#include <regex>
#include <map>
#include "classroom.hpp"
#include "reservation.hpp"

using namespace std;

// 사용자 ID 입력 (존재하는 사용자만 입력 허용)
string InputUser() {
    string input;
    while (true) {
        cout << "ID: ";
        getline(cin, input);

        if (!isExistUser(input)) {
            cout << ".!! ID doesn't exist.\n";
        } else {
            return input;
        }
    }
}

// user.txt에서 사용자 목록을 불러옴
bool loadUsers()
{
    std::ifstream fin("user.txt");

    if (!fin)
    {
        std::cerr << "[Warning] user.txt file not found. Creating empty user.txt...\n";
        std::ofstream fout("user.txt");
        if (!fout)
        {
            std::cerr << "[Error] Failed to create user.txt\n";
            exit(1);
        }
        fout.close();
    }

    std::string id, pw;
    // 2차 구현에서 active 추가
    int admin, active;
    std::map<std::string, bool> id_check;

    while (fin >> id >> pw >> admin >> active)
    {
        if (!isValidID(id))
        {
            std::cerr << "[Error] Invalid ID format detected in user.txt -> " << id << "\n";
            exit(1);
        }

        if (!isValidPassword(pw))
        {
            std::cerr << "[Error] Invalid Password format detected in user.txt -> " << pw << "\n";
            exit(1);
        }

        if (id_check[id])
        {
            std::cerr << "[Error] Duplicate ID detected in user.txt -> " << id << "\n";
            exit(1);
        }
        id_check[id] = true;

        bool isAdmin = (admin != 0);
        // 2차 구현 isActive
        bool isActive = (active != 0);
        users.push_back({id, pw, isAdmin, isActive});
    }

    fin.close();

    // 고정 관리자 계정
    users.push_back({"admin1", "admin123", true, true});

    return true;
}

// 로그인 함수
User* login() {
    string id, pw;
    cout << "ID: ";
    getline(cin, id);
    cout << "PW: ";
    getline(cin, pw);

    for (auto& u : users) {
        if (u.id == id && u.password == pw) {
            return &u;
        }
    }
    cout << ".!! ID or PW is incorrect.\n";
    return nullptr;
}
// 로그아웃
bool logout()
{
    string input;
    cout << "Would you like to log out? (Y/N) ";
    cin >> input;

    if (input == "Y")
    {
        return true;
    }
    else if (input == "N")
    {
        return false;
    }
    cout << ".!! You can only enter upper case 'Y' or 'N'." << endl;
    return logout();
}


// 회원가입 기능
void registerUser() {
    string id, pw;
    bool valid = false;

    while (!valid) {
        cout << "ID: ";
        getline(cin, id);

        if (!isValidID(id)) {
            cout << ".!! Incorrect ID format.\n";
            continue;
        }

        if (isExistUser(id)) {
            cout << ".!! ID already exists.\n";
            continue;
        }

        cout << "PW: ";
        getline(cin, pw);

        if (!isValidPassword(pw)) {
            cout << ".!! Incorrect password format.\n";
            continue;
        }

        valid = true;
        // 2차 구현 isActive default로 true
        users.push_back({id, pw, false, true});
        ofstream fout("user.txt", ios::app);
        // 2차 구현 isActive true
        fout << id << "\t" << pw << "\t0\t1\n";
        fout.close();
        cout << "Registration complete.\n";
    }
}

// 2차 구현: 사용자 프롬프트 user.cpp로 이동
bool userPrompt(const string& user_id) {
	cout << "-- Main --\n";
	cout << "1. classroom list\n";
	cout << "2. reserve classroom\n";
	cout << "3. cancel reservation\n";
	cout << "4. logout\n>> ";
	int choice = getValidatedMenuIndex("menu", 1, 4);
	if (choice == 1) {
		viewClassroomTimetable(); // classroom list
	}
	else if (choice == 2) {
		reserveClassroom(user_id); // reserve classroom
	}
	else if (choice == 3) {
		cancelReservation(user_id); // cancel reservation
	}
	else if (choice == 4) {
        if (logout())    return false; // 로그아웃
	}
	return true; // 계속 진행
}
