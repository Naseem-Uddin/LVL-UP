#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Goal Goal;
struct Goal
{
    int id;
    char *title;
    bool isCompleted;

    // Virtual function pointers for abstract methods
    void (*markComplete)(Goal *self);
    void (*getReward)(Goal *self);
};
void changeTitle(Goal *self, const char *newTitle)
{
    if (self)
    {
        free(self->title);              // Free old title
        self->title = strdup(newTitle); // Set new title
    }
}

// DailyGoal Class (Inherits from Goal)
// ==========================================
typedef struct
{
    Goal base; // Inheritance via struct composition
} DailyGoal;

// Implementations for DailyGoal
void DailyGoal_markComplete(Goal *self)
{
    if (self)
        self->isCompleted = true;
    printf("Daily Goal '%s' completed!\n", self->title);
}

void DailyGoal_getReward(Goal *self)
{
    printf("Reward granted for Daily Goal.\n");
}

// DailyGoal Constructor
DailyGoal *DailyGoal_create(int id, const char *title)
{
    DailyGoal *dg = (DailyGoal *)malloc(sizeof(DailyGoal));
    dg->base.id = id;
    dg->base.title = strdup(title);
    dg->base.isCompleted = false;

    // Bind virtual methods
    dg->base.markComplete = DailyGoal_markComplete;
    dg->base.getReward = DailyGoal_getReward;
    return dg;
}
int main()
{
    DailyGoal *myGoal = DailyGoal_create(1, "Complete C project");

    // Test the goal
    printf("Goal ID: %d\n", myGoal->base.id);
    printf("Goal Title: %s\n", myGoal->base.title);
    printf("Is Completed: %s\n", myGoal->base.isCompleted ? "Yes" : "No");

    // Mark the goal as complete
    myGoal->base.markComplete((Goal *)myGoal);

    // Check the status again
    printf("Is Completed: %s\n", myGoal->base.isCompleted ? "Yes" : "No");

    // Get the reward
    myGoal->base.getReward((Goal *)myGoal);

    // Clean up
    free(myGoal->base.title);
    free(myGoal);

    return 0;
}