#pragma once
#include <string>
#include "avatar.h"

using namespace std;

class User {
private:
    string fName, lName, email, password;
    Avatar avatar;

public:
    User() : avatar() {}
    User(string f, string l, string e, string p, int lvl, int xp) 
        : fName(f), lName(l), email(e), password(p), avatar(lvl, xp) {}

    // Getters
    string getFName() const { return fName; }
    string getLName() const { return lName; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    Avatar& getAvatar() { return avatar; }
    const Avatar& getAvatar() const { return avatar; }

    // Setters
    void setFName(string f) { fName = f; }
    void setLName(string l) { lName = l; }
    void setEmail(string e) { email = e; }
    void setPassword(string p) { password = p; }
};
