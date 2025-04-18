#include "util.h"
// trim 함수, 선택자 입력받을 때 횡공백류열을 입력받아도
// 올바른 입력으로 받기 위한 함수

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}