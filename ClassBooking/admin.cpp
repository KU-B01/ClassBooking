// admin.cpp
#include "admin.hpp"
#include "models.hpp"
#include "util.hpp"
#include "classroom.hpp"
#include "reservation.hpp"
#include "user.hpp"
#include "time.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <algorithm>

using namespace std;

//2차 구현: 관리자 프롬프트 admin.cpp로 이동
bool managerPrompt(const string& admin_id)
{
	cout << "-- Main for manager --\n";
	cout << "1. reservation list and change\n";
	cout << "2. classroom situation and change\n";
	// 2차 구현
	cout << "3. accept/ban user\n";
	// 2차 구현
	cout << "4. add/delete classroom\n";
	cout << "5. logout\n>> ";
	int admin_choice = getValidatedMenuIndex("menu", 1, 5);

    if (admin_choice < 1 && admin_choice > 5) return true;
	if (admin_choice == 1)
		reservationManagementMenu(); // 예약 목록 관리
	else if (admin_choice == 2)
		classroomManagementMenu(admin_id); // 강의실 관리
    // 2차 구현 
	else if (admin_choice == 3)
		UserBanManagementMenu();    // 사용자 제명 & 해제
    // 2차 구현 
	else if (admin_choice == 4)
		ClassroomEditManagementMenu();  // 강의실 목록 관리
	else if (admin_choice == 5 && logout())     return false;

	return true;
}

// 관리자: 예약 목록 관리 메뉴
void reservationManagementMenu()
{
    while (true) {
        cout << "1. register reservation\n";
        cout << "2. check reservation list\n";
        cout << "3. delete reservation\n";
        // 2차 구현 return
        cout << "4. return\n>> ";

        int idx = getValidatedMenuIndex("menu", 1, 4);
        if (idx < 1 && idx >4) continue;

        if (idx == 1) {
            reserveClassroom(InputUser());
        }
        else if (idx == 2)
            reservation_check_admin1();
        else if (idx == 3)
            cancelReservation(InputUser());

        // 2차 구현 return
        else if (idx == 4)  break;
    }
    return;
}

// 관리자: 강의실 예약 허용/금지 메뉴
void classroomManagementMenu(const string &admin_id)
{
    while (true) {
        cout << "1. check reservation table\n";
        cout << "2. accept reservation\n";
        cout << "3. ban reservation\n";
        // 2차 구현 return
        cout << "4. return\n>>";

        int idx = getValidatedMenuIndex("menu", 1, 4);
        if (idx < 1 && idx > 4)   continue;

        if (idx == 1)
            viewClassroomTimetable();
        else if (idx == 2 || idx == 3)
            handleClassroomAccess(admin_id, idx == 2);
        // 2차 구현 return
        else if (idx == 4)  break;
    }
        return;
}

// 모든 예약 내역 출력 (관리자용)
void reservation_check_admin1()
{
    ifstream reservation_file("reservation.txt");
    map<string, string> day_map = {
        {"1", "Mon"},
        {"2", "Tue"},
        {"3", "Wed"},
        {"4", "Thu"},
        {"5", "Fri"},
    };
    string line;
    bool found = false;
    while (getline(reservation_file, line))
    {
        stringstream ss(line);
        string user_id, class_num_str, start_time, end_time, day_str;

        getline(ss, user_id, '\t');
        getline(ss, class_num_str, '\t');
        getline(ss, start_time, '\t');
        getline(ss, end_time, '\t');
        getline(ss, day_str, '\t');

        cout << user_id << " "
             << class_num_str << " "
             << day_map[day_str] << " "
             << start_time << " " << end_time << " " << endl;

        found = true; // 예약 정보를 찾았으므로 true로 설정
    }

    reservation_file.close();

    if (!found)
    {
        cout << "No reservation history found." << endl;
    }

    cout << "\npress any key to continue..." << endl;
    cin.get();
}

// 사용자/관리자 공통 예약 취소
void cancelReservationMenu(const string &current_user_id)
{
    string user_id = trimWhitespace(current_user_id); // 공백 제거
    bool is_admin = false;

    // 관리자 여부 확인
    for (const auto &u : users)
    {
        if (u.id == user_id && u.is_admin)
        {
            is_admin = true;
            break;
        }
    }

    // 관리자라면 ID 재입력 요구
    if (is_admin)
    {
        while (true)
        {
            cout << "Enter ID to cancel reservation: ";
            getline(cin, user_id);
            user_id = trimWhitespace(user_id);

            if (user_id == "admin01")
            {
                cout << ".!! Admin ID cannot cancel reservations.\n";
                return;
            }
            if (!isValidID(user_id) || !isExistUser(user_id))
            {
                cout << ".!! Invalid or non-existent ID.\n";
                continue;
            }
            break;
        }
    }

    bool success = false;
    while (!success)
        success = cancelReservation(user_id);
}

