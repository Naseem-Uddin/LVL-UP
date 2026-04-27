#pragma once
#include <string>
using namespace std;
 
class Avatar {
private:
    int level;
    int xp;
    int xpToNextLevel;
 
    void levelUp();
    int calculateXPThreshold(int level) const;
 
public:
    // Constructors
    Avatar();                        // new avatar
    Avatar(int level, int xp);       // load from file
 
    // XP & leveling
    void gainXP(int amount);
 
    // Dashboard / report helpers
    double getProgressPercent() const;
    void   printProgressBar()   const;
    string getSummary()    const;
 
    // Serialization
    std::string serialize()              const;
    static Avatar deserialize(const std::string& data);
 
    // Getters
    int getLevel()         const { return level; }
    int getXP()            const { return xp; }
    int getXpToNextLevel() const { return xpToNextLevel; }
};