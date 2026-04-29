#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include "../src/user.h"

using namespace std;
namespace fs = std::filesystem;

bool emailExists(const string& targetEmail, const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string fName, lName, email, password;

        getline(ss, fName, ',');
        getline(ss, lName, ',');
        getline(ss, email, ',');
        getline(ss, password);

        if (email == targetEmail) {
            return true;
        }
    }

    return false;
}

bool validateLogin(const string& targetEmail, const string& targetPassword, const string& filename) {
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string fName, lName, email, password;

        getline(ss, fName, ',');
        getline(ss, lName, ',');
        getline(ss, email, ',');
        getline(ss, password);

        if (email == targetEmail) {
            if (password == targetPassword) {
                return true;
            } else {
                cout << "\nERROR: Incorrect password.\n";
                return false;
            }
        }
    }

    cout << "\nERROR: Email does not exist.\n";
    return false;
}

void saveUserData(const User& user, const string& filename) {
    ofstream file(filename, ios::app);
    if (!file.is_open()) {
        cerr << "\nERROR: Error opening file.\n";
        return;
    }

    file << user.fName << ","
         << user.lName << ","
         << user.email << ","
         << user.password << "\n";

    file.close();
}

void signup(const string& filename) {
    while (true) {
        User user;
        cout << "\n--- SIGN UP ---\n";
        cout << "(Type 'q' at any prompt to return to Main Menu)\n";
        
        cout << "First Name: ";
        getline(cin, user.fName);
        if (user.fName == "q") return;
        
        cout << "Last Name: ";
        getline(cin, user.lName);
        if (user.lName == "q") return;
        
        cout << "Email: ";
        getline(cin, user.email);
        if (user.email == "q") return;
        
        if (emailExists(user.email, filename)) {
            cout << "\nERROR: An account with that email already exists.\n";
            continue;
        }
        
        cout << "Password: ";
        getline(cin, user.password);

        if (user.password == "q") return;
        
        saveUserData(user, filename);
        cout << "\nSignup successful!\n";
        return;
    }
}

bool login(const string& filename) {
    while (true) {
        string email, password;
        cout << "\n--- LOGIN ---\n";
        cout << "(Type 'q' at any prompt to return to Main Menu)\n";
        
        cout << "Email: ";
        getline(cin, email);

        if (email == "q") return false;
        
        cout << "Password: ";
        getline(cin, password);

        if (password == "q") return false;
        
        if (validateLogin(email, password, filename)) {
            return true;
        }
    }
}

int main() {
    string filename = "dataFile.txt";

    if (!fs::exists(filename)) {
        ofstream createFile(filename);

        if (!createFile.is_open()) {
            cerr << "\nERROR: Failed to create data file.\n";
            return 1;
        }

        createFile.close();
    }

    while (true) {
        string choice;
        cout << "\n===== MENU =====\n";
        cout << "(Type 'q' to quit)\n";
        cout << "1. Login\n";
        cout << "2. Sign Up\n\n";
        cout << "Choose option: ";
        
        getline(cin, choice); 
        
        if (choice == "1") {
            if (login(filename)) {
                cout << "\nLogin successful. Welcome!\n";
                break; //break out of loop for the rest of the program to run after login
            }
        }

        else if (choice == "2") {
            signup(filename);
        }

        else if (choice == "q") {
            cout << "\nExiting program. Goodbye!\n";
            break;
        }

        else {
            cout << "\nERROR: Invalid option. Please pick a valid option.\n";
        }
    }

    //need to implement the rest of the program logic here
    
    return 0;
}