// 관리자: 강의실 예약 허용 또는 금지 설정
void handleClassroomAccess(const string &admin_id, bool is_accept_mode)
{
    string room = InputClassroom();

    string day;
    while (true)
    {
        cout << "enter a number corresponding to the day of the week\n"
             << "(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";
        getline(cin, day);
        if (checkIdx(day))
        {
			printIdxErrorMessage("day");
			continue;
        }
        if (stoi(day) < 1 || stoi(day) > 5)
        {
            printIdxErrorMessage("day");
            continue;
        }
        break;
    }

    string start, end;
    while (true)
    {
        cout << (is_accept_mode ? "Start accept time: " : "Start ban time: ");
        getline(cin, start);
        start = trimWhitespace(start);
        cout << (is_accept_mode ? "End accept time: " : "End ban time: ");
        getline(cin, end);
        end = trimWhitespace(end);

        if (start.length() != 5 || end.length() != 5)
        {
            cout << ".!! Incorrect form: must be exactly HH:00 format (e.g., 09:00, 14:00).\n";
            continue;
        }

        if (!isValidTime(start) || !isValidTime(end))
        {
            cout << ".!! Incorrect form: an example of correct input is '14:00'. Please enter again.\n";
            continue;
        }
        if (timeToMinutes(start) >= timeToMinutes(end))
        {
            cout << ".!! Start time must be earlier than end time.\n";
            continue;
        }

        int sh = stoi(start.substr(0, 2));
        int eh = stoi(end.substr(0, 2));
        if (sh < 9 || eh > 18)
        {
            cout << ".!! Reservation time must be between 09:00 and 18:00.\n";
            continue;
        }
        break;
    }

    // 1차 구현 중 빠진 부분 사용자 예약이 존재하면 강의실 금지 불가
    if (!is_accept_mode)
    {
        for (const auto &r : reservations)
        {
            if (r.user_id != admin_id && r.room == room && r.day == day &&
                isTimeOverlap(r.start_time, r.end_time, start, end))
            {
                cout << ".!! This time is already reserved by a user. Cannot ban.\n";
                return;
            }
        }
    }

    // 허용 시간 설정이면 기존 금지 시간에서 해당 시간 제거
    if (is_accept_mode)
    {
        vector<Reservation> updated;
        for (auto it = reservations.begin(); it != reservations.end();)
        {
            if (it->user_id == admin_id && it->room == room && it->day == day &&
                isTimeOverlap(it->start_time, it->end_time, start, end))
            {

                if (it->start_time < start)
                    updated.push_back({admin_id, room, day, it->start_time, start});
                if (it->end_time > end)
                    updated.push_back({admin_id, room, day, end, it->end_time});

                it = reservations.erase(it);
            }
            else
            {
                ++it;
            }
        }
        reservations.insert(reservations.end(), updated.begin(), updated.end());
        cout << "Accept time updated.\n";
    }
    // 금지 시간 추가
    else
    {
        bool duplicate = false;
        for (const auto &r : reservations)
        {
            if (r.user_id == admin_id && r.room == room && r.day == day &&
                r.start_time == start && r.end_time == end)
            {
                duplicate = true;
                break;
            }
        }
        if (!duplicate)
        {
            reservations.push_back({admin_id, room, day, start, end});
            cout << "Ban completed.\n";
        }
        else
        {
            cout << ".!! This ban reservation already exists.\n"; // 경고 띄우기
            return;
        }
    }

    vector<Reservation> adminBans;
    for (const auto &r : reservations)
    {
        if (r.user_id == admin_id)
            adminBans.push_back(r);
    }

    vector<Reservation> mergedBans;
    for (const auto &res : adminBans)
    {
        if (mergedBans.empty() || mergedBans.back().room != res.room || mergedBans.back().day != res.day || mergedBans.back().end_time < res.start_time)
        {
            mergedBans.push_back(res);
        }
        else
        {
            mergedBans.back().end_time = max(mergedBans.back().end_time, res.end_time);
        }
    }

    vector<Reservation> newReservations;
    for (const auto &r : reservations)
    {
        if (r.user_id != admin_id)
            newReservations.push_back(r);
    }
    for (const auto &r : mergedBans)
        newReservations.push_back(r);

    reservations = newReservations;
    // reservation.txt 저장
    ofstream fout("reservation.txt");
    for (const auto &r : reservations)
    {
        fout << r.user_id << "\t" << r.room << "\t"
             << r.start_time << "\t" << r.end_time << "\t" << r.day << endl;
    }
    fout.close();
}



// 2차 구현 사용자 밴 관리자 프롬프트 메뉴
void UserBanManagementMenu() {
    while (true) {
        cout << "1. accept user\n";
        cout << "2. ban user\n";
        cout << "3. return\n>> ";

        int idx = getValidatedMenuIndex("menu", 1, 3);
        if (idx < 1 && idx>3) continue;

        if (idx == 1) acceptUser();
        else if (idx == 2) banUser();
        else if (idx == 3)  break;
    }
    return;
}

