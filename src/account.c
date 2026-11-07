/*
 * account.c - accounts and transactions.
 *
 * Every income or expense goes through addTransaction(), which stores it,
 * changes the balance, updates the spending grid and checks the budget.
 *
 * The data is kept in simple arrays:
 *   accountKind[a], accountBalance[a]       one value per account
 *   txnAmount[a][i], txnType[a][i], ...     one value per transaction i of account a
 */

#include <stdio.h>
#include "common.h"

int    accountCount = 0;
int    accountKind[5];
double accountBalance[5];

int    txnCount[5];
double txnAmount[5][200];
int    txnType[5][200];
int    txnDay[5][200];
int    txnMonth[5][200];
int    txnYear[5][200];
int    txnCategory[5][200];

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

/* adds up the first n numbers of an array */
double sumArray(double values[], int n)
{
    int i;
    double total = 0;

    for (i = 0; i < n; i++)
    {
        total += values[i];
    }

    return total;
}

/* returns the new account number, or -1 if it cannot be made */
int addAccount(int kind, double openingBalance)
{
    int a = accountCount;

    if (a >= 5)
    {
        printf("You cannot have more than 5 accounts.\n");
        return -1;
    }
    if (kind < 0 || kind > 2 || openingBalance < 0)
    {
        printf("Invalid account kind or opening balance.\n");
        return -1;
    }

    accountKind[a] = kind;
    accountBalance[a] = openingBalance;
    txnCount[a] = 0;
    accountCount++;

    return a;
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

    return txnCount[account];
}

double getBalance(int account)
{
    if (account < 0 || account >= accountCount)
    {
        return 0;
    }

    return accountBalance[account];
}

double netWorth(void)
{
    return sumArray(accountBalance, accountCount);
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
        printf(" %d. %-8s %12.2f\n", a + 1, kindName(accountKind[a]),
               accountBalance[a]);
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

/* stores a transaction; returns 1 if it worked, 0 if not (a message says why) */
int addTransaction(int account, double amount, int type,
                   int day, int month, int year, int category)
{
    int n;

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
    if (txnCount[account] >= 200)
    {
        printf("This account already has 200 transactions.\n");
        return 0;
    }

    if (updateBalance(&accountBalance[account], amount, type) == 0)
    {
        printf("Not enough balance for this expense.\n");
        return 0;
    }

    n = txnCount[account];
    txnAmount[account][n] = amount;
    txnType[account][n] = type;
    txnDay[account][n] = day;
    txnMonth[account][n] = month;
    txnYear[account][n] = year;
    txnCategory[account][n] = category;
    txnCount[account]++;

    if (type == 2)
    {
        addToGrid(month, category, amount);
        checkBudget(month, category);
    }

    return 1;
}

/* removes transaction number index (counted from 0) and undoes its effect */
int deleteTransaction(int account, int index)
{
    int i;
    double amount;
    int type, month, category;

    if (account < 0 || account >= accountCount ||
        index < 0 || index >= txnCount[account])
    {
        printf("No such transaction.\n");
        return 0;
    }

    amount = txnAmount[account][index];
    type = txnType[account][index];
    month = txnMonth[account][index];
    category = txnCategory[account][index];

    if (type == 1)
    {
        /* taking income back out must not make the balance negative */
        if (updateBalance(&accountBalance[account], amount, 2) == 0)
        {
            printf("Cannot delete: the balance no longer covers this income.\n");
            return 0;
        }
    }
    else
    {
        updateBalance(&accountBalance[account], amount, 1);
        addToGrid(month, category, -amount);
    }

    /* move the later transactions down one place to close the gap */
    for (i = index; i < txnCount[account] - 1; i++)
    {
        txnAmount[account][i] = txnAmount[account][i + 1];
        txnType[account][i] = txnType[account][i + 1];
        txnDay[account][i] = txnDay[account][i + 1];
        txnMonth[account][i] = txnMonth[account][i + 1];
        txnYear[account][i] = txnYear[account][i + 1];
        txnCategory[account][i] = txnCategory[account][i + 1];
    }
    txnCount[account]--;

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
    if (updateBalance(&accountBalance[from], amount, 2) == 0)
    {
        printf("Not enough balance to transfer.\n");
        return 0;
    }
    updateBalance(&accountBalance[to], amount, 1);

    return 1;
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
    if (txnCount[account] == 0)
    {
        printf("No transactions yet.\n");
        return;
    }

    printf(" No  Date        Type     Category    Amount\n");
    for (i = txnCount[account] - 1; i >= 0; i--)
    {
        printf(" %-3d %02d/%02d/%04d  %-8s %-10s %10.2f\n",
               i + 1,
               txnDay[account][i], txnMonth[account][i], txnYear[account][i],
               txnType[account][i] == 1 ? "Income" : "Expense",
               categoryName(txnCategory[account][i]),
               txnAmount[account][i]);
    }
}

/* total of one type (1 income, 2 expense) in a month, over all accounts */
double monthTotalOfType(int month, int type)
{
    int a, i;
    double total = 0;

    for (a = 0; a < accountCount; a++)
    {
        for (i = 0; i < txnCount[a]; i++)
        {
            if (txnType[a][i] == type && txnMonth[a][i] == month)
            {
                total += txnAmount[a][i];
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
