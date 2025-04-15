#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <regex>
#include <vector>
#include <set>
#include <string>
#include <map>

using namespace std;
namespace fs = filesystem;

const vector<string> requiredFiles = { "user.txt", "reservation.txt", "classroom.txt" };
/// <summary>
/// 주어진 문자열의 앞뒤 공백을 제거합니다.
/// </summary>
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    size_t last = str.find_last_not_of(" \t\n\r");
    return (first == string::npos) ? "" : str.substr(first, last - first + 1);
}
/// <summary>
/// 사간  형식이 HH:MM인지 확인합니다. timePattern을 조정하여 그 범위를 조절 가능합니다.
/// timePattern는 [01]?[0-9]|2[0-3]는 첫째자리가 0 OR 1이면 둘째자리는 0~9까지, 첫째자리가 2이면 둘째자리는 0~3까지 가능하다는 의미입니다.
/// </summary>
bool isValidTime(const string& time) {
    const string timePattern = "^([01]?[0-9]|2[0-3]):[0-5][0-9]$";
    return regex_match(time, regex(timePattern));
}
/// <summary>
/// 아이디가 형식에 맞는지 확인합니다.[a-z0-9]{3,20}는 a-z까지, 0-9까지, 3~20자까지 가능하다는 의미입니다.
/// </summary>
bool isValidID(const string& id) {
    const string idPattern = "^[a-z0-9]{3,20}$";
    return regex_match(id, regex(idPattern));
}
/// <summary>
/// 비밀번호가 형식에 맞는지 확인합니다. [A-Za-z]는 대소문자 알파벳, \\d는 숫자, ~!@#$%^&*()는 특수문자, 4~20자까지 가능하다는 의미입니다.
/// 비밀번호는 공백을 포함할 수 없습니다.
/// </summary>
bool isValidPassword(const string& pw) {
    if (pw.find(' ') != string::npos) return false;
    const string pwPattern = "^(?=.*[A-Za-z])(?=.*\\d)[A-Za-z\\d~!@#$%^&*()]{4,20}$";
    return regex_match(pw, regex(pwPattern));
}
/// <summary>
/// 강의실 번호가 형식에 맞는지 확인합니다. \\d{1,10}은 1~10자리 숫자까지 가능하다는 의미입니다.
/// </summary>
bool isValidRoomNumber(const string& num) {
    return regex_match(num, regex("^\\d{1,10}$"));
}

/// <summary>
/// 주어진 시간 문자열을 분으로 변환합니다.
/// </summary>
int timeToMinutes(const string& time) {
    int hour = stoi(time.substr(0, 2));
    int min = stoi(time.substr(3, 2));
    return hour * 60 + min;
}

/// <summary>
/// 주어진 분을 HH:MM 형식의 문자열로 변환합니다.
/// </summary>
bool checkFilesExistAndCreate() {
    bool ok = true;
    for (const auto& file : requiredFiles) {
        if (!fs::exists(file)) {
            cout << "[경고] 파일 없음: " << file << " -> 생성 중...\n";
            ofstream ofs(file);
            if (!ofs) {
                cerr << "[오류] 파일 생성 실패: " << file << endl;
                ok = false;
            }
        }
    }
    return ok;
}
/// <summary>
/// 주어진 파일에 대한 읽기 및 쓰기 권한을 확인합니다.
/// </summary>
bool checkFilePermissions() {
    bool ok = true;
    for (const auto& file : requiredFiles) {
        ifstream in(file);
        ofstream out(file, ios::app);
        if (!in.is_open() || !out.is_open()) {
            cerr << "[오류] 권한 부족: " << file << endl;
            ok = false;
        }
    }
    return ok;
}

set<string> registeredUsers;
map<string, pair<string, string>> classroomTime;

bool validateUserFile(const string& filename) {
    ifstream fin(filename);
    string line;
    set<string> idSet;
    int lineNum = 0;
    bool ok = true;
    while (getline(fin, line)) {
        lineNum++;
        string cleanedLine = line.substr(0, line.find('#'));
        istringstream iss(cleanedLine);
        string id, pw, adminFlag;
        iss >> id >> pw >> adminFlag;

        id = trim(id);
        pw = trim(pw);
        adminFlag = trim(adminFlag);

        if (id.empty() || pw.empty() || adminFlag.empty()) continue;
        if (!isValidID(id)) {
            cerr << "[오류] user.txt 라인 " << lineNum << ": 잘못된 ID 형식: " << id << endl;
            ok = false;
        }
        if (!isValidPassword(pw)) {
            cerr << "[오류] user.txt 라인 " << lineNum << ": 잘못된 비밀번호 형식: " << pw << endl;
            ok = false;
        }
        if (adminFlag != "0" && adminFlag != "1") {
            cerr << "[오류] user.txt 라인 " << lineNum << ": 관리자 여부는 0 또는 1이어야 함: " << adminFlag << endl;
            ok = false;
        }
        if (idSet.count(id)) {
            cerr << "[오류] user.txt 라인 " << lineNum << ": 중복된 ID 발견: " << id << endl;
            ok = false;
        }
        idSet.insert(id);
        registeredUsers.insert(id);
    }
    return ok;
}

