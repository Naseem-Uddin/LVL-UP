#pragma once
#include <algorithm>
#include <string>
#include <vector>
#include "avatar.h"
#include "goals.h"
using namespace std;

class User
{
private:
    string fName, lName, email, password;
    Avatar avatar;
    std::vector<Goal *> myGoals;

    void clearGoals()
    {
        for (Goal *goal : myGoals)
        {
            delete goal;
        }
        myGoals.clear();
    }

public:
    User() : avatar() {}
    User(string f, string l, string e, string p, int lvl, int xp)
        : fName(f), lName(l), email(e), password(p), avatar(lvl, xp) {}

    User(const User &other)
        : fName(other.fName), lName(other.lName), email(other.email), password(other.password), avatar(other.avatar)
    {
        myGoals.reserve(other.myGoals.size());
        for (Goal *goal : other.myGoals)
        {
            myGoals.push_back(goal->clone());
        }
    }

    User &operator=(const User &other)
    {
        if (this == &other)
            return *this;

        clearGoals();
        fName = other.fName;
        lName = other.lName;
        email = other.email;
        password = other.password;
        avatar = other.avatar;

        myGoals.reserve(other.myGoals.size());
        for (Goal *goal : other.myGoals)
        {
            myGoals.push_back(goal->clone());
        }
        return *this;
    }

    ~User()
    {
        clearGoals();
    }

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
        if (myGoals.empty())
        {
            cout << "No goals have been added yet.\n";
            return;
        }

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

    int getNextGoalId() const
    {
        int maxId = 0;
        for (const Goal *goal : myGoals)
        {
            if (goal->getId() > maxId)
            {
                maxId = goal->getId();
            }
        }
        return maxId + 1;
    }
};
