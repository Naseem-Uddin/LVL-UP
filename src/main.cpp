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

string sanitizeGoalText(const string &text)
{
    string result;
    result.reserve(text.size());
    for (char c : text)
    {
        if (c == '|' || c == ';')
        {
            result += ' ';
        }
        else
        {
            result += c;
        }
    }
    return result;
}

string serializeGoalList(const User &user)
{
    string serialized;
    bool first = true;
    for (Goal *goal : user.getGoals())
    {
        if (!first)
            serialized += ";";
        first = false;

        serialized += to_string(goal->getId());
        serialized += "|";
        serialized += goal->getStatus() ? "1" : "0";
        serialized += "|";
        serialized += sanitizeGoalText(goal->getTitle());
    }
    return serialized;
}

void loadGoalsFromString(User &user, const string &goalsData)
{
    if (goalsData.empty())
        return;

    size_t position = 0;
    while (position < goalsData.size())
    {
        size_t next = goalsData.find(';', position);
        string record = goalsData.substr(position, next - position);
        if (!record.empty())
        {
            size_t firstBar = record.find('|');
            size_t secondBar = record.find('|', firstBar == string::npos ? string::npos : firstBar + 1);
            if (firstBar != string::npos && secondBar != string::npos)
            {
                int id = stoi(record.substr(0, firstBar));
                bool completed = (record.substr(firstBar + 1, secondBar - firstBar - 1) == "1");
                string title = record.substr(secondBar + 1);
                DailyGoal *goal = new DailyGoal(id, title);
                goal->setStatus(completed);
                user.addGoal(goal);
            }
        }
        if (next == string::npos)
            break;
        position = next + 1;
    }
}

void writeUserLine(const User &user, ostream &out)
{
    out << user.getFName() << ","
        << user.getLName() << ","
        << user.getEmail() << ","
        << user.getPassword() << ","
        << user.getAvatar().getLevel() << ","
        << user.getAvatar().getXP();

    string goalsData = serializeGoalList(user);
    if (!goalsData.empty())
    {
        out << "," << goalsData;
    }
}

bool updateUserData(const User &user, const string &filename);

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

    writeUserLine(user, file);
    file << "\n";
    file.flush();
    file.close();
}

string getEmailFromLine(const string &line)
{
    stringstream ss(line);
    string fName, lName, email;
    getline(ss, fName, ',');
    getline(ss, lName, ',');
    getline(ss, email, ',');
    return email;
}

bool updateUserData(const User &user, const string &filename)
{
    vector<string> lines;
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "\nERROR: Unable to open data file for update.\n";
        return false;
    }

    string line;
    bool found = false;
    while (getline(file, line))
    {
        if (line.empty())
            continue;

        string existingEmail = getEmailFromLine(line);
        if (existingEmail == user.getEmail())
        {
            stringstream newLine;
            writeUserLine(user, newLine);
            lines.push_back(newLine.str());
            found = true;
        }
        else
        {
            lines.push_back(line);
        }
    }
    file.close();

    if (!found)
    {
        cerr << "\nWARNING: Could not find existing user record to update; appending a new line.\n";
        stringstream newLine;
        writeUserLine(user, newLine);
        lines.push_back(newLine.str());
    }

    string tempFilename = filename + ".tmp";
    ofstream outFile(tempFilename);
    if (!outFile.is_open())
    {
        cerr << "\nERROR: Unable to open temporary file for update.\n";
        return false;
    }

    for (const auto &l : lines)
    {
        outFile << l << "\n";
    }
    outFile.flush();
    outFile.close();

    fs::rename(tempFilename, filename);
    return true;
}

bool parseInt(const string &text, int &value)
{
    try
    {
        size_t idx;
        value = stoi(text, &idx);
        return idx == text.size();
    }
    catch (const exception &)
    {
        return false;
    }
}

void signup(const string &filename)
{
    while (true)
    {
        string fName, lName, email, password;
        cout << "\n--- SIGN UP ---\n";
        cout << "(Type 'q' at any prompt to return to Main Menu)\n\n";

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
        cout << "(Type 'q' at any prompt to return to Main Menu)\n\n";

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
            string fName, lName, e, p, lvlStr, xpStr, goalsData;
            getline(ss, fName, ',');
            getline(ss, lName, ',');
            getline(ss, e, ',');
            getline(ss, p, ',');
            getline(ss, lvlStr, ',');
            getline(ss, xpStr, ',');
            getline(ss, goalsData);

            if (e == email)
            {
                if (p == password)
                {
                    int lvl = stoi(lvlStr);
                    int xp = stoi(xpStr);
                    User user(fName, lName, e, p, lvl, xp);
                    loadGoalsFromString(user, goalsData);
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
        cout << "Pressing 'q' returns back; at the main menu it quits.\n\n";
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

    int goalId = currentUser.getNextGoalId();
    while (true)
    {
        string choice;
        cout << "\n--- USER MENU ---\n";
        cout << "(Type 'q' to quit)\n";
        cout << "Pressing 'q' returns to the previous menu; at the main menu it quits.\n\n";
        cout << "1. Create Daily Goal\n";
        cout << "2. View Avatar\n";
        cout << "3. View My Goals\n";
        cout << "4. Remove a Goal\n";
        cout << "5. Mark Goal Complete\n";

        cout << "\n\nChoose option: ";

        getline(cin, choice);

        if (choice == "1")
        {
            cout << "\nGoal title: ";
            string title;
            getline(cin, title);

            if (title == "q")
            {
                cout << "\nGoal creation cancelled.\n";
                continue;
            }

            if (title.empty())
            {
                cout << "\nERROR: Goal title cannot be empty.\n";
                continue;
            }

            // 1. Create the goal
            Goal *g = new DailyGoal(goalId++, title);

            // 2. Add it to the user's vector!
            currentUser.addGoal(g);
            updateUserData(currentUser, filename);

            cout << "\nCreated goal: '" << g->getTitle() << "' and added to your list!\n";
        }

        else if (choice == "2")
        {
            cout << "\nAvatar - Level: " << currentUser.getAvatar().getLevel()
                 << ", XP: " << currentUser.getAvatar().getXP() << "\n";
        }

        else if (choice == "3")
        {
            cout << "\n--- " << currentUser.getFName() << "'s Goals ---\n";
            currentUser.displayGoals();
        }
        else if (choice == "4")
        {
            cout << "\nEnter Goal ID to remove: ";
            string idStr;
            getline(cin, idStr);

            if (idStr == "q")
            {
                cout << "\nGoal removal cancelled.\n";
                continue;
            }

            int id;
            if (!parseInt(idStr, id))
            {
                cout << "\nERROR: Please enter a valid numeric Goal ID or 'q' to cancel.\n";
                continue;
            }

            currentUser.removeGoal(id);
            updateUserData(currentUser, filename);
        }
        
        else if (choice == "5")
        {
            cout << "\nEnter Goal ID to mark complete: ";
            string idStr;
            getline(cin, idStr);

            if (idStr == "q")
            {
                cout << "\nMark complete cancelled.\n";
                continue;
            }

            int id;
            if (!parseInt(idStr, id))
            {
                cout << "\nERROR: Please enter a valid numeric Goal ID or 'q' to cancel.\n";
                continue;
            }

            bool found = false;
            for (Goal *g : currentUser.getGoals())
            {
                if (g->getId() == id)
                {
                    g->markComplete();
                    g->getReward();
                    currentUser.getAvatar().gainXP(10);
                    currentUser.removeGoal(id);
                    updateUserData(currentUser, filename);
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                cout << "\nERROR: No goal found with ID " << id << ".\n";
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