// 2차 구현 사용자 밴 해제
void acceptUser() {
    while (true) {
        cout << "\nenter user ID to accept\nuser ID: ";
        string input;
        getline(cin, input);
        input = trimWhitespace(input);

        if (!isValidID(input)) {
            cout << ".!! Check validity of user ID.\n";
            continue;
        }

        bool found = false;
        for (auto& u : users) {
            if (!u.is_admin && u.id == input) {
                found = true;
                if (u.is_active) {
                    cout << ".!! Selected user's permission is already accepted.\n";
                    continue;
                }
                u.is_active = true;
                cout << "User " << u.id << " successfully accepted.\n\n";

                ofstream fout("user.txt");
                for (const auto& usr : users) {
                    if (!usr.is_admin) {
                        fout << usr.id << "\t" << usr.password << "\t0\t" << (usr.is_active ? 1 : 0) << "\n";
                    }
                }
                fout.close();
                return;
            }
        }

        if (!found) {
            cout << ".!! User ID doesn't exist in user list.\n";
        }
    }
}

// 2차 구현 사용자 밴
void banUser() {
    while (true) {
        cout << "\nenter user ID to ban\nuser ID: ";
        string input;
        getline(cin, input);
        input = trimWhitespace(input);

        if (!isValidID(input)) {
            cout << ".!! Check validity of user ID.\n";
            continue;
        }

        bool found = false;
        for (auto& u : users) {
            if (!u.is_admin && u.id == input) {
                found = true;
                if (!u.is_active) {
                    cout << ".!! Selected user's permission is already banned.\n";
                    continue;
                }
                u.is_active = false;
                cout << "User " << u.id << " successfully banned.\n\n";

                ofstream fout("user.txt");
                for (const auto& usr : users) {
                    if (!usr.is_admin) {
                        fout << usr.id << "\t" << usr.password << "\t0\t" << (usr.is_active ? 1 : 0) << "\n";
                    }
                }
                fout.close();
                return;
            }
        }

        if (!found) {
            cout << ".!! User ID doesn't exist in user list.\n";
        }
    }
}

// 2차 구현 사용자 밴 여부 확인
bool isUserBanned(const string& id) {
    for (const auto& u : users) {
        if (u.id == id && !u.is_admin) {
            return !u.is_active;
        }
    }
    return false;
}

// 2차 구현 강의실 추가/삭제
void ClassroomEditManagementMenu() {
    while (true) {
        cout << "\n-- Classroom Add/Delete Menu --\n";
        cout << "1. add classroom\n";
        cout << "2. delete classroom\n";
        cout << "3. return\n>> ";

        int idx = getValidatedMenuIndex("menu", 1, 3);
        if (idx < 1 && idx >3)    continue;

        if (idx == 1) {
            addClassroom();
        }
        else if (idx == 2) {
            deleteClassroom();
        }
        else if (idx == 3)  break;
    }
    return;
}

// 2차 구현 강의실 추가
void addClassroom() {
    while (true) {
        cout << "classroom number: ";
        string room;
        getline(cin, room);
        room = trimWhitespace(room);

        try {
            int roomNum = stoi(room);
            if (roomNum < 101 || roomNum > 999) {
                cout << "Classroom numbers must be 101-999.\n";
                continue;
            }
        } catch (...) {
            cout << "Classroom numbers must be 101-999.\n";
            continue;
        }

        if (isExistRoomNumber(room)) {
            cout << "The classroom you entered already exists.\n";
            continue;
        }

        ofstream fout("classroom.txt", ios::app);
        if (!fout) {
            cerr << "[Error] Failed to open classroom.txt for writing.\n";
            return;
        }
        fout << room << " 1 09:00 18:00\n";
        fout.close();

        classrooms.push_back({room, true, "09:00", "18:00"});

        cout << "The classroom successfully added.\n";
        return;
    }
};

// 2차 구현 강의실 삭제
void deleteClassroom() {
    while (true) {
        cout << "classroom number: ";
        string room;
        getline(cin, room);
        room = trimWhitespace(room);

        try {
            int roomNum = stoi(room);
            if (roomNum < 101 || roomNum > 999) {
                cout << "Classroom numbers must be 101-999.\n";
                continue;
            }
        } catch (...) {
            cout << "Classroom numbers must be 101-999.\n";
            continue;
        }

        if (!isExistRoomNumber(room)) {
            cout << "The classroom you entered doesn’t exist.\n";
            continue;
        }

        // 예약 존재 여부 검사 로직 만약 미래 사용자의 예약이 있다면 강의실 삭제 불가가
        bool hasFutureReservation = false;
        for (const auto& r : reservations) {
            if (r.room == room && !isBeforeVirtualTime(r.day, r.start_time)) {
                hasFutureReservation = true;
                break;
            }
        }

        if (hasFutureReservation) {
            cout << ".!! Future reservation exists for this classroom. Cannot delete.\n";
            return;
        }

        ifstream fin("classroom.txt");
        ofstream fout("classroom_tmp.txt");

        string line;
        while (getline(fin, line)) {
            if (line.find(room + " ") != 0) {
                fout << line << "\n";
            }
        }
        fin.close();
        fout.close();

        remove("classroom.txt");
        rename("classroom_tmp.txt", "classroom.txt");

        classrooms.erase(
            remove_if(classrooms.begin(), classrooms.end(),
                      [&](const Classroom& c) { return c.room == room; }),
            classrooms.end()
        );

        cout << "The classroom successfully deleted.\n";
        return;
    };
};
