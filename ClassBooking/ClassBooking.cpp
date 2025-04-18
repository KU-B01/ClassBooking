/*
* 2025 04 14
* �Ϲ� ����� ���� ������ ����
*/


// �⺻ �����, ���������, ���ڿ�ó��, ���� ���̺귯�� ������
#include <iostream>// ����� ��� ���
#include <fstream>// ���� ����� ���
#include <sstream> // ���ڿ� ��Ʈ�� ó����
#include <string>  // ���ڿ� Ÿ�� ���
#include <vector> // ���� �迭 ���� ���
#include <iomanip>// ��� ���Ŀ� (setw �� �� �ʿ�)
#include "util.hpp"
using namespace std;

// ����� ����ü
struct User {
    string id;
    string password;
    bool is_admin;
};

// ���� ����ü
struct Reservation {
    string user_id;
    string room;
    string day;
    string start_time;
    string end_time;
};

// ���ǽ� ����ü
struct Classroom {
    string room;
    bool is_available;
    string available_start;
    string available_end;
};

// ���ͷ� ����, ����, ���ǽ� ������, �̰� ���� ������
vector<User> users;
vector<Reservation> reservations;
vector<Classroom> classrooms;

// ���� ����Ʈ
vector<string> weekdays = { "Mon", "Tue", "Wed", "Thu", "Fri" };
// �ð��� ����Ʈ��, 1�ð� ����
vector<string> times = {
    "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00"
};

// �ð� ��ġ���� Ȯ���ϴ� �Լ�
bool isTimeOverlap(const string& s1, const string& e1, const string& s2, const string& e2) {
    return !(e1 <= s2 || s1 >= e2);
}

// ���ǽ� �ҷ����� �Լ�
bool loadClassrooms() {
    ifstream fin("classroom.txt");
    if (!fin) return false;
    string room, start, end;
    int avail;
    while (fin >> room >> avail >> start >> end) {
        bool available_flag = (avail != 0);  // int�� bool�� �ٲ�
        classrooms.push_back({ room, available_flag, start, end });
    }
    return true;
}

// ���� �ҷ����� �Լ�
bool loadUsers() {
    ifstream fin("user.txt");
    if (!fin) return false;
    string id, pw;
    int admin;
    while (fin >> id >> pw >> admin) {
        bool isAdmin = (admin != 0);
        users.push_back({ id, pw, isAdmin });
    }
    return true;
}

// ���� �ҷ����� �Լ�
bool loadReservations() {
    ifstream fin("reservation.txt");
    if (!fin) return false;
    string id, room, start, end, day;
    while (fin >> id >> room >> start >> end >> day) {
        reservations.push_back({ id, room, day, start, end });
    }
    return true;
}

// �����ϴ� ���ǽ����� Ȯ��
bool isExistRoomNumber(const string& input) {
    for (const Classroom& cls : classrooms) {
        if (cls.room == input) {
            return true;
        }
    }
    return false;
}

// �����ϴ� ��������� Ȯ�� 
bool isExistUser(const string& input) {
    for (const User& usr : users) {
        if (usr.id == input) {
            return true;
        }
    }
    return false;
}

string InputClassroom() {
    string input;

    while (true) {
        cout << "classroom number: ";
        cin >> input;

        // ������ �߸��ưų� �������� �ʴ� ���ǽ��̸� �ٽ� �Է�
        if (!validateRoomNumber(input) || !isExistRoomNumber(input)) {
            cout << ".!! The classroom you entered doesn't exist. Please try again.\n";
        }
        else {
            return input;  // ��ȿ�ϸ� ��ȯ
        }
    }
}

void InputUser() {
    string input;

    while (true) {
        cout << "ID:";
        cin >> input;

        if (!isExistUser(input)) {
            cout << ".!! ID doesn't exist.\n";
        }
        else { break; }
    }

}

// ���ǽ� ������ �������, �״�� �ᵵ �ɵ�
void printClassroomList() {
    cout << "3F: "; for (auto& c : classrooms) if (c.room[0] == '3') cout << c.room; cout << endl;
    cout << "4F: "; for (auto& c : classrooms) if (c.room[0] == '4') cout << c.room; cout << endl;
    cout << "5F: "; for (auto& c : classrooms) if (c.room[0] == '5') cout << c.room; cout << endl;
    cout << "6F: "; for (auto& c : classrooms) if (c.room[0] == '6') cout << c.room; cout << endl;
}

// ���ǽ� �ð�ǥ �����, �׽�Ʈ ������ �����ѰŶ� ���� �ʿ�
void printTimeTable(const string& room) {
    cout << "      ";
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
    cout << "press any key to continue ...";
    cin.ignore(); cin.get();  // �ƹ� Ű ���
}

