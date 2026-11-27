/*
 * storage.c - saves everything when the program exits and loads it again
 * at start-up, in four plain text files in the folder the program runs from:
 *
 *   accounts.txt   accounts and all their transactions
 *   budgets.txt    the monthly limit of each category
 *   goals.txt      savings goals
 *   recurring.txt  recurring payments
 *
 * A missing file just means this is the first run.
 */

#include <stdio.h>
#include <string.h>
#include "common.h"

void saveAccounts(void)
{
    FILE *file = fopen("accounts.txt", "w");
    int a, i;
    struct Transaction t;

    if (file == NULL)
    {
        printf("Could not save accounts.txt\n");
        return;
    }

    fprintf(file, "%d\n", getAccountCount());
    for (a = 0; a < getAccountCount(); a++)
    {
        fprintf(file, "%d %.2f %d\n", getKind(a), getBalance(a),
                getTransactionCount(a));
        for (i = 0; i < getTransactionCount(a); i++)
        {
            t = getTransaction(a, i);
            fprintf(file, "%d %d %d %d %d %.2f %s\n", t.type, t.date.day,
                    t.date.month, t.date.year, t.category, t.amount,
                    t.description);
        }
    }

    fclose(file);
}

void saveBudgets(void)
{
    FILE *file = fopen("budgets.txt", "w");
    int c;

    if (file == NULL)
    {
        printf("Could not save budgets.txt\n");
        return;
    }

    for (c = 0; c < 5; c++)
    {
        fprintf(file, "%.2f\n", getLimit(c));
    }

    fclose(file);
}

void saveGoals(void)
{
    FILE *file = fopen("goals.txt", "w");
    int i;

    if (file == NULL)
    {
        printf("Could not save goals.txt\n");
        return;
    }

    fprintf(file, "%d\n", getGoalCount());
    for (i = 0; i < getGoalCount(); i++)
    {
        fprintf(file, "%.2f %.2f %s\n", getGoalTarget(i), getGoalSaved(i),
                getGoalName(i));
    }

    fclose(file);
}

void saveAllData(void)
{
    FILE *file;

    saveAccounts();
    saveBudgets();
    saveGoals();

    file = fopen("recurring.txt", "w");
    if (file == NULL)
    {
        printf("Could not save recurring.txt\n");
        return;
    }
    saveRecurring(file);
    fclose(file);
}

/* reads the rest of the line into text without the new line */
void readRestOfLine(FILE *file, char text[], int size)
{
    char *newline;

    if (fgets(text, size, file) == NULL)
    {
        text[0] = '\0';
        return;
    }
    newline = strchr(text, '\n');
    if (newline != NULL)
    {
        *newline = '\0';
    }
}

void loadAccounts(void)
{
    FILE *file = fopen("accounts.txt", "r");
    int accounts, a, i, n, kind;
    double balance;
    struct Transaction t;

    if (file == NULL)
    {
        return;
    }

    if (fscanf(file, "%d", &accounts) != 1)
    {
        fclose(file);
        return;
    }

    for (a = 0; a < accounts; a++)
    {
        if (fscanf(file, "%d %lf %d", &kind, &balance, &n) != 3)
        {
            break;
        }
        if (addAccount(kind, balance) < 0)
        {
            break;
        }
        for (i = 0; i < n; i++)
        {
            if (fscanf(file, "%d %d %d %d %d %lf ", &t.type, &t.date.day,
                       &t.date.month, &t.date.year, &t.category,
                       &t.amount) != 6)
            {
                break;
            }
            readRestOfLine(file, t.description, 40);
            appendTransaction(a, t);
        }
    }

    fclose(file);
}

void loadBudgets(void)
{
    FILE *file = fopen("budgets.txt", "r");
    int c;
    double limit;

    if (file == NULL)
    {
        return;
    }

    for (c = 0; c < 5; c++)
    {
        if (fscanf(file, "%lf", &limit) == 1)
        {
            setLimit(c, limit);
        }
    }

    fclose(file);
}

void loadGoals(void)
{
    FILE *file = fopen("goals.txt", "r");
    int n, i;
    double target, saved;
    char name[30];

    if (file == NULL)
    {
        return;
    }

    if (fscanf(file, "%d", &n) == 1)
    {
        for (i = 0; i < n; i++)
        {
            if (fscanf(file, "%lf %lf ", &target, &saved) != 2)
            {
                break;
            }
            readRestOfLine(file, name, 30);
            loadGoal(name, target, saved);
        }
    }

    fclose(file);
}

void loadDataFromFiles(void)
{
    FILE *file;

    loadAccounts();
    loadBudgets();
    loadGoals();

    file = fopen("recurring.txt", "r");
    if (file != NULL)
    {
        loadRecurring(file);
        fclose(file);
    }
}
