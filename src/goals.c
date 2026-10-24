
#include <stdio.h>

#define MAX_GOALS 6

double goalTarget[MAX_GOALS];
double goalSaved[MAX_GOALS];
int goalCount = 0;

int createGoal(double target)
{
    if (goalCount == MAX_GOALS)
    {
        printf("The goals are full\n");
        return -1;
    }

    if (target <= 0)
    {
        printf("Target is invalid\n");
        return -1;
    }

    goalTarget[goalCount] = target;
    goalSaved[goalCount] = 0;

    goalCount++;

    printf("Goal %d created successfully\n", goalCount);

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

    goalSaved[index] = goalSaved[index] + amount;

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

    percent = goalSaved[index] / goalTarget[index] * 100;

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

        printf("\nGoal %d\n", index + 1);

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
               goalSaved[index], goalTarget[index]);

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
        goalTarget[i] = goalTarget[i + 1];
        goalSaved[i] = goalSaved[i + 1];
    }

    goalCount--;

    printf("Goal removed\n");
}