// �α��� ���, �׽�Ʈ ������ �����ѰŶ� ���� �ʿ�
User* login() {
    string id, pw;
    cout << "ID: "; cin >> id;
    cout << "PW: "; cin >> pw;
    for (auto& u : users) {
        if (u.id == id && u.password == pw) return &u;
    }
    cout << ".!! ID or PW is incorrect\n";
    return nullptr;
}

// ���ǽ� �����ϴ� �Լ� ----------++ �����ڰ� ���� ���� �� �ð��̶� �Ȱ�ġ���� Ȯ�� �ϴ� ���� �߰� �ʿ�.
void reserveClassroom(const string& user_id) {
    string day, start, end;
    string room = InputClassroom();  // �Լ� ȣ���ϸ鼭 room�� �޾ƿ�
    cout << "day(1~5): "; cin >> day;
    cout << "start time(HH:MM): "; cin >> start;
    cout << "end time(HH:MM): "; cin >> end;

    // ��ġ�� �ð� �����ߴ��� üũ
    for (const auto& r : reservations) {
        if (r.user_id == user_id && r.day == day && isTimeOverlap(r.start_time, r.end_time, start, end)) {
            cout << ".!! Already reserved time\n";
            return;
        }
    }

    // ���ǽ� ���� ���ɽð� �ȿ� �ִ��� Ȯ��
    for (const auto& c : classrooms) {
        if (c.room == room && c.is_available && c.available_start <= start && c.available_end >= end) {
            reservations.push_back({ user_id, room, day, start, end });
            ofstream fout("reservation.txt", ios::app);
            fout << user_id << "\t" << room << "\t" << start << "\t" << end << "\t" << day << endl;
            cout << "Reservation completed\n";
            return;
        }
    }
    cout << ".!! This is not a time available for reservation\n";
}

//���� ��� ��� �� ���� �Լ� ȣ��
void showListAndEditReservation() {
    while (true) {
        cout << "1. register reservation\n2. checkreservation\n3. delete reservation\n>> ";
        int input; cin >> input;

        if(input == 1) { //������ID, ���ǽ� ȣ��, ���� �ð��� �Է� �ް� ���
        
        }
        else if(input == 2) { //���� ���� ����Ʈ ��� 6.2.1 reservation.txt
            
        }
        else if(input == 3) { //id�� �Է¹޾� �ش� ������� ���� ���� ���, ����� ���ǽ� ���
        
        }
        else{
            cout << ".!! Enter the index number in the menu.\n";
        }
    }
}


//���ǽ� ���� ��� �� ���� �Լ�- ������
void showAndEditClassroom(const string& admin_id) {
    while (true) {
        cout << "1. check reservation\n2. accept reservation\n3. ban reservation\n>> ";
        int input; cin >> input;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << ".!! Enter the index number in the menu.\n";
            continue;
        }

        if (input == 1) { // 6.3.2.1 check reservation
            printClassroomList();
            cout << "classroom number: ";
            string room; cin >> room;
            printTimeTable(room);
        }
        else if (input == 2) { // 6.3.2.2 accept reservation -> reservation.txt ���� ��ü ��� �����ϰ�
            cout << "classroom number: ";
            string room; cin >> room;
            bool roomFound = false;
            for (auto& c : classrooms) {
                if (c.room == room) {
                    roomFound = true;
                    cout << "enter a number corresponding to the day of the week\n"
                         << "(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";
                    int day; cin >> day;
                    if (day < 1 || day > 5) {
                        cout << ".!! Invalid weekday input\n";
                        break;
                    }
                    string start, end;
                    cout << "start accept time: "; cin >> start;
                    cout << "end accept time: "; cin >> end;

                    vector<Reservation> new_reservations;

                    // �ð� Ȯ�� �˻� �߰��ؾ���.
                    // ���� ���� ����Ʈ ��ȸ�ϸ鼭 ����(������) ���� �� ���ð���� ��ġ�� �κ� ����
                    for (auto it = reservations.begin(); it != reservations.end(); ) {
                        if (it->user_id == admin_id &&
                            it->room == room &&
                            it->day == to_string(day) &&
                            isTimeOverlap(it->start_time, it->end_time, start, end)) {
        
                            // ���ҵ� ���� �ð����� �����
                            if (it->start_time < start) {
                                new_reservations.push_back({ admin_id, room, to_string(day), it->start_time, start });
                            }
                            if (it->end_time > end) {
                                new_reservations.push_back({ admin_id, room, to_string(day), end, it->end_time });
                            }
        
                            // ���� ���� ���� ����
                            it = reservations.erase(it);
                        }
                        else {
                            ++it;
                        }
                    }
        
                    // ���ο� ���� ���� �߰�
                    for (const auto& r : new_reservations) {
                        reservations.push_back(r);
                    }
        
                    // ���� ��ü ����
                    ofstream fout("reservation.txt");
                    for (const auto& r : reservations) {
                        fout << r.user_id << "\t" << r.room << "\t" << r.start_time << "\t"
                             << r.end_time << "\t" << r.day << endl;
                    }
        
                    cout << "Accept completed.\n";
                    break;
                }
            }
                
            if (!roomFound) cout << ".!! Room not found\n";
        }
        else if (input == 3) { // 6.3.2.3 ban reservation ���� ����
            cout << "classroom number: ";
            string room; cin >> room;
            bool roomFound = false;
            for (auto& c : classrooms) {
                if (c.room == room) {
                    roomFound = true;
                    cout << "enter a number corresponding to the day of the week\n"
                         << "(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";
                    int day; cin >> day;
                    if (day < 1 || day > 5) {
                        cout << ".!! Invalid weekday input\n";
                        break;
                    }
                    string start, end;
                    cout << "start ban time: "; cin >> start;
                    cout << "end ban time: "; cin >> end;
                    // ������ ���Ϻ��� �����ϴ� ������ ������, ��ü �ð����� ���� ������� ��ü
                    reservations.push_back({admin_id, room, to_string(day), start, end});
                    ofstream fout("reservation.txt", ios::app);
                    fout << admin_id << "\t" << room << "\t" << start << "\t" << end << "\t" << day << endl;
                    cout << "Ban completed.\n";
                    break;
                }
            }
            if (!roomFound) cout << ".!! Room not found\n";
        }
        else {
            cout << ".!! Enter the index number in the menu.\n";
        }
        break;
    }
}



