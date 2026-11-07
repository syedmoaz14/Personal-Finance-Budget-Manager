/* goals.c (Varaa): at most 5 goals, numbered from 0 */
int createGoal(double target);
void addToGoal(int index, double amount);
double goalPercent(int index);
void showGoals(void);
void removeGoal(int index);