bool validateClassroomFile(const string& filename) {
    ifstream fin(filename);
    string line;
    int lineNum = 0;
    bool ok = true;
    while (getline(fin, line)) {
        lineNum++;
        string cleanedLine = line.substr(0, line.find('#'));
        istringstream iss(cleanedLine);
        string room, status, start, end;
        iss >> room >> status >> start >> end;

        room = trim(room);
        status = trim(status);
        start = trim(start);
        end = trim(end);

        if (!room.empty() && status == "1") {
            classroomTime[room] = make_pair(start, end);
        }

        if (!isValidRoomNumber(room)) {
            cerr << "[오류] classroom.txt 라인 " << lineNum << ": 잘못된 강의실 번호: " << room << endl;
            ok = false;
        }
        if (status != "0" && status != "1") {
            cerr << "[오류] classroom.txt 라인 " << lineNum << ": 예약 가능 여부는 0 또는 1이어야 함: " << status << endl;
            ok = false;
        }
        if (!isValidTime(start) || !isValidTime(end)) {
            cerr << "[오류] classroom.txt 라인 " << lineNum << ": 잘못된 시간 형식: " << start << " - " << end << endl;
            ok = false;
        }
        if (start >= end) {
            cerr << "[오류] classroom.txt 라인 " << lineNum << ": 시작 시간이 종료 시간보다 같거나 늦음: " << start << " - " << end << endl;
            ok = false;
        }
    }
    return ok;
}

bool validateReservationFile(const string& filename) {
    ifstream fin(filename);
    string line;
    map<string, vector<pair<int, int>>> reservationTimeline;
    map<string, int> userTotalTime;
    int lineNum = 0;
    bool ok = true;

    while (getline(fin, line)) {
        lineNum++;
        string cleanedLine = line.substr(0, line.find('#'));
        istringstream iss(cleanedLine);
        string user, room, start, end;
        iss >> user >> room >> start >> end;

        user = trim(user);
        room = trim(room);
        start = trim(start);
        end = trim(end);

        if (user.empty() || room.empty() || start.empty() || end.empty()) continue;
        if (!isValidID(user) || !isValidRoomNumber(room) || !isValidTime(start) || !isValidTime(end) || start >= end) {
            cerr << "[오류] reservation.txt 라인 " << lineNum << ": 잘못된 예약 형식: " << line << endl;
            ok = false;
            continue;
        }
        if (!registeredUsers.count(user)) {
            cerr << "[오류] reservation.txt 라인 " << lineNum << ": 존재하지 않는 사용자 ID: " << user << endl;
            ok = false;
        }
        if (!classroomTime.count(room)) {
            cerr << "[오류] reservation.txt 라인 " << lineNum << ": 예약 불가능한 강의실 번호: " << room << endl;
            ok = false;
        }
        auto [open, close] = classroomTime[room];
        if (start < open || end > close) {
            cerr << "[오류] reservation.txt 라인 " << lineNum << ": 강의실 예약 가능 시간 외 요청: " << room << " " << start << "-" << end << endl;
            ok = false;
        }

        int s = timeToMinutes(start), e = timeToMinutes(end);
        for (const auto& [rs, re] : reservationTimeline[user]) {
            if (!(e <= rs || s >= re)) {
                cerr << "[오류] reservation.txt 라인 " << lineNum << ": 사용자 중복 예약 감지: " << user << " " << start << "-" << end << endl;
                ok = false;
            }
        }
        reservationTimeline[user].emplace_back(s, e);
        userTotalTime[user] += e - s;
        if (userTotalTime[user] > 180) {
            cerr << "[오류] reservation.txt 라인 " << lineNum << ": 사용자 예약 총합 3시간 초과: " << user << endl;
            ok = false;
        }
    }
    return ok;
}

void performIntegrityCheck() {
    cout << "[시작] 무결성 검사ing\n";
    bool hasError = false;

    if (!checkFilesExistAndCreate()) hasError = true;
    if (!checkFilePermissions()) hasError = true;
    if (!validateUserFile("user.txt")) hasError = true;
    if (!validateClassroomFile("classroom.txt")) hasError = true;
    if (!validateReservationFile("reservation.txt")) hasError = true;

    if (hasError) {
        cerr << "[종료] 무결성 검사 실패\n";
        exit(1);
    }
    else {
        cout << "[완료] 무결성 검사 통과\n";
    }
}

int main() {
    performIntegrityCheck();
    return 0;
}
