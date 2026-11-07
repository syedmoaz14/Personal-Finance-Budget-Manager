/*
 * utils.c - safe keyboard input.
 * Every number the user types goes through these functions,
 * so the rest of the program never sees bad input.
 */

#include <stdio.h>
#include "common.h"

/* throws away whatever is left on the line; returns 1 if there was something */
int skipRestOfLine(void)
{
    int c = getchar();
    int extra = 0;

    while (c != '\n' && c != EOF)
    {
        extra = 1;
        c = getchar();
    }

    return extra;
}

/* asks until the user types a whole number from min to max */
int readInt(int min, int max)
{
    int value, ok, extra;

    while (1)
    {
        ok = scanf("%d", &value);
        if (ok == EOF)
        {
            return min;     /* input ended, stop asking */
        }
        extra = skipRestOfLine();

        if (ok == 1 && extra == 0 && value >= min && value <= max)
        {
            return value;
        }
        printf("Enter a whole number from %d to %d: ", min, max);
    }
}

/* asks until the user types a number that is not below min */
double readDouble(double min)
{
    double value;
    int ok, extra;

    while (1)
    {
        ok = scanf("%lf", &value);
        if (ok == EOF)
        {
            return min;
        }
        extra = skipRestOfLine();

        if (ok == 1 && extra == 0 && value >= min)
        {
            return value;
        }
        printf("Enter a number of at least %.2f: ", min);
    }
}

int daysInMonth(int month, int year)
{
    switch (month)
    {
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
        {
            return 29;
        }
        return 28;
    default:
        return 31;
    }
}

/* asks for year, month and day; the day is checked against the month */
int readDate(int *day, int *month, int *year)
{
    printf("Year (2000-2100): ");
    *year = readInt(2000, 2100);
    printf("Month (1-12): ");
    *month = readInt(1, 12);
    printf("Day (1-%d): ", daysInMonth(*month, *year));
    *day = readInt(1, daysInMonth(*month, *year));

    return 1;
}
