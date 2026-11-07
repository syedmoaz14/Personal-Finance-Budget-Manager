#include <stdio.h>

/*
 * report.c (Varaa)
 *
 * The report does not keep any numbers of its own. It asks the other files:
 *   grid.c     (Moaz)  getSpent, categoryYearTotal
 *   category.c (Moaz)  categoryName
 *   account.c  (Moaz)  monthIncome, monthExpense
 * Month is 1-12 everywhere. There are 5 categories, numbered 0-4.
 *
 * These lines announce those functions until common.h exists;
 * then replace them with: #include "common.h"
 */
double getSpent(int month, int category);
double categoryYearTotal(int category);
const char *categoryName(int category);
double monthIncome(int month);
double monthExpense(int month);

void yearSummary(void)
{
    int month, category;
    double totalIncome = 0;
    double totalExpense = 0;

    for (month = 1; month <= 12; month++)
    {
        totalIncome += monthIncome(month);
        totalExpense += monthExpense(month);
    }

    printf("Year Summary: \n");

    printf("Total Income: %.2f \n", totalIncome);
    printf("Total Expenses: %.2f \n", totalExpense);

    for (category = 0; category < 5; category++)
    {
        printf("%s Expense: %.2f \n",
               categoryName(category), categoryYearTotal(category));
    }
}

void printMonthlyReport(int month)
{
    int index, category;
    double amount[5];

    if (month < 1 || month > 12)
    {
        printf("Invalid month\n");
        return;
    }

    for (category = 0; category < 5; category++)
    {
        amount[category] = getSpent(month, category);
    }

    printf("Monthly Report: \n");

    printf("Month Number: %d\n", month);
    printf("Income: %.2f\n", monthIncome(month));
    printf("Expenses: %.2f\n", monthExpense(month));
    printf("Difference: %.2f\n",
           monthIncome(month) - monthExpense(month));

    printf("Category Spending: \n");
    for (category = 0; category < 5; category++)
    {
        printf("%d. %s: %.2f\n",
               category + 1, categoryName(category), amount[category]);
    }

    printf("Top Spending Categories: \n");

    for (index = 0; index < 3; index++)
    {
        int j, highestIndex = -1;
        double highest = 0;

        for (j = 0; j < 5; j++)
        {
            if (amount[j] > highest)
            {
                highest = amount[j];
                highestIndex = j;
            }
        }

        if (highestIndex == -1)
        {
            if (index == 0)
            {
                printf("No spending recorded.\n");
            }
            break;
        }

        printf("%s: %.2f\n", categoryName(highestIndex), highest);

        amount[highestIndex] = -1;
    }

    printf("Previous Month Comparison: \n");

    if (month == 1)
    {
        printf("No previous month available. \n");
    }
    else
    {
        printf("Income Difference: %.2f \n",
               monthIncome(month) - monthIncome(month - 1));

        printf("Expenses Difference: %.2f \n",
               monthExpense(month) - monthExpense(month - 1));
    }
}
