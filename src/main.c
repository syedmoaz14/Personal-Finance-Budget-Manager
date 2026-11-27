/*
 * Personal Finance & Budget Manager
 *
 * main.c only starts the program and shows the main menu.
 * Each menu choice opens a screen from menus.c.
 * Data is loaded from text files at start-up and saved when you exit.
 *
 * Build (from the project folder): compile every .c file in the src folder
 * together, i.e. gcc -Wall with all the files, -o finance
 */

#include <stdio.h>
#include "common.h"

int main()
{
    int choice = -1;
    int c, day, month, year;

    printf("\nPersonal Finance & Budget Manager\n");

    clearGrid();
    loadDataFromFiles();
    if (getAccountCount() == 0)
    {
        addAccount(0, 0);   /* first run: start with one cash account */
    }

    postDueRecurring();

    /* show budget alerts for the current month */
    todayDate(&day, &month, &year);
    for (c = 0; c < 5; c++)
    {
        checkBudget(month, c);
    }

    while (choice != 0)
    {
        printf("\n========== PERSONAL FINANCE MANAGER ==========\n\n");
        printf(" 1. Accounts\n");
        printf(" 2. Transactions\n");
        printf(" 3. Categories\n");
        printf(" 4. Budgets\n");
        printf(" 5. Savings Goals\n");
        printf(" 6. Search & Filter\n");
        printf(" 7. Recurring\n");
        printf(" 8. Reports\n");
        printf(" 0. Exit\n\n");
        printf("==============================================\n");
        printf("Enter choice: ");
        choice = readInt(0, 42);   /* 9 to 41 are refused below; 42 is a secret */

        switch (choice)
        {
        case 1:
            accountsMenu();
            break;
        case 2:
            transactionsMenu();
            break;
        case 3:
            categoriesMenu();
            break;
        case 4:
            budgetsMenu();
            break;
        case 5:
            goalsMenu();
            break;
        case 6:
            searchMenu();
            break;
        case 7:
            recurringMenu();
            break;
        case 8:
            reportsMenu();
            break;
        case 42:
            printf("\nThe answer to life, the universe and your budget: spend less than you earn!\n");
            break;
        case 0:
            saveAllData();
            printf("\nData saved. Goodbye.\n");
            break;
        default:
            printf("Please choose 0 to 8.\n");
            break;
        }
    }

    freeAccounts();
    freeGoals();
    freeRecurring();

    return 0;
}
