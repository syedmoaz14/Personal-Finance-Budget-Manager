/*
 * Personal Finance & Budget Manager
 *
 * main.c only starts the program and shows the main menu.
 * Each menu choice opens a screen from menus.c.
 * Data is kept in memory only; saving to files comes in week 15.
 *
 * Build (from the project folder), compile every .c file in src together:
 *   gcc -Wall src/main.c src/menus.c src/utils.c src/account.c src/grid.c
 *       src/category.c src/budget.c src/goals.c src/report.c -o finance
 */

#include <stdio.h>
#include "common.h"

int main()
{
    int choice = -1;

    printf("\nPersonal Finance & Budget Manager\n");

    clearGrid();
    addAccount(0, 0);       /* start with one cash account */

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
        choice = readInt(0, 8);

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
        case 0:
            printf("\nGoodbye. (Data is not saved yet - file handling is week 15.)\n");
            break;
        }
    }

    return 0;
}
