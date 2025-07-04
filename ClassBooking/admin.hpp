#ifndef ADMIN_HPP
#define ADMIN_HPP

#include <string>

using namespace std;

// 2차 구현: 관리자 프롬프트 (관리자 로그인 후 표시되는 메뉴)
bool managerPrompt(const string& admin_id);

// 관리자: 예약 목록 조회 및 삭제 (기능 선택 포함)
void reservationManagementMenu();

// 관리자: 강의실 예약 현황 확인 및 설정 (기능 선택 포함)
void classroomManagementMenu(const string &admin_id);

void reservation_check_admin1();

// 2차 구현 사용자 밴
void UserBanManagementMenu();
void acceptUser();
void banUser();
bool isUserBanned(const string& id);

// 2차 구현 강의실 추가/삭제
void ClassroomEditManagementMenu();
void addClassroom();
void deleteClassroom();



#endif
