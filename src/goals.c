/*
 * goals.c - savings goals such as "Laptop - 150000".
 *
 * Each goal is a struct Goal. The list starts small and doubles with
 * realloc when it is full, and is freed when the program exits.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

struct Goal
{
    char name[30];
    double target;
    double saved;
};

struct Goal *goals = NULL;
int goalCount = 0;
int goalCapacity = 0;

/* makes room for one more goal; returns 1 if it worked */
int makeRoom(void)
{
    struct Goal *bigger;
    int newCapacity;

    if (goalCount < goalCapacity)
    {
        return 1;
    }

    newCapacity = (goalCapacity == 0) ? 4 : goalCapacity * 2;
    bigger = realloc(goals, newCapacity * sizeof(struct Goal));
    if (bigger == NULL)
    {
        printf("Out of memory.\n");
        return 0;
    }

    goals = bigger;
    goalCapacity = newCapacity;
    return 1;
}

/* adds a goal with money already saved (used by createGoal and by the loader) */
int loadGoal(const char *name, double target, double saved)
{
    if (makeRoom() == 0)
    {
        return -1;
    }

    strncpy(goals[goalCount].name, name, 29);
    goals[goalCount].name[29] = '\0';
    goals[goalCount].target = target;
    goals[goalCount].saved = saved;
    goalCount++;

    return goalCount - 1;
}

int createGoal(const char *name, double target)
{
    if (target <= 0)
    {
        printf("Target is invalid\n");
        return -1;
    }
    if (name[0] == '\0')
    {
        printf("A goal needs a name.\n");
        return -1;
    }

    if (loadGoal(name, target, 0) < 0)
    {
        return -1;
    }

    printf("Goal \"%s\" created successfully\n", name);
    return goalCount - 1;
}

void addToGoal(int index, double amount)
{
    if (index < 0 || index >= goalCount)
    {
        printf("Invalid goal number\n");
        return;
    }

    if (amount <= 0)
    {
        printf("Invalid amount\n");
        return;
    }

    goals[index].saved = goals[index].saved + amount;

    printf("Amount added\n");
}

double goalPercent(int index)
{
    double percent;

    if (index < 0 || index >= goalCount)
    {
        printf("Invalid index\n");
        return -1;
    }

    percent = goals[index].saved / goals[index].target * 100;

    if (percent > 100)
    {
        percent = 100;
    }

    return percent;
}

void showGoals(void)
{
    int index, count;
    int filled;
    double percent;

    if (goalCount == 0)
    {
        printf("No goals available\n");
        return;
    }

    printf("Goals Saved:\n");

    for (index = 0; index < goalCount; index++)
    {
        percent = goalPercent(index);
        filled = (int)(percent / 5);

        printf("\n%d. %s\n", index + 1, goals[index].name);

        printf("[");

        for (count = 0; count < 20; count++)
        {
            if (count < filled)
            {
                printf("#");
            }
            else
            {
                printf("-");
            }
        }

        printf("] %.0f%%\n", percent);

        printf("Saved: %.2f, Target: %.2f\n",
               goals[index].saved, goals[index].target);

        if (percent >= 100)
        {
            printf("Status: Complete\n");
        }
    }
}

void removeGoal(int index)
{
    int i;

    if (index < 0 || index >= goalCount)
    {
        printf("Invalid goal number\n");
        return;
    }

    for (i = index; i < goalCount - 1; i++)
    {
        goals[i] = goals[i + 1];
    }

    goalCount--;

    printf("Goal removed\n");
}

int getGoalCount(void)
{
    return goalCount;
}

/* used by storage.c to save the goals */
const char *getGoalName(int index)
{
    return goals[index].name;
}

double getGoalTarget(int index)
{
    return goals[index].target;
}

double getGoalSaved(int index)
{
    return goals[index].saved;
}

void freeGoals(void)
{
    free(goals);
    goals = NULL;
    goalCount = 0;
    goalCapacity = 0;
}
