#include <iostream>
#include <string>
#include <memory>

// Base Class (Goal)
// ==========================================
class Goal
{
protected: // Accessible by child classes
    int id;
    std::string title; // std::string handles memory automatically
    bool isCompleted;

public:
    // Constructor
    Goal(int id, std::string title)
        : id(id), title(title), isCompleted(false) {}

    // Virtual destructor is vital when using inheritance
    virtual ~Goal() {}

    // Method to change title
    void changeTitle(const std::string &newTitle)
    {
        title = newTitle;
    }

    // Abstract methods (Pure Virtual Functions)
    virtual void markComplete() = 0;
    virtual void getReward() = 0;

    // Getters for main
    int getId() const { return id; }
    std::string getTitle() const { return title; }
    bool getStatus() const { return isCompleted; }
};

// DailyGoal Class (Inherits from Goal)
// ==========================================
class DailyGoal : public Goal
{
public:
    // Constructor calls the base class constructor
    DailyGoal(int id, std::string title) : Goal(id, title) {}

    // Implement abstract methods
    void markComplete() override
    {
        isCompleted = true;
        std::cout << "Daily Goal '" << title << "' completed!" << std::endl;
    }

    void getReward() override
    {
        std::cout << "Reward granted for Daily Goal." << std::endl;
    }
};

int main()
{
    // Use a pointer to demonstrate polymorphism
    Goal *myGoal = new DailyGoal(1, "Complete C++ conversion");

    // Test the goal
    std::cout << "Goal ID: " << myGoal->getId() << std::endl;
    std::cout << "Goal Title: " << myGoal->getTitle() << std::endl;
    std::cout << "Is Completed: " << (myGoal->getStatus() ? "Yes" : "No") << std::endl;

    // Mark the goal as complete (Dynamic Dispatch)
    myGoal->changeTitle("Complete C++ conversion - Updated Title");
    std::cout << "Goal Title: " << myGoal->getTitle() << std::endl;

    myGoal->markComplete();

    // Check the status again
    std::cout << "Is Completed: " << (myGoal->getStatus() ? "Yes" : "No") << std::endl;

    // Get the reward
    myGoal->getReward();

    // Clean up memory
    delete myGoal;

    return 0;
}