#include <iostream>
#include <fstream>
#include <vector>
#include <sstream> // input parsing��
#include <iomanip>
#include <conio.h>  // kbhit, getch --> windows��, mac������ termios ���
using namespace std;

// ���ǽ� ����ü
struct Classroom {
    string room;
    bool is_available;
    string available_start;
    string available_end;
};

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

vector<Classroom> classrooms;     // ���ǽ� ���� ����
vector<User> users;                 // ����� ���� ����
vector<Reservation> reservations;   // ���� ����
string classroomNum;                  // �Է¹޴� ���ǽ� ��ȣ
bool triggered = false;     // main �ƹ� Ű �Է� ���ѷ����� ����

// ���� ����Ʈ
vector<string> weekdays = { "Mon", "Tue", "Wed", "Thu", "Fri" };
// �ð��� ����Ʈ��, 1�ð� ����
vector<string> times = {
    "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00", "17:00", "18:00"
};

////////////////////////////////////////////////////////////////////////////////////////////
//����� ������Ʈ �ҷ�����

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

// ���ǽ� ����Ʈ ���
void printClassroomList() {
    cout << "3F: "; for (auto& c : classrooms) if (c.room[0] == '3') cout << c.room << ", "; cout << endl;
    cout << "4F: "; for (auto& c : classrooms) if (c.room[0] == '4') cout << c.room << ", "; cout << endl;
    cout << "5F: "; for (auto& c : classrooms) if (c.room[0] == '5') cout << c.room << ", "; cout << endl;
    cout << "6F: "; for (auto& c : classrooms) if (c.room[0] == '6') cout << c.room << ", "; cout << endl;
}

// ���ǽ� ��ȣ�� �Է¹޴� �Լ�
string getClassroomNum() {
    string input;
    cout << "classroom number: ";
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
    cout << "The classroom you entered doesn't exist." << endl;
    return false;
}

// �ð� ��ġ���� Ȯ���ϴ� �Լ�
bool isTimeOverlap(const string& s1, const string& e1, const string& s2, const string& e2) {
    return !(e1 <= s2 || s1 >= e2);
}

// ���� ���� ��� �Լ�
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

    // ��ȿ�� ���ǽ� ��ȣ�� �Էµ� ������ �ݺ�
    while (true) {
        string input = getClassroomNum();
        if (checkClassroomNum(input)) {
            printTimeTable(classroomNum);
            break;
        }
    }

    cout << "Press any key to continue..." << endl;

    // �ƹ� Ű �Է� ���
    while (true) {
        if (!triggered && _kbhit()) {
            _getch();     // �Է¹���

            // ����� ������Ʈ�� ���ư�
            // userPrompt();

            break;  // ��� ����
        }
    }

    cout << "finished." << endl;
    return 0;
}
