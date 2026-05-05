#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>
#include "../src/user.h"
#include "../src/goals.h"

using namespace std;
namespace fs = std::filesystem;

struct LoginResult
{
    bool success;
    User user;
};

bool emailExists(const string &targetEmail, const string &filename)
{
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string fName, lName, email, password;

        getline(ss, fName, ',');
        getline(ss, lName, ',');
        getline(ss, email, ',');
        getline(ss, password);

        if (email == targetEmail)
        {
            return true;
        }
    }

    return false;
}

bool validateLogin(const string &targetEmail, const string &targetPassword, const string &filename)
{
    ifstream file(filename);
    string line;

    while (getline(file, line))
    {
        stringstream ss(line);
        string fName, lName, email, password;

        getline(ss, fName, ',');
        getline(ss, lName, ',');
        getline(ss, email, ',');
        getline(ss, password);

        if (email == targetEmail)
        {
            if (password == targetPassword)
            {
                return true;
            }

            else
            {
                cout << "\nERROR: Incorrect password.\n";
                return false;
            }
        }
    }

    cout << "\nERROR: Email does not exist.\n";
    return false;
}

void saveUserData(const User &user, const string &filename)
{
    ofstream file(filename, ios::app);
    if (!file.is_open())
    {
        cerr << "\nERROR: Error opening file.\n";
        return;
    }

    file << user.getFName() << ","
         << user.getLName() << ","
         << user.getEmail() << ","
         << user.getPassword() << ","
         << user.getAvatar().getLevel() << ","
         << user.getAvatar().getXP() << "\n";

    file.close();
}

void updateUserData(const User &user, const string &filename)
{
    vector<string> lines;
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string fName, lName, email, password;
        getline(ss, fName, ',');
        getline(ss, lName, ',');
        getline(ss, email, ',');
        getline(ss, password);
        if (email == user.getEmail())
        {
            stringstream newLine;
            newLine << user.getFName() << "," << user.getLName() << "," << user.getEmail() << "," << user.getPassword() << "," << user.getAvatar().getLevel() << "," << user.getAvatar().getXP();
            lines.push_back(newLine.str());
        }

        else
        {
            lines.push_back(line);
        }
    }
    file.close();
    ofstream outFile(filename);
    for (const auto &l : lines)
    {
        outFile << l << "\n";
    }
    outFile.close();
}

void signup(const string &filename)
{
    while (true)
    {
        string fName, lName, email, password;
        cout << "\n--- SIGN UP ---\n";
        cout << "(Type 'q' at any prompt to return to Main Menu)\n";

        cout << "First Name: ";
        getline(cin, fName);
        if (fName == "q")
            return;

        cout << "Last Name: ";
        getline(cin, lName);
        if (lName == "q")
            return;

        cout << "Email: ";
        getline(cin, email);
        if (email == "q")
            return;

        if (emailExists(email, filename))
        {
            cout << "\nERROR: An account with that email already exists.\n";
            continue;
        }

        cout << "Password: ";
        getline(cin, password);
        if (password == "q")
            return;

        User user(fName, lName, email, password, 1, 0);
        saveUserData(user, filename);
        cout << "\nSignup successful!\n";
        return;
    }
}

LoginResult login(const string &filename)
{
    while (true)
    {
        string email, password;
        cout << "\n--- LOGIN ---\n";
        cout << "(Type 'q' at any prompt to return to Main Menu)\n";

        cout << "Email: ";
        getline(cin, email);
        if (email == "q")
            return {false, User()};

        cout << "Password: ";
        getline(cin, password);
        if (password == "q")
            return {false, User()};

        ifstream file(filename);
        string line;

        while (getline(file, line))
        {
            stringstream ss(line);
            string fName, lName, e, p, lvlStr, xpStr;
            getline(ss, fName, ',');
            getline(ss, lName, ',');
            getline(ss, e, ',');
            getline(ss, p, ',');
            getline(ss, lvlStr, ',');
            getline(ss, xpStr);

            if (e == email)
            {
                if (p == password)
                {
                    int lvl = stoi(lvlStr);
                    int xp = stoi(xpStr);
                    User user(fName, lName, e, p, lvl, xp);

                    return {true, user};
                }

                else
                {
                    cout << "\nERROR: Incorrect password.\n";
                    return {false, User()};
                }
            }
        }

        cout << "\nERROR: Email does not exist.\n";
    }
}

