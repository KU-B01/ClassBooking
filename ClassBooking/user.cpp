#include "user.h"

User::User() : id(""), pw(""), isAdmin(false) {}

User::User(const std::string& id, const std::string& pw, bool isAdmin)
    : id(id), pw(pw), isAdmin(isAdmin) {}

std::string User::getId() const {
    return id;
}

std::string User::getPassword() const {
    return pw;
}

bool User::getIsAdmin() const {
    return isAdmin;
}

void User::setId(const std::string& newId) {
    id = newId;
}

void User::setPassword(const std::string& newPw) {
    pw = newPw;
}

void User::setIsAdmin(bool status) {
    isAdmin = status;
}