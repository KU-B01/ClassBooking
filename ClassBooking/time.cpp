// 전부 2차 구현
#define _CRT_SECURE_NO_WARNINGS
#include "time.hpp"
#include "models.hpp"
#include "util.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>

std::string g_virtualTimeStr;
std::tm g_virtualTimeTm = {};

std::tm parseTime(const std::string& timeStr) {
    std::tm tm = {};
    std::istringstream ss(timeStr);
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M");
    return tm;
}

bool isTimeEarlier(const std::tm& t1, const std::tm& t2) {
    return std::mktime(const_cast<std::tm*>(&t1)) <= std::mktime(const_cast<std::tm*>(&t2));
}

std::string loadPreviousTime() {
    std::ifstream fin("time.txt");
    std::string prev;
    if (fin >> std::ws && std::getline(fin, prev)) {
        if (prev.empty()) return "";
        return prev;
    }
    return "";
}

void saveVirtualTime(const std::string& timeStr) {
    std::ofstream fout("time.txt");
    fout << timeStr << std::endl;
}

std::string getVirtualTimeFromUser() {
    std::string input;

    while (true) {
        std::cout << "Enter Current time (YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, input);
        
        if (isValidDateTime(input)) {
            return input;
        }
        
        std::cout << ".!! Incorrect format or invalid date/time.\n";
    }
}

int getISOWeekNumber(const std::tm& tm) {
    std::tm temp = tm;
    std::mktime(&temp);
    char buffer[8];
    std::strftime(buffer, sizeof(buffer), "%W", &temp); // 월요일이 기준(일주일의 시작작)
    return std::stoi(buffer);
}

bool isNewWeek(const std::tm& prev, const std::tm& curr) {
    return getISOWeekNumber(prev) != getISOWeekNumber(curr);
}

void resetReservationsIfNewWeek(const std::tm& prev, const std::tm& curr) {
    if (isNewWeek(prev, curr)) {
        std::ofstream fout("reservation.txt", std::ios::trunc);
        fout.close();
        reservations.clear();
        std::cout << "[Info] A new week has started. reservation.txt has been cleared.\n";
    }
}

void loadTime() {
    std::ifstream fin("time.txt");
    if (!fin) {
        std::cerr << "[Warning] time.txt not found. Creating empty time.txt\n";
        std::ofstream fout("time.txt");
        if (!fout) {
            std::cerr << "[Error] Failed to create time.txt\n";
            exit(1);
        }
        fout.close();
        return;
    }

    std::string prev;
    getline(fin, prev);
    fin.close();

    if (prev.empty()) {
        std::cerr << "[Warning] time.txt is empty. Virtual time will be requested.\n";
        return;
    }

    if (!isValidDateTime(prev)) {
        std::cerr << "[Error] Invalid time format in time.txt. Exiting...\n";
        exit(1);
    }
};

void VirtualTime() {
    std::string prev = loadPreviousTime();

    if (!prev.empty()) {
        std::cout << "last time: " << prev << "\n";
    }

    while (true) {
        g_virtualTimeStr = getVirtualTimeFromUser();
        g_virtualTimeTm = parseTime(g_virtualTimeStr);

        if (!prev.empty()) {
            std::tm prevTm = parseTime(prev);

            if (isTimeEarlier(g_virtualTimeTm, prevTm)) {
                std::cout << ".!! Cannot go back in time. Please enter a time later than " << prev << "\n";
                continue;
            }

            resetReservationsIfNewWeek(prevTm, g_virtualTimeTm);
        }

        break;
    }

    saveVirtualTime(g_virtualTimeStr);
}

int getCurrentWeekdayIndex() {
    std::time_t t = std::mktime(&g_virtualTimeTm);
    std::tm* normalized = std::localtime(&t);
    int wday = normalized->tm_wday;
    return (wday == 0 ? 6 : wday - 1);
}

int getCurrentHour() {
    return g_virtualTimeTm.tm_hour;
}


bool isBeforeVirtualTime(const std::string& day, const std::string& start) {
    int inputDay = std::stoi(day) - 1; // 입력 요일 (1~5) → 0~4
    int inputHour = std::stoi(start.substr(0, 2));
	int inputMinute = std::stoi(start.substr(3, 2));

    int curDay = getCurrentWeekdayIndex();
    int curHour = getCurrentHour();

    return (inputDay < curDay) 
        || (inputDay == curDay && inputHour < curHour);
}


