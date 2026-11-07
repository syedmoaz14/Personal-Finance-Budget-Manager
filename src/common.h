/*
 * common.h - the list of every function other files can call.
 *
 * Every .c file starts with:  #include "common.h"
 * It holds only function names (prototypes), no code.
 *
 * Numbers every file agrees on:
 *   accounts ........ at most 5, numbered from 0
 *   transactions .... at most 200 per account
 *   categories ...... 5, numbered from 0: Food, Transport, Rent, Utilities, Other
 *   goals ........... at most 5, numbered from 0
 *   months .......... 1 to 12 everywhere
 *   type ............ 1 = income, 2 = expense
 *   account kind .... 0 = cash, 1 = bank, 2 = savings
 */

/* utils.c */
int    readInt(int min, int max);
double readDouble(double min);
int    readDate(int *day, int *month, int *year);

/* menus.c */
void accountsMenu(void);
void transactionsMenu(void);
void categoriesMenu(void);
void budgetsMenu(void);
void goalsMenu(void);
void searchMenu(void);
void recurringMenu(void);
void reportsMenu(void);

/* budget.c */
void   setLimit(int category, double amount);
double getLimit(int category);
double percentUsed(int month, int category);
void   checkBudget(int month, int category);
void   showStatus(int month);
void   showMonthTable(void);

/* goals.c */
int    createGoal(double target);
void   addToGoal(int index, double amount);
double goalPercent(int index);
void   showGoals(void);
void   removeGoal(int index);

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
const char *kindName(int kind);
void        listAccounts(void);
double      netWorth(void);
int         updateBalance(double *balance, double amount, int type);
int         addTransaction(int account, double amount, int type,
                           int day, int month, int year, int category);
int         deleteTransaction(int account, int index);
int         transfer(int from, int to, double amount);
void        printHistory(int account);
double      monthIncome(int month);
double      monthExpense(int month);
