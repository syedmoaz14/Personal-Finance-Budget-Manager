#include <stdio.h>
void clearGrid(void);
void addToGrid(int month, int category, double amount);
double getSpent(int month, int category);
double monthTotal(int month);
double categoryYearTotal(int category);

int main()
{
    clearGrid();
    addToGrid(1, 0, 30);    /* Jan, Food */
    addToGrid(1, 2, 90);    /* Jan, Rent */
    addToGrid(2, 0, 10);    /* Feb, Food */

    printf("%.2f\n", getSpent(1, 0));          /* 30.00 */
    printf("%.2f\n", monthTotal(1));           /* 120.00: 30 + 90 */
    printf("%.2f\n", categoryYearTotal(0));    /* 40.00: 30 + 10 */
    printf("%.2f\n", monthTotal(13));          /* 0.00: bad month */
    return 0;
}
