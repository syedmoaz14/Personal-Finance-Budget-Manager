/*
 * budget.c - monthly limits per category and the alerts.
 * It never stores spending itself; it asks grid.c with getSpent().
 */

#include <stdio.h>
#include "common.h"

double budgetLimit[5];      /* one limit per category, 0 means no limit */

void setLimit(int category, double amount)
{
    if (category < 0 || category > 4 || amount < 0)
    {
        printf("Invalid category or limit.\n");
        return;
    }

    budgetLimit[category] = amount;
}

double getLimit(int category)
{
    if (category < 0 || category > 4)
    {
        return 0;
    }

    return budgetLimit[category];
}

/* percent of the limit used, or -1 if there is no limit */
double percentUsed(int month, int category)
{
    if (category < 0 || category > 4 || budgetLimit[category] <= 0)
    {
        return -1;
    }

    return getSpent(month, category) * 100 / budgetLimit[category];
}

/* prints a bar of 10 cells, for example [#####-----] */
void printBar(double percent)
{
    int i;
    int filled = (int)(percent / 10);

    if (filled > 10)
    {
        filled = 10;
    }

    printf("[");
    for (i = 0; i < 10; i++)
    {
        if (i < filled)
        {
            printf("#");
        }
        else
        {
            printf("-");
        }
    }
    printf("]");
}

/* called after every expense: warns from 80%, alerts above 100% */
void checkBudget(int month, int category)
{
    double percent = percentUsed(month, category);

    if (percent < 0)
    {
        return;
    }

    if (percent > 100)
    {
        printf("[!] OVERSPENT: %s is at %.0f%% of its budget.\n",
               categoryName(category), percent);
    }
    else if (percent >= 80)
    {
        printf("[!] WARNING: %s is at %.0f%% of its budget.\n",
               categoryName(category), percent);
    }
}

/* one line per category for the given month */
void showStatus(int month)
{
    int c;
    double percent;

    printf("\nBudget status for month %d\n", month);
    printf("Category      Spent     Limit   Used\n");

    for (c = 0; c < 5; c++)
    {
        printf("%-10s %9.2f %9.2f   ", categoryName(c),
               getSpent(month, c), budgetLimit[c]);

        percent = percentUsed(month, c);
        if (percent < 0)
        {
            printf("(no limit set)\n");
        }
        else
        {
            printBar(percent);
            printf(" %.0f%%\n", percent);
        }
    }
}

/* the whole year: months down, categories across, with totals */
void showMonthTable(void)
{
    int m, c;

    printf("\nMonth");
    for (c = 0; c < 5; c++)
    {
        printf("%10s", categoryName(c));
    }
    printf("%10s\n", "Total");

    for (m = 1; m <= 12; m++)
    {
        printf("%5d", m);
        for (c = 0; c < 5; c++)
        {
            printf("%10.2f", getSpent(m, c));
        }
        printf("%10.2f\n", monthTotal(m));
    }

    printf("Total");
    for (c = 0; c < 5; c++)
    {
        printf("%10.2f", categoryYearTotal(c));
    }
    printf("\n");
}
