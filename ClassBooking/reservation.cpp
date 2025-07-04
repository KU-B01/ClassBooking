#include "reservation.hpp"
#include "models.hpp"
#include "util.hpp"
#include "classroom.hpp"
#include "user.hpp"
#include "time.hpp"
#include "admin.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

// 예약 정보 파일 로드
// 예약 불러오는 함수
bool loadReservations()
{
    ifstream fin("reservation.txt"); // .cpp용
    
    if (!fin)
    {
        cerr << "[Warning] reservation.txt file not found. Creating empty reservation.txt...\n";
        ofstream fout("reservation.txt");
        if (!fout)
        {
            cerr << "[Error] Failed to create reservation.txt.\n";
            exit(1);
        }
        fout.close();
        return true; // 빈 파일 만들고 정상 진행
    }

    fin.seekg(0, ios::end);
    if (fin.tellg() == 0)
    {
        fin.close();
        return true;
    }
    fin.seekg(0);

    string id, room, start, end, day;
    int lineNum = 0;

    while (fin >> id >> room >> start >> end >> day)
    {
        ++lineNum;

        if (!isExistUser(id))
        {
            cerr << "[Error] Line " << lineNum << ": User ID not found -> " << id << endl;
            exit(1);
        }

        if (!isExistRoomNumber(room))
        {
            cerr << "[Error] Line " << lineNum << ": Classroom not found -> " << room << endl;
            exit(1);
        }

        if (!isValidTime(start) || !isValidTime(end))
        {
            cerr << "[Error] Line " << lineNum << ": Invalid time format (must be HH:00).\n";
            exit(1);
        }

        if (start >= end)
        {
            cerr << "[Error] Line " << lineNum << ": Start time must be earlier than end time.\n";
            exit(1);
        }

        if (day != "1" && day != "2" && day != "3" && day != "4" && day != "5")
        {
            cerr << "[Error] Line " << lineNum << ": Invalid day value (must be 1~5). (got " << day << ")\n";
            exit(1);
        }

        reservations.push_back({id, room, day, start, end});
        lineCount_r++;
    }

    fin.close();
    return true;
}

