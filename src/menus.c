/*
 * menus.c - the screens. Each menu shows its options, reads a choice
 * and calls the function that does the work. It does no calculations.
 */

#include <stdio.h>
#include "common.h"

/* shows the accounts and asks which one; returns its number from 0 */
int askAccount(void)
{
    listAccounts();
    printf("Account number: ");
    return readInt(1, getAccountCount()) - 1;
}

void accountsMenu(void)
{
    int choice = -1;
    int kind, from, to;
    double amount;

    while (choice != 0)
    {
        printf("\n--- ACCOUNTS ---\n");
        printf("1. Add account\n2. List accounts\n3. Transfer money\n0. Back\n");
        printf("Enter choice: ");
        choice = readInt(0, 3);

        switch (choice)
        {
        case 1:
            printf("Kind: 1. Cash  2. Bank  3. Savings\n");
            printf("Enter kind: ");
            kind = readInt(1, 3) - 1;
            printf("Opening balance: ");
            amount = readDouble(0);
            if (addAccount(kind, amount) >= 0)
            {
                printf("Account added.\n");
            }
            break;
        case 2:
            listAccounts();
            break;
        case 3:
            if (getAccountCount() < 2)
            {
                printf("You need at least two accounts to transfer.\n");
                break;
            }
            printf("Transfer FROM:\n");
            from = askAccount();
            printf("Transfer TO:\n");
            to = askAccount();
            printf("Amount: ");
            amount = readDouble(0.01);
            if (transfer(from, to, amount) == 1)
            {
                printf("Transfer done.\n");
            }
            break;
        }
    }
}

void transactionsMenu(void)
{
    int choice = -1;
    int account, day, month, year, category, number;
    double amount;

    while (choice != 0)
    {
        printf("\n--- TRANSACTIONS ---\n");
        printf("1. Add income\n2. Add expense\n3. View history\n4. Delete a transaction\n0. Back\n");
        printf("Enter choice: ");
        choice = readInt(0, 4);

        switch (choice)
        {
        case 1:
            account = askAccount();
            printf("Income amount: ");
            amount = readDouble(0.01);
            readDate(&day, &month, &year);
            if (addTransaction(account, amount, 1, day, month, year, 4) == 1)
            {
                printf("Income added.\n");
            }
            break;
        case 2:
            account = askAccount();
            printf("Expense amount: ");
            amount = readDouble(0.01);
            category = selectCategory();
            readDate(&day, &month, &year);
            if (addTransaction(account, amount, 2, day, month, year, category) == 1)
            {
                printf("Expense added.\n");
            }
            break;
        case 3:
            account = askAccount();
            printHistory(account);
            break;
        case 4:
            account = askAccount();
            if (getTransactionCount(account) == 0)
            {
                printf("No transactions yet.\n");
                break;
            }
            printHistory(account);
            printf("Number of the transaction to delete: ");
            number = readInt(1, getTransactionCount(account));
            if (deleteTransaction(account, number - 1) == 1)
            {
                printf("Transaction deleted.\n");
            }
            break;
        }
    }
}

void categoriesMenu(void)
{
    int choice = -1;
    int i;

    while (choice != 0)
    {
        printf("\n--- CATEGORIES ---\n");
        printf("1. View categories\n0. Back\n");
        printf("Enter choice: ");
        choice = readInt(0, 1);

        if (choice == 1)
        {
            for (i = 0; i < 5; i++)
            {
                printf(" %d. %s\n", i + 1, categoryName(i));
            }
            printf("(Adding your own categories comes in a later week.)\n");
        }
    }
}

void budgetsMenu(void)
{
    int choice = -1;
    int category, month;
    double amount;

    while (choice != 0)
    {
        printf("\n--- BUDGETS ---\n");
        printf("1. Set a limit\n2. View status for a month\n3. View the year table\n0. Back\n");
        printf("Enter choice: ");
        choice = readInt(0, 3);

        switch (choice)
        {
        case 1:
            category = selectCategory();
            printf("Monthly limit for %s (0 = no limit): ", categoryName(category));
            amount = readDouble(0);
            setLimit(category, amount);
            printf("Limit saved.\n");
            break;
        case 2:
            printf("Month (1-12): ");
            month = readInt(1, 12);
            showStatus(month);
            break;
        case 3:
            showMonthTable();
            break;
        }
    }
}

void goalsMenu(void)
{
    int choice = -1;
    int number;
    double amount;

    while (choice != 0)
    {
        printf("\n--- SAVINGS GOALS ---\n");
        printf("1. Create a goal\n2. Add money to a goal\n3. View goals\n4. Remove a goal\n0. Back\n");
        printf("Enter choice: ");
        choice = readInt(0, 4);

        switch (choice)
        {
        case 1:
            printf("Target amount: ");
            amount = readDouble(0.01);
            createGoal(amount);
            break;
        case 2:
            printf("Goal number (1-5): ");
            number = readInt(1, 5);
            printf("Amount to add: ");
            amount = readDouble(0.01);
            addToGoal(number - 1, amount);
            break;
        case 3:
            showGoals();
            break;
        case 4:
            printf("Goal number (1-5): ");
            number = readInt(1, 5);
            removeGoal(number - 1);
            break;
        }
    }
}

void searchMenu(void)
{
    printf("\nSearch & Filter will be added in week 12 (string functions).\n");
}

void recurringMenu(void)
{
    printf("\nRecurring payments will be added in week 14 (dynamic memory).\n");
}

void reportsMenu(void)
{
    int choice = -1;
    int month;

    while (choice != 0)
    {
        printf("\n--- REPORTS ---\n");
        printf("1. Monthly report\n2. Year summary\n0. Back\n");
        printf("Enter choice: ");
        choice = readInt(0, 2);

        switch (choice)
        {
        case 1:
            printf("Month (1-12): ");
            month = readInt(1, 12);
            printMonthlyReport(month);
            break;
        case 2:
            yearSummary();
            break;
        }
    }
}