int main()
{
    string filename = "dataFile.txt";

    if (!fs::exists(filename))
    {
        ofstream createFile(filename);

        if (!createFile.is_open())
        {
            cerr << "\nERROR: Failed to create data file.\n";
            return 1;
        }

        createFile.close();
    }

    User currentUser;
    bool loggedIn = false;

    while (true)
    {
        string choice;
        cout << "\n===== MENU =====\n";
        cout << "(Type 'q' to quit)\n";
        cout << "1. Login\n";
        cout << "2. Sign Up\n\n";
        cout << "Choose option: ";

        getline(cin, choice);

        if (choice == "1")
        {
            LoginResult result = login(filename);
            if (result.success)
            {
                currentUser = result.user;
                cout << "\nLogin successful. Welcome " << currentUser.getFName() << "!\n";
                cout << "Your Avatar - Level: " << currentUser.getAvatar().getLevel()
                     << ", XP: " << currentUser.getAvatar().getXP() << "\n";
                loggedIn = true;
                break; // break out of loop for the rest of the program to run after login
            }
        }

        else if (choice == "2")
        {
            signup(filename);
        }

        else if (choice == "q")
        {
            cout << "\nExiting program. Goodbye!\n";
            break;
        }

        else
        {
            cout << "\nERROR: Invalid option. Please pick a valid option.\n";
        }
    }

    if (!loggedIn)
    {
        return 0;
    }

    // need to implement the rest of the program logic here
    int goalId = 1;
    while (true)
    {
        string choice;
        cout << "\n--- USER MENU ---\n";
        cout << "(Type 'q' to quit)\n";
        cout << "1. Create Daily Goal\n";
        cout << "2. View Avatar\n";
        cout << "3. View My Goals\n";
        cout << "4. Remove a Goal\n";
        cout << "5. Mark Goal Complete\n";
        cout << "q. Quit\n\n";
        cout << "Choose option: ";

        getline(cin, choice);

        if (choice == "1")
        {
            cout << "Goal title: ";
            string title;
            getline(cin, title);

            // 1. Create the goal
            Goal *g = new DailyGoal(goalId++, title);

            // 2. Add it to the user's vector!
            currentUser.addGoal(g);

            cout << "Created goal: '" << g->getTitle() << "' and added to your list!\n";

            // Notice: No "delete g;" here!
            // The User class is now responsible for deleting it in removeGoal().
        }

        else if (choice == "2")
        {
            cout << "Avatar - Level: " << currentUser.getAvatar().getLevel()
                 << ", XP: " << currentUser.getAvatar().getXP() << "\n";
        }
        else if (choice == "3")
        {
            cout << "\n--- " << currentUser.getFName() << "'s Goals ---\n";
            currentUser.displayGoals();
        }
        else if (choice == "4")
        {
            cout << "Enter Goal ID to remove: ";
            string idStr;
            getline(cin, idStr);
            int id = stoi(idStr);
            currentUser.removeGoal(id);
        }
        else if (choice == "5")
        {
            cout << "Enter Goal ID to mark complete: ";
            string idStr;
            getline(cin, idStr);
            int id = stoi(idStr);
            for (Goal *g : currentUser.getGoals())
            {
                if (g->getId() == id)
                {
                    g->markComplete();
                    g->getReward();
                    currentUser.getAvatar().gainXP(10);
                    break;
                }
            }
        }

        else if (choice == "q")
        {
            updateUserData(currentUser, filename);
            cout << "\nGoodbye!\n";
            break;
        }

        else
        {
            cout << "\nInvalid option.\n";
        }
    }
}