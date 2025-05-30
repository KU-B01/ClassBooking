// 전부 2차 구현 
#pragma once
#include <string>
#include <ctime>

// time.txt에서 이전 가상 시간 불러오기
std::string loadPreviousTime();

// time.txt에 현재 가상 시간 저장
void saveVirtualTime(const std::string& timeStr);

// 사용자에게 가상 시간 입력받기 (YYYY-MM-DD HH:MM)
std::string getVirtualTimeFromUser();

// 문자열을 std::tm 구조로 파싱
std::tm parseTime(const std::string& timeStr);

// 두 std::tm 간 시간 비교: t1 < t2 이면 true
// 타임머신 방지용
bool isTimeEarlier(const std::tm& t1, const std::tm& t2);

// 가상 시간 전체 로드 및 검사 함수 (main에서 무결성 용)
// time.txt 있는지 확인 없으면 만들고, 있는데 비어있으면 비어있다고 말해주고,
// 이상한 것 쓰여있으면 에러 출력하고 exit
void loadTime();

// 로그인 직후 가상시각 받는 함수, 시간 유효성 검사
void handleVirtualTime();

// 전역 가상 시간 (문자열 + 구조체)
extern std::string g_virtualTimeStr;
extern std::tm g_virtualTimeTm;

// 요일 계산 함수 (월(0) ~ 금(4))
// 단순히 컴퓨터 요일 계산 방식일뿐 우리 구현할 때 (1~5)이므로
// 다른 로직에서는 -1해서 계산 요망
// 이전 시간 예약 방지용용
int getCurrentWeekdayIndex();

// 시간 추출 함수 (시 단위)
// 이전 시간 예약 방지용
int getCurrentHour();

// 사용자가 입력한 예약 시간이 가상 시간보다 이른지 판단
// 이전 시간 예약 방지용
bool isBeforeVirtualTime(const std::string& day, const std::string& start);

// 주차 비교 함수 (ISO 주차 번호)
// reservation.txt 초기화 용
int getISOWeekNumber(const std::tm& tm);

// 두 시간의 주차가 다른지 여부 확인
// reservation.txt 초기화 용
bool isNewWeek(const std::tm& prev, const std::tm& curr);

// 주차 변경 시 예약 초기화 처리
// reservation.txt 초기화 용
void resetReservationsIfNewWeek(const std::tm& prev, const std::tm& curr);
