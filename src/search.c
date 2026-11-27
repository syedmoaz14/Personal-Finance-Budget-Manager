/*
 * search.c - find transactions by keyword, category, amount and month.
 *
 * The user fills in only the filters they want. A transaction is shown only
 * if it passes every filter that was filled in.
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common.h"

/* copies text into lower, all in lower case, so the keyword match ignores case */
void toLower(const char *text, char lower[], int size)
{
    int i;

    for (i = 0; i < size - 1 && text[i] != '\0'; i++)
    {
        lower[i] = tolower(text[i]);
    }
    lower[i] = '\0';
}

/* 1 if the transaction passes every filter (category -1, max 0, month 0 = ignore) */
int passes(struct Transaction t, const char *keyword, int category,
           double minAmount, double maxAmount, int month)
{
    char note[40];

    if (category >= 0 && t.category != category)
    {
        return 0;
    }
    if (t.amount < minAmount)
    {
        return 0;
    }
    if (maxAmount > 0 && t.amount > maxAmount)
    {
        return 0;
    }
    if (month > 0 && t.date.month != month)
    {
        return 0;
    }
    if (keyword[0] != '\0')
    {
        toLower(t.description, note, 40);
        if (strstr(note, keyword) == NULL)
        {
            return 0;
        }
    }

    return 1;
}

void searchMenu(void)
{
    char keyword[40];
    int category, month, a, i, found = 0;
    double minAmount, maxAmount, total = 0;
    struct Transaction t;

    if (getAccountCount() == 0)
    {
        printf("No accounts yet.\n");
        return;
    }

    printf("\n--- SEARCH & FILTER (leave a filter empty or 0 to skip it) ---\n");
    printf("Word in the note (press Enter to skip): ");
    readText(keyword, 40);
    toLower(keyword, keyword, 40);

    printf("Category (0 = any, 1-5 = Food, Transport, Rent, Utilities, Other): ");
    category = readInt(0, 5) - 1;
    printf("Minimum amount (0 = none): ");
    minAmount = readDouble(0);
    printf("Maximum amount (0 = none): ");
    maxAmount = readDouble(0);
    printf("Month (0 = any, 1-12): ");
    month = readInt(0, 12);

    printf("\n No  Date        Type     Category       Amount  Note\n");
    for (a = 0; a < getAccountCount(); a++)
    {
        for (i = 0; i < getTransactionCount(a); i++)
        {
            t = getTransaction(a, i);
            if (passes(t, keyword, category, minAmount, maxAmount, month))
            {
                printOneTransaction(i + 1, t);
                found++;
                total += t.amount;
            }
        }
    }

    if (found == 0)
    {
        printf("No transactions match.\n");
    }
    else
    {
        printf("%d found, total %.2f\n", found, total);
    }
}
