// 전부 2차 구현
#include "time.hpp"
#include "models.hpp"
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
    return std::mktime(const_cast<std::tm*>(&t1)) < std::mktime(const_cast<std::tm*>(&t2));
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
    std::regex pattern(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}$)");

    while (true) {
        std::cout << "Enter virtual time (YYYY-MM-DD HH:MM): ";
        std::getline(std::cin, input);
        if (std::regex_match(input, pattern)) return input;
        std::cout << ".!! Incorrect format.\n";
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
    bool firstRun = !fin;
    fin.close();

    std::string prev = loadPreviousTime();
    if (prev.empty()) {
        std::cerr << "[Warning] time.txt not found or is empty.\n";
        firstRun = true;

        std::ofstream fout("time.txt");
        if (!fout) {
            std::cerr << "[Error] Failed to create time.txt\n";
            exit(1);
        }
        fout.close();
    }

    g_virtualTimeStr = getVirtualTimeFromUser();
    g_virtualTimeTm = parseTime(g_virtualTimeStr);

    if (!firstRun) {
        std::tm prevTm = parseTime(prev);
        if (isTimeEarlier(g_virtualTimeTm, prevTm)) {
            std::cerr << ".!! Cannot go back in time. Exiting...\n";
            exit(1);
        }
        resetReservationsIfNewWeek(prevTm, g_virtualTimeTm);
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

    int curDay = getCurrentWeekdayIndex();
    int curHour = getCurrentHour();

    return (inputDay < curDay) || (inputDay == curDay && inputHour < curHour);
}
