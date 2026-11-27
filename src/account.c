/*
 * account.c - accounts and transactions.
 *
 * Every income or expense goes through addTransaction(), which stores it,
 * changes the balance, updates the spending grid and checks the budget.
 *
 * Each account is a struct Account. Its transactions live in memory that
 * starts small and doubles with realloc when it is full.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

struct Account accounts[5];
int accountCount = 0;

const char *kindName(int kind)
{
    switch (kind)
    {
    case 0:  return "Cash";
    case 1:  return "Bank";
    case 2:  return "Savings";
    default: return "Unknown";
    }
}

/* returns the new account number, or -1 if it cannot be made */
int addAccount(int kind, double openingBalance)
{
    struct Account *a;

    if (accountCount >= 5)
    {
        printf("You cannot have more than 5 accounts.\n");
        return -1;
    }
    if (kind < 0 || kind > 2 || openingBalance < 0)
    {
        printf("Invalid account kind or opening balance.\n");
        return -1;
    }

    a = &accounts[accountCount];
    a->kind = kind;
    a->balance = openingBalance;
    a->count = 0;
    a->capacity = 4;
    a->txns = malloc(a->capacity * sizeof(struct Transaction));
    if (a->txns == NULL)
    {
        printf("Out of memory.\n");
        return -1;
    }

    accountCount++;
    return accountCount - 1;
}

int getAccountCount(void)
{
    return accountCount;
}

int getTransactionCount(int account)
{
    if (account < 0 || account >= accountCount)
    {
        return 0;
    }

    return accounts[account].count;
}

double getBalance(int account)
{
    if (account < 0 || account >= accountCount)
    {
        return 0;
    }

    return accounts[account].balance;
}

int getKind(int account)
{
    if (account < 0 || account >= accountCount)
    {
        return 0;
    }

    return accounts[account].kind;
}

/* returns a copy of one transaction; the caller checks the numbers first */
struct Transaction getTransaction(int account, int index)
{
    return accounts[account].txns[index];
}

double netWorth(void)
{
    int a;
    double total = 0;

    for (a = 0; a < accountCount; a++)
    {
        total += accounts[a].balance;
    }

    return total;
}

void listAccounts(void)
{
    int a;

    if (accountCount == 0)
    {
        printf("No accounts yet.\n");
        return;
    }

    for (a = 0; a < accountCount; a++)
    {
        printf(" %d. %-8s %12.2f\n", a + 1, kindName(accounts[a].kind),
               accounts[a].balance);
    }
    printf("    Net worth: %.2f\n", netWorth());
}

/*
 * Changes a balance through a pointer. type 1 = income, 2 = expense.
 * Returns 1 if it worked, 0 if the amount is bad or an expense is
 * bigger than the balance.
 */
int updateBalance(double *balance, double amount, int type)
{
    if (amount <= 0)
    {
        return 0;
    }

    if (type == 1)
    {
        *balance = *balance + amount;
        return 1;
    }

    if (type == 2)
    {
        if (amount > *balance)
        {
            return 0;
        }
        *balance = *balance - amount;
        return 1;
    }

    return 0;
}

/*
 * Adds a transaction to the account's list (growing the list if needed)
 * and, for an expense, to the spending grid. It does not change the balance;
 * addTransaction does that, and the loader restores balances itself.
 */
int appendTransaction(int account, struct Transaction t)
{
    struct Account *a = &accounts[account];
    struct Transaction *bigger;

    if (a->count == a->capacity)
    {
        bigger = realloc(a->txns, a->capacity * 2 * sizeof(struct Transaction));
        if (bigger == NULL)
        {
            printf("Out of memory: transaction not stored.\n");
            return 0;
        }
        a->txns = bigger;
        a->capacity = a->capacity * 2;
    }

    a->txns[a->count] = t;
    a->count++;

    if (t.type == 2)
    {
        addToGrid(t.date.month, t.category, t.amount);
    }

    return 1;
}

