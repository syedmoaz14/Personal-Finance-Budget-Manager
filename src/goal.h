#ifndef GOALS_H
#define GOALS_H

#define MAX_GOALS 5

int createGoal(double target);
void addToGoal(int index, double amount);
double goalPercent(int index);
void showGoals(void);
void removeGoal(int index);

#endif