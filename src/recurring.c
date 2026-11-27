/*
 * recurring.c - rent, salary and subscriptions that repeat every month.
 *
 * At start-up postDueRecurring() reads today's date with time.h and posts
 * every payment that is due, once for each month that was missed. It posts
 * through addTransaction(), so balance, spending grid and budget alerts
 * work exactly as for a payment typed by hand.
 *
 * The list of payments starts small and doubles with realloc when full.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "common.h"

struct Recurring
{
    int account;
    double amount;
    int type;                   /* 1 income, 2 expense */
    int category;
    char description[40];
    int dayOfMonth;             /* 1-28, so every month has this day */
    struct Date lastPosted;     /* month and year of the last posting */
};

struct Recurring *items = NULL;
int itemCount = 0;
int itemCapacity = 0;

/* today's date from the computer's clock */
void todayDate(int *day, int *month, int *year)
{
    time_t now;
    struct tm *local;

    time(&now);
    local = localtime(&now);

    *day = local->tm_mday;
    *month = local->tm_mon + 1;
    *year = local->tm_year + 1900;
}

/* adds one payment to the list; lastMonth and lastYear = last month posted */
int storeRecurring(int account, double amount, int type, int category,
                   const char *description, int dayOfMonth,
                   int lastMonth, int lastYear)
{
    struct Recurring *bigger;
    struct Recurring *r;

    if (itemCount == itemCapacity)
    {
        itemCapacity = (itemCapacity == 0) ? 4 : itemCapacity * 2;
        bigger = realloc(items, itemCapacity * sizeof(struct Recurring));
        if (bigger == NULL)
        {
            printf("Out of memory.\n");
            return 0;
        }
        items = bigger;
    }

    r = &items[itemCount];
    r->account = account;
    r->amount = amount;
    r->type = type;
    r->category = category;
    strncpy(r->description, description, 39);
    r->description[39] = '\0';
    r->dayOfMonth = dayOfMonth;
    r->lastPosted.day = 1;
    r->lastPosted.month = lastMonth;
    r->lastPosted.year = lastYear;
    itemCount++;

    return 1;
}

void showRecurring(void)
{
    int i;

    if (itemCount == 0)
    {
        printf("No recurring payments.\n");
        return;
    }

    for (i = 0; i < itemCount; i++)
    {
        printf(" %d. %-8s %10.2f on day %d of each month  (%s, account %d)\n",
               i + 1, items[i].type == 1 ? "Income" : "Expense",
               items[i].amount, items[i].dayOfMonth,
               items[i].description, items[i].account + 1);
    }
}

void removeRecurring(int index)
{
    int i;

    for (i = index; i < itemCount - 1; i++)
    {
        items[i] = items[i + 1];
    }
    itemCount--;
}

/*
 * Posts every month of every payment that is due. A month is due when it is
 * before this month, or it is this month and today is on or after the
 * payment's day. A payment that fails (for example not enough money) is
 * tried again next time.
 */
void postDueRecurring(void)
{
    int i, m, y;
    int day, month, year;
    struct Recurring *r;

    todayDate(&day, &month, &year);

    for (i = 0; i < itemCount; i++)
    {
        r = &items[i];

        while (1)
        {
            m = r->lastPosted.month + 1;
            y = r->lastPosted.year;
            if (m > 12)
            {
                m = 1;
                y++;
            }

            if (y > year || (y == year && m > month))
            {
                break;
            }
            if (y == year && m == month && day < r->dayOfMonth)
            {
                break;
            }

            if (addTransaction(r->account, r->amount, r->type, r->dayOfMonth,
                               m, y, r->category, r->description) == 0)
            {
                break;
            }
            printf("[Auto-posted] %s %.2f  (%02d/%02d/%04d)\n", r->description,
                   r->amount, r->dayOfMonth, m, y);

            r->lastPosted.month = m;
            r->lastPosted.year = y;
        }
    }
}

void saveRecurring(FILE *file)
{
    int i;

    fprintf(file, "%d\n", itemCount);
    for (i = 0; i < itemCount; i++)
    {
        fprintf(file, "%d %.2f %d %d %d %d %d %s\n",
                items[i].account, items[i].amount, items[i].type,
                items[i].category, items[i].dayOfMonth,
                items[i].lastPosted.month, items[i].lastPosted.year,
                items[i].description);
    }
}

void loadRecurring(FILE *file)
{
    int i, n;
    int account, type, category, dayOfMonth, lastMonth, lastYear;
    double amount;
    char description[40];
    char *newline;

    if (fscanf(file, "%d", &n) != 1)
    {
        return;
    }

    for (i = 0; i < n; i++)
    {
        if (fscanf(file, "%d %lf %d %d %d %d %d ", &account, &amount, &type,
                   &category, &dayOfMonth, &lastMonth, &lastYear) != 7)
        {
            break;
        }
        if (fgets(description, 40, file) == NULL)
        {
            break;
        }
        newline = strchr(description, '\n');
        if (newline != NULL)
        {
            *newline = '\0';
        }
        storeRecurring(account, amount, type, category, description,
                       dayOfMonth, lastMonth, lastYear);
    }
}

void freeRecurring(void)
{
    free(items);
    items = NULL;
    itemCount = 0;
    itemCapacity = 0;
}

void recurringMenu(void)
{
    int choice = -1;
    int account, category, type, dayOfMonth, number;
    int day, month, year;
    int lastMonth, lastYear;
    double amount;
    char note[40];

    while (choice != 0)
    {
        printf("\n--- RECURRING PAYMENTS ---\n");
        printf("1. Add a recurring payment\n2. View them\n3. Remove one\n4. Post due payments now\n0. Back\n");
        printf("Enter choice: ");
        choice = readInt(0, 4);

        switch (choice)
        {
        case 1:
            account = askAccount();
            printf("1. Income (e.g. salary)  2. Expense (e.g. rent): ");
            type = readInt(1, 2);
            printf("Amount: ");
            amount = readDouble(0.01);
            category = (type == 1) ? 4 : selectCategory();
            printf("Day of the month (1-28): ");
            dayOfMonth = readInt(1, 28);
            printf("Note (e.g. Rent): ");
            readText(note, 40);

            /* start from last month, so this month's payment is posted when due */
            todayDate(&day, &month, &year);
            lastMonth = month - 1;
            lastYear = year;
            if (lastMonth < 1)
            {
                lastMonth = 12;
                lastYear--;
            }
            if (storeRecurring(account, amount, type, category, note,
                               dayOfMonth, lastMonth, lastYear) == 1)
            {
                printf("Saved. It will post on day %d of every month.\n", dayOfMonth);
            }
            break;
        case 2:
            showRecurring();
            break;
        case 3:
            if (itemCount == 0)
            {
                printf("No recurring payments.\n");
                break;
            }
            showRecurring();
            printf("Number to remove: ");
            number = readInt(1, itemCount);
            removeRecurring(number - 1);
            printf("Removed.\n");
            break;
        case 4:
            postDueRecurring();
            break;
        }
    }
}
