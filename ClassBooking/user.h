#pragma once
#include <string>

class User {
private:
    std::string id;
    std::string pw;
    bool isAdmin;

public:
    User();
    User(const std::string& id, const std::string& pw, bool isAdmin);
    std::string getId() const;
    std::string getPassword() const;
    bool getIsAdmin() const;
    void setId(const std::string& newId);
    void setPassword(const std::string& newPw);
    void setIsAdmin(bool status);
};