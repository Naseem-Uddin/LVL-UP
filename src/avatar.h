#pragma once
#include <string>

using namespace std;
 
class Avatar {
private:
    int level;
    int xp;
    int xpToNextLevel;
 
public:
    // Constructors
    Avatar();                        // new avatar
    Avatar(int level, int xp);       // load from file

    void gainXP(int amount);
    void levelUp();
 
    // Serialization
    string serialize() const;
 
    // Getters
    int getLevel() const;
    void setLevel(int lvl);
    int getXP() const;
    void setXP(int points);
    int getXpToNextLevel() const;
};