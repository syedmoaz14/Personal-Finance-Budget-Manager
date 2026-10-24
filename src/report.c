#include <stdio.h>

double income[12] = {0};
double expense[12] = {0};
double food[12] = {0};
double transport[12] = {0};
double rent[12] = {0};
double other[12] = {0};

double monthIncome(int month)
{
    if (month < 0 || month >= 12)
    {
        printf("Invalid month");
        return -1;
    }

    return income[month];
}

double monthExpense(int month)
{
    if (month < 0 || month >= 12)
    {
        printf("Invalid Month");
        return -1;
    }

    return expense[month];
}

void yearSummary(void)
{
    int index;
    double totalIncome = 0;
    double totalExpense = 0;
    double totalFood = 0;
    double totalTransport = 0;
    double totalRent = 0;
    double totalOther = 0;

    for (index = 0; index < 12; index++)
    {
        totalIncome += income[index];
        totalExpense += expense[index];
        totalFood += food[index];
        totalTransport += transport[index];
        totalRent += rent[index];
        totalOther += other[index];
    }

    printf("Year Summary: \n");

    printf("Total Income: %.2f \n", totalIncome);
    printf("Total Expenses: %.2f \n", totalExpense);

    printf("Food Expense: %.2f \n", totalFood);
    printf("Transport Expense: %.2f \n", totalTransport);
    printf("Rent Expense: %.2f \n", totalRent);
    printf("Other Expense: %.2f \n", totalOther);
}

void printMonthlyReport(int month)
{
    int index;
    double amount[4];

    if (month < 0 || month >= 12)
    {
        printf("Invalid month\n");
        return;
    }

    amount[0] = food[month];
    amount[1] = transport[month];
    amount[2] = rent[month];
    amount[3] = other[month];

    printf("Monthly Report: \n");

    printf("Month Number: %d\n", month + 1);
    printf("Income: %.2f\n", monthIncome(month));
    printf("Expenses: %.2f\n", monthExpense(month));
    printf("Difference: %.2f\n",
           monthIncome(month) - monthExpense(month));

    printf("Category Spending: \n");
    printf("1. Food: %.2f\n", food[month]);
    printf("2. Transport: %.2f\n", transport[month]);
    printf("3. Rent: %.2f\n", rent[month]);
    printf("4. Other: %.2f\n", other[month]);

    printf("Top Spending Categories: \n");

    for (index = 0; index < 3; index++)
    {
        int j, highestIndex = -1;
        double highest = -1;

        for (j = 0; j < 4; j++)
        {
            if (amount[j] > highest)
            {
                highest = amount[j];
                highestIndex = j;
            }
        }

        if (highestIndex == 0)
        {
            printf("Food: %.2f\n", highest);
        }
        else if (highestIndex == 1)
        {
            printf("Transport: %.2f\n", highest);
        }
        else if (highestIndex == 2)
        {
            printf("Rent: %.2f\n", highest);
        }
        else if (highestIndex == 3)
        {
            printf("Other: %.2f\n", highest);
        }

        amount[highestIndex] = -1;
    }

    printf("Previous Month Comparison: \n");

    if (month == 0)
    {
        printf("No previous month available. \n");
    }
    else
    {
        printf("Income Difference: %.2f \n",
               income[month] - income[month - 1]);

        printf("Expenses Difference: %.2f \n",
               expense[month] - expense[month - 1]);
    }
}