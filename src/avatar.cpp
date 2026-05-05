#include "avatar.h"
#include <iostream>
#include <string>

using namespace std;

//new avatar default constructor
Avatar::Avatar(){
    level = 1; 
    xp = 0;
    xpToNextLevel = 100;
}

//load saved avatar constructor 
Avatar::Avatar(int lvl, int points){
    level = lvl; 
    xp = points;
    xpToNextLevel = 100;
}

//getters & setters
int Avatar::getLevel() const{
    return level;
}
void Avatar::setLevel(int lvl){
    level = lvl;
}
int Avatar::getXP() const{
    return xp;
}
void Avatar::setXP(int points) {
    xp = points;
}
int Avatar::getXpToNextLevel() const{
    return xpToNextLevel;
}

//add xp after task complete
void Avatar::gainXP(int amount) {
    if (amount <= 0) 
        return;
 
    xp += amount;
    cout << "Gained " << amount << " XP" << endl;
 
    while (xp >= xpToNextLevel) {
        levelUp();
    }
}
//when xpToNextLevel is reached, level up & adjust xp
void Avatar::levelUp() {
    xp -= xpToNextLevel;         
    level+= 1;
 
    std::cout << "LEVEL UP! You are now Level " << level << "\n";
}
string Avatar::serialize() const{
    return to_string(level) + "," + to_string(xp);
}
void Avatar::progressBar() const{
    int total = 10;
    int filled = (xp * total) / xpToNextLevel;
    cout << "[";
    
    if (filled > total)
        filled = total;
    
    for (int i = 0; i < total; i++){
        if (i < filled)
            cout << "●";
        else
            cout << "○";
    }
    cout << "] " << xp << "/" << xpToNextLevel << endl;
    
}
