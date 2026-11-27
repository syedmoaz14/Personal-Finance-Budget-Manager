/*
 * common.h - structures and the list of every function other files can call.
 *
 * Every .c file starts with:  #include "common.h"
 *
 * Numbers every file agrees on:
 *   accounts ........ at most 5, numbered from 0
 *   transactions .... grow as needed (realloc), numbered from 0 per account
 *   categories ...... 5, numbered from 0: Food, Transport, Rent, Utilities, Other
 *   goals ........... at most 5, numbered from 0
 *   months .......... 1 to 12 everywhere
 *   type ............ 1 = income, 2 = expense
 *   account kind .... 0 = cash, 1 = bank, 2 = savings
 */

#include <stdio.h>      /* FILE is used in the prototypes below */

/* ---------- structures (week 13) ---------- */
struct Date
{
    int day;
    int month;
    int year;
};

struct Transaction
{
    double amount;
    int type;                   /* 1 income, 2 expense */
    struct Date date;           /* nested structure */
    int category;               /* 0-4 */
    char description[40];
};

struct Account
{
    int kind;                   /* 0 cash, 1 bank, 2 savings */
    double balance;
    struct Transaction *txns;   /* grows with realloc (week 14) */
    int count;                  /* transactions stored */
    int capacity;               /* transactions that fit before growing */
};

/* utils.c */
int    readInt(int min, int max);
double readDouble(double min);
int    readDate(int *day, int *month, int *year);
void   readText(char text[], int size);

/* menus.c */
int  askAccount(void);
void accountsMenu(void);
void transactionsMenu(void);
void categoriesMenu(void);
void budgetsMenu(void);
void goalsMenu(void);
void reportsMenu(void);

/* budget.c */
void   setLimit(int category, double amount);
double getLimit(int category);
double percentUsed(int month, int category);
void   checkBudget(int month, int category);
void   showStatus(int month);
void   showMonthTable(void);

/* goals.c */
int    createGoal(const char *name, double target);
int    loadGoal(const char *name, double target, double saved);
void   addToGoal(int index, double amount);
double goalPercent(int index);
void   showGoals(void);
void   removeGoal(int index);
int    getGoalCount(void);
const char *getGoalName(int index);
double getGoalTarget(int index);
double getGoalSaved(int index);
void   freeGoals(void);

/* recurring.c */
void todayDate(int *day, int *month, int *year);
void recurringMenu(void);
void postDueRecurring(void);
void saveRecurring(FILE *file);
void loadRecurring(FILE *file);
void freeRecurring(void);

/* search.c */
void searchMenu(void);

/* storage.c */
void saveAllData(void);
void loadDataFromFiles(void);

/* report.c */
void yearSummary(void);
void printMonthlyReport(int month);

/* category.c */
const char *categoryName(int category);
int         selectCategory(void);

/* grid.c */
void   clearGrid(void);
void   addToGrid(int month, int category, double amount);
double getSpent(int month, int category);
double monthTotal(int month);
double categoryYearTotal(int category);

/* account.c */
int         addAccount(int kind, double openingBalance);
int         getAccountCount(void);
int         getTransactionCount(int account);
double      getBalance(int account);
int         getKind(int account);
struct Transaction getTransaction(int account, int index);
const char *kindName(int kind);
void        listAccounts(void);
double      netWorth(void);
int         updateBalance(double *balance, double amount, int type);
int         appendTransaction(int account, struct Transaction t);
int         addTransaction(int account, double amount, int type,
                           int day, int month, int year, int category,
                           const char *description);
int         deleteTransaction(int account, int index);
int         transfer(int from, int to, double amount);
void        printHistory(int account);
void        printOneTransaction(int number, struct Transaction t);
double      monthIncome(int month);
double      monthExpense(int month);
void        freeAccounts(void);
