/*
 * 2025 04 14
 * 일반 사용자 예약 구현용 버전
 */

// 기본 입출력, 파일입출력, 문자열처리, 벡터 라이브러리 가져옴
#include "admin.hpp"
#include "models.hpp"
#include "util.hpp"
#include "classroom.hpp"
#include "reservation.hpp"
#include "user.hpp"
#include "time.hpp"

#include <iostream>  // 입출력 기능 사용
#include <fstream>   // 파일 입출력 사용
#include <sstream>   // 문자열 스트림 처리용
#include <string>    // 문자열 타입 사용
#include <vector>    // 동적 배열 벡터 사용
#include <iomanip>   // 출력 정렬용 (setw 쓸 때 필요)
#include <limits>    // numeric_limits 사용을 위해 추가
#include <algorithm> // max 함수 사용을 위해 추가
#include <set>
#include <regex>
#include <map>

// 프로그램 시작
int main()
{
    // 2차 구현 loadTime() 추가
    loadTime();
    loadUsers();
    loadClassrooms();
    loadReservations();

    while (true)
    {
        cout << "----Classroom Booking Program----\n";
        cout << "1. login\n2. accession\n3. exit\n>> ";
        int choice = getValidatedMenuIndex("menu", 1, 3);

        // 로그인
        if (choice == 1)
        {
            User *user = nullptr;
            while (!user)
                user = login();
            
            // 2차 구현    
            VirtualTime();

            if (user->is_admin)
            {
                // 관리자 프롬프트
                while (true)
                {
                    if (!managerPrompt(user->id)) break; // 로그아웃 시 루프 탈출
                }
            }
            else
            {
                // 사용자 프롬프트
                while (true)
                {
					if (!userPrompt(user->id)) break; // 로그아웃 시 루프 탈출
                }
            }
        }
        // 회원가입
        else if (choice == 2)
            registerUser();
        // 프로그램 종료
        else if (choice == 3)
        {
            string confirm;
            cout << "If you want to quit this program, enter 'quit': ";
            cin >> confirm;
            if (confirm == "quit")
                break;
        }
    }
    return 0;
}
