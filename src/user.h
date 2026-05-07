#pragma once
#include <string>
#include "avatar.h"
#include "goals.h"
using namespace std;

class User
{
private:
    string fName, lName, email, password;
    Avatar avatar;
    std::vector<Goal *> myGoals;

public:
    User() : avatar() {}
    User(string f, string l, string e, string p, int lvl, int xp)
        : fName(f), lName(l), email(e), password(p), avatar(lvl, xp) {}

    // Getters
    string getFName() const { return fName; }
    string getLName() const { return lName; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    Avatar &getAvatar() { return avatar; }
    const Avatar &getAvatar() const { return avatar; }

    // Setters
    void setFName(string f) { fName = f; }
    void setLName(string l) { lName = l; }
    void setEmail(string e) { email = e; }
    void setPassword(string p) { password = p; }

    // Goal management
    void addGoal(Goal *goal)
    {
        myGoals.push_back(goal);
    }
    void displayGoals() const
    {
        for (const auto &goal : myGoals)
        {
            cout << "ID: " << goal->getId() << ", Title: " << goal->getTitle()
                 << ", Status: " << (goal->getStatus() ? "Completed" : "Incomplete") << endl;
        }
    }
    void removeGoal(int id)
    {

        auto it = std::find_if(myGoals.begin(), myGoals.end(),
                               [id](Goal *g)
                               { return g->getId() == id; });

        if (it != myGoals.end())
        {
            delete *it;
            myGoals.erase(it);
        }
    }
    const std::vector<Goal *> &getGoals() const { return myGoals; }
};
