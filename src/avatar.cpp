#include "avatar.h"
#include <iostream>
#include <string>
 
// Constructors

Avatar::Avatar(){
    level = 1; 
    xp = 0;
    xpToNextLevel = calculateXPThreshold(level); 
}

Avatar::Avatar(int lvl, int experience){
    level = lvl;
    xp = experience;
    xpToNextLevel=calculateXPThreshold(level);
}
 
// Private helpers

int Avatar::calculateXPThreshold(int lvl) const {
    return 100 * lvl;  // Level 1→100 XP, Level 2→200 XP, etc.
}
 
void Avatar::levelUp() {
    xp -= xpToNextLevel;         
    level+= 1;
    xpToNextLevel = calculateXPThreshold(level);
 
    std::cout << "  *** LEVEL UP! You are now Level " << level << " ***\n";
    std::cout << "  Next level requires " << xpToNextLevel << " XP.\n";
}
 

void Avatar::gainXP(int amount) {
    if (amount <= 0) 
        return;
 
    xp += amount;
    cout << "Gained " << amount << " XP \n";
 
    while (xp >= xpToNextLevel) {
        levelUp();
    }
}
 
double Avatar::getProgressPercent() const {
    return (double)xp / xpToNextLevel * 100.0;
}
 
void Avatar::printProgressBar() const {
    const int barLength = 20;
    int filled = static_cast<int>(getProgressPercent() / 100.0 * barLength);
    int empty  = barLength - filled;
 
    std::string bar = "[";
    for (int i = 0; i < filled; i++)    
        bar += "=";
    if (filled < barLength)             
        bar += ">";
    for (int i = 0; i < empty - 1; i++) 
        bar += " ";
    bar += "]";
 
    printf("  Level %-3d | %s %d%% (%d/%d XP)\n",
           level, bar.c_str(),
           static_cast<int>(getProgressPercent()), xp, xpToNextLevel);
}
 
string Avatar::getSummary() const {
    return "Level" + to_string(level) + " | " + 
        to_string(xp) + "/" + to_string(xpToNextLevel) + " XP";
}
 
string Avatar::serialize() const {
    return to_string(level) + "," + to_string(xp);
}
 
Avatar Avatar::deserialize(const std::string& data) {
    int comma = data.find(',');
    int lvl = stoi(data.substr(0, comma));
    int experience = stoi(data.substr(comma + 1));

    return Avatar(lvl, experience);
}