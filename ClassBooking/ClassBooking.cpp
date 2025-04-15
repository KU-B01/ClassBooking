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
#include <conio.h>  // kbhit, getch --> windows��, mac������ termios ���
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

//����� list��ȸ�� ����
string classroomNum;                  // �Է¹޴� ���ǽ� ��ȣ
bool triggered = false;     // main �ƹ� Ű �Է� ���ѷ����� ����

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


// ���ǽ� ��ȣ�� �Է¹޴� �Լ�
string getClassroomNum() {

    string input;
    cout << "classroom number: ";
    // ��Ʈ�� ����(���� �޽��� ���� ���)
    cin.ignore(numeric_limits<streamsize>::max(), ' ');
    getline(cin, input);  //��ü ���ڿ��� ����

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

// ���ǽ� ��ȣ ��ȿ�� Ȯ��
bool checkClassroomNum(string classroomNum) {
    for (auto& c : classrooms) {
        if (classroomNum == c.room) {   //���ڿ� ��
            return true;
        }
    }
    // classroom.txt�� ���� ���ǽ� ��ȣ�� ���
    return false;
}

// index�� �Է¹��� �� ��ȿ�� �˻� �Լ�
bool checkIdx(string inputIdx) {
    if (inputIdx.length() == 1 && inputIdx[0] >= '0' && inputIdx[0] <= '9') {
        return false;
    }
    cout << ".!! Enter the index number in the menu." << endl;
    return true;
}

// ���ǽ� ������ �������, �״�� �ᵵ �ɵ�
void printClassroomList() {
    cout << endl;
    cout << "3F: "; for (auto& c : classrooms) if (c.room[0] == '3') cout << c.room << ", "; cout << endl;
    cout << "4F: "; for (auto& c : classrooms) if (c.room[0] == '4') cout << c.room << ", "; cout << endl;
    cout << "5F: "; for (auto& c : classrooms) if (c.room[0] == '5') cout << c.room << ", "; cout << endl;
    cout << "6F: "; for (auto& c : classrooms) if (c.room[0] == '6') cout << c.room << ", "; cout << endl;
}

// ���ǽ� �ð�ǥ �����, �׽�Ʈ ������ �����ѰŶ� ���� �ʿ�
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

// 6.2.1 ����� ������Ʈ - ��� ��ȸ
int userClassroomList() {
    string anyInput = "";

    //���� ��ȿ�� Ȯ�� - ���α׷� ���� �� Ȯ���ؼ� �ʿ���� ���� ���� �� ����
    // if (!loadClassrooms()) {
    //     cout << "Failed to load classrooms from file!" << endl;
    //     return 1;
    // }

    printClassroomList();

    // ��ȿ�� ���ǽ� ��ȣ�� �Էµ� ������ �ݺ�
    while (true) {
        string input = getClassroomNum();
        if (checkClassroomNum(input)) {
            printTimeTable(classroomNum);
            break;
        }
        else {
            cout << ".!! The classroom you entered doesn't exist." << endl;
            continue;
        }
    }

    cout << "Press any key to continue..." << endl;

    // �ƹ� Ű �Է� ���
    while (true) {
        if (!triggered && _kbhit()) {
            _getch();     // �Է¹���
            cout << endl;
            break;  // ��� ����
        }
    }

    return 0;
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

// ���ǽ� �����ϴ� �Լ�
void reserveClassroom(const string& user_id) {
    string room, day, start, end;
    cout << "classroom number: "; cin >> room;
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

// ���α׷� ����
int main() {
    // ���� �ε� �ȵǸ� ������, ������ ���Ἲ ��Ʈ���� �߰��� ����
    if (!loadUsers() || !loadClassrooms() || !loadReservations()) {
        return 1;
    }

    while (true) {
        cout << "\n----Classroom Booking Program----\n";
        cout << "1. login\n2. accession\n3. exit\n>> ";
        // sel type int => string (���ڿ��� �Էµ� ���� ����)
        string sel; cin >> sel;
        
        if (checkIdx(sel)) continue;

        if (stoi(sel) == 1) {
            User* user = nullptr;
            while (!user) user = login();
            if (user->is_admin) {
                cout << "\n-- Main for manager --\n";
                // ������ ����� ���� ���� �ȵ�
            }
            else {
                cout << "\n-- Main --\n";
                while (true) {
                    cout << "1. classroom list\n2. reserve classroom\n3. cancel reservation\n4. logout\n>> ";
                    string c; cin >> c;
                    if (checkIdx(c)) continue;
                    if (stoi(c) == 1) {
                        userClassroomList();
                    }
                    else if (stoi(c) == 2) reserveClassroom(user->id);
                    else if (stoi(c) == 3) cancelReservation(user->id);
                    else if (stoi(c) == 4) break;
                }
            }
        }
        else if (stoi(sel) == 2) {
            // ȸ������
            string id, pw;
            cout << "ID: "; cin >> id;
            cout << "PW: "; cin >> pw;
            users.push_back({ id, pw, false });
            ofstream fout("user.txt", ios::app);
            fout << id << "\t" << pw << "\t0\n";
            cout << "Registration complete\n";
        }
        else if (stoi(sel) == 3) {
            // ���� Ȯ��
            string confirm;
            cout << "If you want to quit this program, enter 'quit': ";
            cin >> confirm;
            if (confirm == "quit") break;
        }
    }
    return 0;
}