// ���� ��� ���
void cancelReservation(const string& user_id) {
    vector<int> indices;
    for (int i = 0; i < reservations.size(); ++i) {
        if (reservations[i].user_id == user_id) {
            cout << indices.size() + 1 << ". " << reservations[i].room << " " << weekdays[stoi(reservations[i].day) - 1] << " " << reservations[i].start_time << "~" << reservations[i].end_time << endl;
            indices.push_back(i);
        }
    }
    if (indices.empty()) {
        cout << "No reservations to cancel\n";
        return;
    }
    cout << "Enter the number you want to cancel: ";
    int choice; cin >> choice;
    if (choice < 1 || choice > indices.size()) {
        cout << ".!! Invalid input\n";
        return;
    }
    reservations.erase(reservations.begin() + indices[choice - 1]);
    ofstream fout("reservation.txt");
    for (auto& r : reservations) {
        fout << r.user_id << "\t" << r.room << "\t" << r.start_time << "\t" << r.end_time << "\t" << r.day << endl;
    }
    cout << "Reservation canceled\n";
}

// �����ڰ� ���ǽ� �����ϴ� �Լ�
void adminReserveClassroom(const string& user_id) {
    InputUser();
    reserveClassroom(user_id);
}

// ���α׷� ����
int main() {
    // ���� �ε� �ȵǸ� ������, ������ ���Ἲ ��Ʈ���� �߰��� ����
    if (!loadUsers() || !loadClassrooms() || !loadReservations()) {
        return 1;
    }

    while (true) {
        cout << "----Classroom Booking Program----\n";
        cout << "1. login\n2. accession\n3. exit\n>> ";
        int sel; cin >> sel;
        if (sel == 1) {
            User* user = nullptr;
            while (!user) user = login();
            if (user->is_admin) {
                cout << "-- Main for manager --\n";
                // ������ ��� - ������
                while (true){
                    cout << "1. reservation list and change\n2. classroom situation and change\n3. logout\n>> ";
                    int choice; cin >> choice;
                    if(choice == 1){
                        //���� ��� ��� �� ���� �Լ� ȣ��
                        
                    }
                    else if (choice == 2){
                        //���ǽ� ���� ��� �� ���� �Լ� ȣ��
                        showAndEditClassroom(user -> id);
                    }
                    else if (choice == 3){
                        break;
                    }
                    else{
                        cout << ".!! Enter the index number in the mune.\n";

                    }
                }
            }
            else {
                cout << "-- Main --\n";
                while (true) {
                    cout << "1. classroom list\n2. reserve classroom\n3. cancel reservation\n4. logout\n>> ";
                    int c; cin >> c;
                    if (c == 1) {
                        printClassroomList();
                        cout << "classroom number: ";
                        string room; cin >> room;
                        printTimeTable(room);
                    }
                    else if (c == 2) reserveClassroom(user->id);
                    else if (c == 3) cancelReservation(user->id);
                    else if (c == 4) break;
                }
            }
        }
        else if (sel == 2) {
            // ȸ������
            string id, pw;
            cout << "ID: "; cin >> id;
            cout << "PW: "; cin >> pw;
            users.push_back({ id, pw, false });
            ofstream fout("user.txt", ios::app);
            fout << id << "\t" << pw << "\t0\n";
            cout << "Registration complete\n";
        }
        else if (sel == 3) {
            // ���� Ȯ��
            string confirm;
            cout << "If you want to quit this program, enter 'quit': ";
            cin >> confirm;
            if (confirm == "quit") break;
        }
    }
    return 0;
}