/* stores a transaction; returns 1 if it worked, 0 if not (a message says why) */
int addTransaction(int account, double amount, int type,
                   int day, int month, int year, int category,
                   const char *description)
{
    struct Transaction t;

    if (account < 0 || account >= accountCount)
    {
        printf("Invalid account.\n");
        return 0;
    }
    if (amount <= 0)
    {
        printf("The amount must be above zero.\n");
        return 0;
    }
    if (type != 1 && type != 2)
    {
        printf("Invalid transaction type.\n");
        return 0;
    }
    if (month < 1 || month > 12 || day < 1 || day > 31)
    {
        printf("Invalid date.\n");
        return 0;
    }
    if (category < 0 || category > 4)
    {
        printf("Invalid category.\n");
        return 0;
    }

    if (updateBalance(&accounts[account].balance, amount, type) == 0)
    {
        printf("Not enough balance for this expense.\n");
        return 0;
    }

    t.amount = amount;
    t.type = type;
    t.date.day = day;
    t.date.month = month;
    t.date.year = year;
    t.category = category;
    strncpy(t.description, description, 39);
    t.description[39] = '\0';

    if (appendTransaction(account, t) == 0)
    {
        /* undo the balance change because the record was not stored */
        updateBalance(&accounts[account].balance, amount, type == 1 ? 2 : 1);
        return 0;
    }

    if (type == 2)
    {
        checkBudget(month, category);
    }

    return 1;
}

/* removes transaction number index (counted from 0) and undoes its effect */
int deleteTransaction(int account, int index)
{
    int i;
    struct Account *a;
    struct Transaction t;

    if (account < 0 || account >= accountCount ||
        index < 0 || index >= accounts[account].count)
    {
        printf("No such transaction.\n");
        return 0;
    }

    a = &accounts[account];
    t = a->txns[index];

    if (t.type == 1)
    {
        /* taking income back out must not make the balance negative */
        if (updateBalance(&a->balance, t.amount, 2) == 0)
        {
            printf("Cannot delete: the balance no longer covers this income.\n");
            return 0;
        }
    }
    else
    {
        updateBalance(&a->balance, t.amount, 1);
        addToGrid(t.date.month, t.category, -t.amount);
    }

    /* move the later transactions down one place to close the gap */
    for (i = index; i < a->count - 1; i++)
    {
        a->txns[i] = a->txns[i + 1];
    }
    a->count--;

    return 1;
}

/* moves money from one account to another (not income, not spending) */
int transfer(int from, int to, double amount)
{
    if (from < 0 || from >= accountCount || to < 0 || to >= accountCount)
    {
        printf("Invalid account.\n");
        return 0;
    }
    if (from == to)
    {
        printf("Choose two different accounts.\n");
        return 0;
    }
    if (updateBalance(&accounts[from].balance, amount, 2) == 0)
    {
        printf("Not enough balance to transfer.\n");
        return 0;
    }
    updateBalance(&accounts[to].balance, amount, 1);

    return 1;
}

/* prints one transaction line; number is the number shown to the user */
void printOneTransaction(int number, struct Transaction t)
{
    printf(" %-3d %02d/%02d/%04d  %-8s %-10s %10.2f  %s\n",
           number, t.date.day, t.date.month, t.date.year,
           t.type == 1 ? "Income" : "Expense",
           categoryName(t.category), t.amount, t.description);
}

/* prints the transactions of one account, newest first */
void printHistory(int account)
{
    int i;

    if (account < 0 || account >= accountCount)
    {
        printf("Invalid account.\n");
        return;
    }
    if (accounts[account].count == 0)
    {
        printf("No transactions yet.\n");
        return;
    }

    printf(" No  Date        Type     Category       Amount  Note\n");
    for (i = accounts[account].count - 1; i >= 0; i--)
    {
        printOneTransaction(i + 1, accounts[account].txns[i]);
    }
}

/* total of one type (1 income, 2 expense) in a month, over all accounts */
double monthTotalOfType(int month, int type)
{
    int a, i;
    double total = 0;

    for (a = 0; a < accountCount; a++)
    {
        for (i = 0; i < accounts[a].count; i++)
        {
            if (accounts[a].txns[i].type == type &&
                accounts[a].txns[i].date.month == month)
            {
                total += accounts[a].txns[i].amount;
            }
        }
    }

    return total;
}

double monthIncome(int month)
{
    return monthTotalOfType(month, 1);
}

double monthExpense(int month)
{
    return monthTotalOfType(month, 2);
}

/* gives back all transaction memory; called once when the program exits */
void freeAccounts(void)
{
    int a;

    for (a = 0; a < accountCount; a++)
    {
        free(accounts[a].txns);
        accounts[a].txns = NULL;
    }
    accountCount = 0;
}