// 사용자 예약 등록 처리 (내부 사용)
bool reserveClassroom(const std::string user_id)
{
    string local_id = user_id; // 내부에서 수정 가능한 복사본
    string day, start, end;
    bool is_admin = false;

    while (true)
    {
        // 2차 구현 사용자 밴 여부 확인
        if (isUserBanned(local_id)) {
        cout << "This account can't reserve.\n\n";
        return false;
        }
        
        is_admin = false;

        // --- 사용자 유형 확인 (admin 여부) ---
        for (const auto &u : users)
        {
            if (u.id == local_id)
            {
                is_admin = u.is_admin;
                break;
            }
        }

        if (!is_admin)
        {
            break; // 관리자가 아니면 루프 탈출
        }

        // 관리자이면 경고하고 다시 입력받음
        std::cout << "!! Admin users are not allowed to perform this operation.\n";
        local_id = InputUser();
    }

    string room = InputClassroom();

    // --- 요일 입력 유효성 검사 ---
    while (true)
    {   
        cout << "Enter a number corresponding to the day of the week\n(1. Mon, 2. Tue, 3. Wed, 4. Thu, 5. Fri): ";

        cin.clear();
        while (cin.peek() == '\n')
            cin.ignore(); // 개행만 남은 버퍼 날리기
            
        getline(cin, day);
        if (checkIdx(day))
        {
            printIdxErrorMessage("day");
            continue;
        }

        int dayInt = stoi(day);
        if (dayInt < 1 || dayInt > 5)
        {
            cout << ".!! Enter the index number of the day of the week.\n";
            continue;
        }

        // 2차 구현 강의실 예약 시 가상 시각의 요일보다 빠른 요일이 들어왔을 때 차단
        int currentWday = getCurrentWeekdayIndex();  // 0 = Mon, ..., 4 = Fri
        if (!is_admin && (dayInt - 1) < currentWday)
        {
            cout << ".!! It is already past time. You can't reserve on that day.\n\n";
            continue;
        } 
        break;
    }

    // --- 시간 입력 유효성 검사 ---
    while (true)
    {
        cout << "start time (HH:00): ";
        cin >> start;
        cout << "end time (HH:00): ";
        cin >> end;

        regex pattern("^(0[0-9]|1[0-9]|2[0-3]):00$");
        if (!regex_match(start, pattern) || !regex_match(end, pattern))
        {
            cout << ".!! Incorrect form: an example of correct input is '14:00'.\n";
            continue;
        }

        int sh = stoi(start.substr(0, 2));
        int eh = stoi(end.substr(0, 2));

        if (sh < 9 || sh > 17)
        {
            cout << ".!! Incorrect form: start must be between 09:00 and 17:00.\n";
            continue;
        }
        if (eh < 10 || eh > 18)
        {
            cout << ".!! Incorrect form: end must be between 10:00 and 18:00.\n";
            continue;
        }
        if (sh >= eh)
        {
            cout << ".!! Start time must be earlier than end time.\n";
            continue;
        }

        if (!is_admin && eh - sh != 1)
        {
            cout << ".!! General users can only reserve exactly 1 hour.\n";
            continue;
        }

        // 2차 구현 부분 time.cpp
        if (!is_admin && isBeforeVirtualTime(day, start)) {
            cout << ".!! It is already past time. You can't reserve at that time.\n\n";
            continue;
        }

        break;
    }

    int newStartHour = stoi(start.substr(0, 2));
    int newEndHour = stoi(end.substr(0, 2));
    int newDuration = newEndHour - newStartHour;

    // --- 2차 구현 주간 총합 3시간 --> 7시간 초과 여부 검사 ---
    int totalReservedHours = 0;
    for (const auto &r : reservations)
    {
        if (r.user_id == local_id)
        {
            int startHour = stoi(r.start_time.substr(0, 2));
            int endHour = stoi(r.end_time.substr(0, 2));
            totalReservedHours += (endHour - startHour);
        }
    }

    if (!is_admin && (totalReservedHours + newDuration > 7))
    {
        cout << ".!! You exceeded the weekly reservation limit of 7 hours.\n";
        return false;
    }

    // 2차 구현 일간 총합 5시간 초과 여부 검사
    int dailyReservedHours = 0;
    for (const auto &r : reservations)
    {
        if (r.user_id == local_id && r.day == day)
        {
            int startHour = stoi(r.start_time.substr(0, 2));
            int endHour = stoi(r.end_time.substr(0, 2));
            dailyReservedHours += (endHour - startHour);
        }
    }

    if (!is_admin && (dailyReservedHours + newDuration > 5))
    {
        cout << ".!! You exceeded the daily reservation limit of 5 hours.\n";
        return false;
    }

    // 2차 구현 동일 강의실 연속 3시간 초과 여부 검사
    // 이건 어떻게 막아야할 지 몰라서 100% AI 활용했습니다.
    // 더 쉬운 방법이 있다면 트라이해보시는 게 좋을 것 같습니다.
    std::vector<std::pair<int, int>> timeBlocks;
    for (const auto &r : reservations)
    {
        if (r.user_id == local_id && r.day == day && r.room == room)
        {
            int s = stoi(r.start_time.substr(0, 2));
            int e = stoi(r.end_time.substr(0, 2));
            timeBlocks.emplace_back(s, e);
        }
    }
    timeBlocks.emplace_back(newStartHour, newEndHour);
    sort(timeBlocks.begin(), timeBlocks.end());

    int currentConsecStartHour = timeBlocks[0].first;
    int currentConsecEndHour = timeBlocks[0].second;
    int maxConsecutiveHours = 0;

    for (size_t i = 1; i < timeBlocks.size(); ++i) {
        int currStart = timeBlocks[i].first;
        int currEnd = timeBlocks[i].second;

        if (currStart == currentConsecEndHour) {
            currentConsecEndHour = currEnd;
        } else {
            maxConsecutiveHours = max(maxConsecutiveHours, currentConsecEndHour - currentConsecStartHour);
            currentConsecStartHour = currStart;
            currentConsecEndHour = currEnd;
        }
    }

    maxConsecutiveHours = max(maxConsecutiveHours, currentConsecEndHour - currentConsecStartHour);

    if (!is_admin && maxConsecutiveHours > 3) {
        cout << ".!! You cannot reserve more than 3 consecutive hours in the same room.\n";
        return false;
    }

    // --- 예약 충돌 확인 ---
    for (const auto &r : reservations)
    {
        if (r.room == room && r.day == day && isTimeOverlap(r.start_time, r.end_time, start, end))
        {
            cout << ".!! Already reserved time.\n";
            return false;
        }
    }

    // --- 관리자(admin)가 설정한 금지 예약과 겹치는지 검사 ---
    for (const auto &r : reservations)
    {
        if (r.room == room && r.day == day)
        {
            if (r.user_id.find("admin") == 0 && isTimeOverlap(r.start_time, r.end_time, start, end))
            {
                cout << ".!! This time is not available (admin banned time).\n";
                return false;
            }
        }
    }

    // --- 강의실 예약 가능 시간 확인 ---
    for (const auto &c : classrooms)
    {
        if (c.room == room && c.is_available && c.available_start <= start && c.available_end >= end)
        {
            reservations.push_back({local_id, room, day, start, end});
            ofstream fout("reservation.txt", ios::app);
            fout << local_id << "\t" << room << "\t" << start << "\t" << end << "\t" << day << endl;
            fout.close();
            cout << "Reservation completed\n";
            return true;
        }
    }

    cout << ".!! This is not a time available for reservation\n";
    return false;
}


// 예약 취소 처리 함수
bool cancelReservation(const string& user_id) {
    bool flag = false; // 루프 돌리기
    vector<int> indices;
    int reservation_number = 0; // 인덱스 유효 검사용
    for (size_t i = 0; i < reservations.size(); ++i)
    {
        if (reservations[i].user_id == user_id)
        {
            cout << indices.size() + 1 << ". " << reservations[i].room << " " << weekdays[stoi(reservations[i].day) - 1] << " " << reservations[i].start_time << "~" << reservations[i].end_time << endl;
            indices.push_back(i);
            reservation_number++;
        }
    }
    if (indices.empty())
    {
        cout << "No reservations to cancel\n";
        flag = true;
        return flag;
    }

    int idx_r = -1;
    while (true) {
        cout << "Enter the number you want to cancel: ";
        string choice;
        cin.clear();
        while (cin.peek() == '\n')
            cin.ignore();
        getline(cin, choice);

        if (checkIdx(choice)) {
            printIdxErrorMessage("menu");
            continue;
        }

        idx_r = stoi(choice);
        if (idx_r < 1 || idx_r > reservation_number) {
            printIdxErrorMessage("menu");
            continue;
        }

        break; // 유효한 입력 받으면 루프 탈출
    }

    reservations.erase(reservations.begin() + indices[idx_r - 1]);
    ofstream fout("reservation.txt");
    for (auto &r : reservations)
    {
        fout << r.user_id << "\t" << r.room << "\t" << r.start_time << "\t" << r.end_time << "\t" << r.day << endl;
    }
    cout << "Reservation canceled.\n";
    flag = true;
    return flag;
}
