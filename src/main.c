/*
 * Personal Finance & Budget Manager  -  Week 7 version
 *
 * Uses only topics covered up to Week 7:
 *   input/output, variables, data types, if / else, switch,
 *   operators, for / while / do-while loops, referencing (&) and
 *   introductory pointers (*p).
 * No arrays, functions, structs or files yet - those arrive in later
 * weeks (see docs/PLAN.md).
 *
 * Build:  gcc -Wall src/main.c -o finance
 */

#include <stdio.h>

int main()
{
    /* ---- account (single cash account until arrays in Week 8) ---- */
    double balance = 0;
    double totalIncome = 0;
    double totalExpense = 0;

    /* ---- spending per category (arrays replace these in Week 8) ---- */
    double spentFood = 0, spentTransport = 0, spentRent = 0, spentOther = 0;
    double limitFood = 0, limitTransport = 0, limitRent = 0, limitOther = 0;

    /* pointers used only for referencing, as taught in Week 7 */
    double *balPtr = &balance;
    double *spent;
    double *limit;

    int choice = -1;

    printf("\nPersonal Finance & Budget Manager\n");

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
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("\n--- ACCOUNTS ---\n");
            printf("Cash balance : %.2f\n", *balPtr);
            printf("Total income : %.2f\n", totalIncome);
            printf("Total expense: %.2f\n", totalExpense);
            break;

        case 2:
        {
            int type, cat;
            double amount;

            printf("\n--- TRANSACTIONS ---\n");
            printf("1. Add income\n2. Add expense\n0. Back\n");
            printf("Enter choice: ");
            scanf("%d", &type);

            if (type == 1)
            {
                printf("Income amount: ");
                scanf("%lf", &amount);
                if (amount <= 0)
                {
                    printf("Amount must be positive.\n");
                }
                else
                {
                    *balPtr = *balPtr + amount;
                    totalIncome += amount;
                    printf("Income added. Balance: %.2f\n", *balPtr);
                }
            }
            else if (type == 2)
            {
                printf("Category: 1.Food 2.Transport 3.Rent 4.Other\n");
                printf("Enter category: ");
                scanf("%d", &cat);

                /* point at the chosen category's variables */
                switch (cat)
                {
                case 1: spent = &spentFood;      limit = &limitFood;      break;
                case 2: spent = &spentTransport; limit = &limitTransport; break;
                case 3: spent = &spentRent;      limit = &limitRent;      break;
                case 4: spent = &spentOther;     limit = &limitOther;     break;
                default:
                    printf("Invalid category.\n");
                    spent = NULL;
                    limit = NULL;
                }

                if (spent != NULL)
                {
                    printf("Expense amount: ");
                    scanf("%lf", &amount);

                    if (amount <= 0)
                    {
                        printf("Amount must be positive.\n");
                    }
                    else if (amount > *balPtr)
                    {
                        printf("Not enough balance.\n");
                    }
                    else
                    {
                        *balPtr = *balPtr - amount;
                        totalExpense += amount;
                        *spent += amount;
                        printf("Expense added. Balance: %.2f\n", *balPtr);

                        /* budget check: warning at 80%, alert over 100% */
                        if (*limit > 0)
                        {
                            double pct = *spent * 100 / *limit;
                            if (pct > 100)
                                printf("[!] OVERSPENT: %.0f%% of budget used.\n", pct);
                            else if (pct >= 80)
                                printf("[!] WARNING: %.0f%% of budget used.\n", pct);
                        }
                    }
                }
            }
            break;
        }

        case 3:
            printf("\n--- CATEGORIES ---\n");
            printf("1. Food\n2. Transport\n3. Rent\n4. Other\n");
            printf("(Sub-categories arrive with recursion in Week 12.)\n");
            break;

        case 4:
        {
            int sub, c, i, bars;
            double newLimit, pct;

            printf("\n--- BUDGETS ---\n");
            printf("1. Set limit\n2. View status\n0. Back\n");
            printf("Enter choice: ");
            scanf("%d", &sub);

            if (sub == 1)
            {
                printf("Category: 1.Food 2.Transport 3.Rent 4.Other\n");
                printf("Enter category: ");
                scanf("%d", &c);
                printf("New monthly limit: ");
                scanf("%lf", &newLimit);

                if (newLimit < 0)
                    printf("Limit cannot be negative.\n");
                else if (c == 1) limitFood = newLimit;
                else if (c == 2) limitTransport = newLimit;
                else if (c == 3) limitRent = newLimit;
                else if (c == 4) limitOther = newLimit;
                else printf("Invalid category.\n");
            }
            else if (sub == 2)
            {
                printf("\nCategory    Spent      Limit      Used\n");

                for (c = 1; c <= 4; c++)
                {
                    switch (c)
                    {
                    case 1: printf("Food       "); spent = &spentFood;      limit = &limitFood;      break;
                    case 2: printf("Transport  "); spent = &spentTransport; limit = &limitTransport; break;
                    case 3: printf("Rent       "); spent = &spentRent;      limit = &limitRent;      break;
                    default: printf("Other      "); spent = &spentOther;     limit = &limitOther;
                    }

                    printf("%-10.2f %-10.2f ", *spent, *limit);

                    if (*limit > 0)
                    {
                        pct = *spent * 100 / *limit;
                        bars = (int)(pct / 10);
                        if (bars > 10)
                            bars = 10;

                        printf("[");
                        for (i = 0; i < 10; i++)
                        {
                            if (i < bars)
                                printf("#");
                            else
                                printf("-");
                        }
                        printf("] %.0f%%\n", pct);
                    }
                    else
                    {
                        printf("(no limit set)\n");
                    }
                }
            }
            break;
        }

        case 5:
            printf("\nSavings Goals - coming in Week 13 (structs).\n");
            break;
        case 6:
            printf("\nSearch & Filter - coming in Week 12 (string functions).\n");
            break;
        case 7:
            printf("\nRecurring - coming in Week 14 (dynamic memory).\n");
            break;
        case 8:
            printf("\nReports - coming in Week 15 (file handling).\n");
            break;

        case 0:
            printf("\nGoodbye. (Data is not saved yet - file handling is Week 15.)\n");
            break;

        default:
            printf("Invalid selection. Please try again.\n");
        }
    }

    return 0;
}
