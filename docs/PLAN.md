# Personal Finance & Budget Manager — Design, Work Split & Weekly Roadmap

**Team (BCS-1C):** Syed Moaz Ali (26K-0630) · Syeda Maryam Fatima (26K-0618) · Varaa (26K-0633)

## Context
The project **strictly follows the course outline**. Each week's commit uses only topics taught up to that week; later weeks refactor the code up (variables → arrays → functions → structs → DMA → files). Nothing is used before it is taught, so there are no early functions and no early structs. The one addition beyond the outline is the standard header `time.h`, used only for recurring auto-posting and the default "today" date (week 14).

**Status:** weeks 1–7 are done. The current code (`src/main.c`) is a single `main()` using only I/O, variables, `if`, `switch`, loops and basic pointer referencing. Varaa joins from week 8, so the team is now three.

**Course outline used (topics per week)**

| Wk | Topics |
|---|---|
| 1–2 | Problem solving, algorithms, pseudo code, flowcharts |
| 3 | Decisions, IDE, program structure, I/O, variables, data types, format specifiers |
| 4 | if, if-else, nested if |
| 5 | switch, nested switch, logical/conditional/bitwise operators, modulus |
| 6 | (Mid I) |
| 7 | Loops (for, while, do-while), referencing, intro to pointers |
| 8 | Loops with 1D arrays |
| 9 | Nested loops with N-D arrays |
| 10 | Functions, passing arrays to functions, pointers with functions (by value / reference, returning) |
| 11 | MID II |
| 12 | String library functions, 2D char arrays, recursion, `const` & `static` |
| 13 | Structures, nested structures, structure arrays |
| 14 | Single pointer (incl. structures) with DMA; 2D pointers with DMA |
| 15 | 2D pointers with DMA; filing in C |

**Team balance (about 40% / 30% / 30%):**
- **Moaz** keeps the central engine and integration: pointers, DMA growth, the spending grid, categories drill-down.
- **Maryam** owns the interface and budgets: menus, input validation, budgets and alerts, category names.
- **Varaa** owns the "extras" that sit on top of the engine: savings goals, search, recurring payments and reports.

Every module is a complete feature its owner can demo and explain alone, and together each member's files cover every course topic at least once, so each has real commits every week and a clear viva story.

---

## 1. Final Architecture (target at semester end)

### Files
```
project/
├── src/
│   ├── main.c            (Moaz)    main menu, program loop, calls every module
│   ├── common.h          (Moaz)    constants, struct definitions, prototypes
│   ├── account.c / .h    (Moaz)    accounts + transaction DMA array (realloc)
│   ├── grid.c / .h       (Moaz)    month×category spend grid (2D array → 2D DMA)
│   ├── category.c / .h   (Moaz)    selectCategory() recursive drill-down, categoryName()
│   │                     (Maryam)  addCategory() name checks + recursive printTree()
│   ├── utils.c / .h      (Maryam)  input validation: readInt, readDouble, readDate
│   ├── menus.c / .h      (Maryam)  all sub-menu screens (nested switch)
│   ├── budget.c / .h     (Maryam)  budget limits, % used, WARNING/OVERSPENT alerts, bars
│   ├── goals.c / .h      (Varaa)   savings goals, deposit, % progress
│   ├── search.c / .h     (Varaa)   search/filter by date range, category, amount, keyword
│   ├── recurring.c / .h  (Varaa)   recurring payments, auto-posted on startup using time.h
│   ├── report.c / .h     (Varaa)   monthly summary on screen + report_YYYY_MM.txt
│   └── storage.c / .h    (split)   Moaz   = accounts, transactions
│                                   Maryam = budgets, categories
│                                   Varaa  = goals, recurring
├── data/             saved .txt data files
├── docs/             plan, overview, flowcharts
├── README.md
└── .gitignore
```
Build: `gcc -Wall src/*.c -o finance`. Until week 10 the project is the single file `src/main.c`.

### Core structures (final form, week 13+)
```c
typedef struct { int day, month, year; } Date;                 // nested struct

typedef struct {
    int id; char name[30]; int parentId;                       // tree via parentId, walked recursively
} Category;

typedef struct {
    int id; double amount; char type;                          // 'I' income / 'E' expense
    Date date; int categoryId; char description[60];
} Transaction;

typedef struct {
    int id; char name[30]; char kind[15];                      // cash/bank/savings
    double balance;
    Transaction *txns; int txnCount, txnCapacity;              // single-pointer DMA, realloc x2 (wk 14)
} Account;

typedef struct {
    int accountId; double amount; int categoryId;
    char description[60]; int dayOfMonth; Date lastPosted;
} Recurring;

typedef struct { char name[40]; double target, saved; int accountId; } Goal;

// grid.c:      double **spendGrid;   // [12 months][categoryCount], 2D pointer DMA (wk 14–15, Moaz)
// budget.c:    double *budgetLimit;  // [categoryCount], one malloc (wk 14, Maryam)
// goals.c:     Goal *goals;          // malloc / realloc (wk 14, Varaa)
// recurring.c: Recurring *items;     // realloc (wk 14, Varaa)
// report.c:    double **compare;     // [category][this month, last month], 2D DMA (wk 14–15, Varaa)
```

### Syllabus coverage: all three members touch every topic
| Course topic | Moaz | Maryam | Varaa |
|---|---|---|---|
| switch / nested switch (wk 5) | main menu | all sub-menus | goals and report option choices |
| loops, pointer referencing (wk 7) | menu loop, balance via pointer | budget progress bars | goal progress bars |
| 1D arrays (wk 8) | accounts list | budget limits, category names | goals as parallel arrays |
| nested loops, N-D arrays (wk 9) | filling the spend grid | printing the month×category table | yearly totals per month and category |
| functions, pointers with functions (wk 10) | `updateBalance(double *bal, ...)` | `readInt(min, max)`, `readDate(&d, &m, &y)`, `checkBudget()` | `addToGoal(index, amount)`, report totals from arrays |
| string library, 2D char arrays (wk 12) | category name lookup | name checks (`strcpy`, `strcmp`) | keyword search (`strstr`, `strcmp`) |
| recursion (wk 12) | `selectCategory()` drill-down | `printTree(parentId, depth)` | `treeTotal(categoryId)` sums a category and all its sub-categories |
| static / const (wk 12) | `static int nextTxnId` | `const` params in utils | `static int nextGoalId`, `const` search parameters |
| structs, nested, struct arrays (wk 13) | Account, Transaction, Date | Category array | Goal and Recurring struct arrays |
| single-pointer DMA (wk 14) | `realloc` growing transactions | `malloc` budget-limit array + `free` | `malloc` goals, `realloc` recurring payments |
| 2D pointers DMA (wk 14–15) | `double **spendGrid` | — (reads it through a function) | `double **compare` for the month vs month report |
| filing (wk 15) | accounts, transactions | budgets, categories | goals, recurring, report file |

---

## 2. Responsibility Split

| | **Moaz — core engine (~40%)** | **Maryam — interface & budgets (~30%)** | **Varaa — extras (~30%)** |
|---|---|---|---|
| Modules | main, common.h, account, grid, `selectCategory` | utils, menus, budget, category add/print | goals, search, recurring, report |
| Hard parts | realloc, 2D DMA, recursive drill-down, integration | input validation loops, alert logic | `time.h` auto-posting, recursive tree total, 2D DMA report |
| Recursion | `selectCategory()` | `printTree()` | `treeTotal()` |
| DMA | transactions (realloc), 2D grid | `malloc` budget limits + `free` | `malloc` goals, `realloc` recurring, 2D comparison table |
| Files | accounts, transactions | budgets.txt, categories.txt | goals.txt, recurring.txt, report file |
| Extra | reviews both members' code and explains concepts before each week | test cases, `docs/TESTING.md`, menu polish | README screenshots, sample data, final flowcharts |

**Why this is safe for evaluation:** each member commits from their own GitHub account every week. Each member's files cover every course topic, and each module is a complete feature that can be demoed and explained alone.

**Interface contract** (agreed in week 10 when functions arrive):
- Moaz provides `int selectCategory(void)` (week 12), `const char *categoryName(int id)`, `double getSpent(int month, int categoryId)` and `void addTransaction(int accountId, double amount, char type, int categoryId, ...)`. Everyone else reads the grid and the transaction list through these functions only.
- Moaz calls `void checkBudget(int month, int categoryId)` (Maryam) after every expense and `void addToGoal(int goalIndex, double amount)` (Varaa) when a deposit is tagged to a goal.
- Maryam provides `int readInt(int min, int max)`, `double readDouble(double min)` and `int readDate(int *d, int *m, int *y)` (rejects dates like 31/02, pointer parameters from week 10). Everyone uses these for input.
- Varaa provides `int searchTransactions(...)` for the search menu, `void postDueRecurring(...)` which Moaz calls at startup (it uses Moaz's `addTransaction`), and `void printMonthlyReport(int month, int year)` for the reports menu.
- Maryam's `menus.c` calls each of these; if a menu needs a new prototype, the owner tells Moaz so `common.h` stays consistent.

---

## 3. Weekly Roadmap (≥1 commit per person per week)

Varaa joins at week 8, so her first commits are in that week.

| Wk | Topic learned | Moaz | Maryam | Varaa |
|---|---|---|---|---|
| 1–2 | algorithms, flowcharts | Repo setup, README | Main-flow flowchart in `docs/` | — (joins later) |
| 3–5 | I/O, if, switch | Main menu with `switch` | Sub-menu screens with nested `switch` (stubs) | — |
| 6 | (Mid I) | Light week | Light week | — |
| **7** ✅ | loops, referencing, intro pointers | `main.c`: menu `while` loop, one cash account, income/expense, balance via `double *`, per-category variables chosen through pointers | Budget limits, 80% / 100% alert check, `[####------]` bar with `for` loop | Joins; sets up Git, reads the repo, learns the menu and flow |
| **8** | loops + 1D arrays | Accounts as arrays (name, balance, kind); transaction history in parallel arrays; net worth loop | Category names + budget limit as arrays; "set limit" / "view limits" with loops | Savings goals as parallel arrays (name, target, saved); create, deposit and progress bar with loops |
| **9** | nested loops, N-D arrays | `spend[12][MAX_CAT]` grid filled on each expense; transactions as 2D array per account | Print the month×category table with nested loops | Yearly report: income and expense totals per month and per category with nested loops |
| **10** | functions, arrays + pointers with functions | Split into `account.c`, `grid.c`; `updateBalance(double *bal, ...)`; `getSpent()`; pass arrays to functions | `utils.c`: `readInt`/`readDouble`/`readDate`; `checkBudget()`; `menus.c` | `goals.c` and `report.c` as functions; `addToGoal()` by pointer; pass arrays to report functions |
| 11 | **MID II** | Light week: bug fixes | Light week: README progress update | Light week: test goals and report |
| **12** | strings, 2D char arrays, recursion, const/static | Recursive `selectCategory()`; `categoryName()` | No-duplicate category names (`strcpy`/`strcmp`); recursive `printTree()` | `search.c` (keyword with `strstr`, category, date, amount); recursive `treeTotal()`; `static` goal id |
| **13** | structs, nested, struct arrays | Refactor to `Account`, `Transaction`, `Date` structs in `common.h` | Move categories to a `Category` struct array | `Goal` and `Recurring` structs; `goals.c` and `recurring.c` on struct arrays |
| **14** | single pointer DMA (+ structs), 2D pointers DMA | `Transaction *txns` with `realloc`; start `double **spendGrid` | Budget limits via `malloc` + `free` | Goals via `malloc`, recurring via `realloc`; `time.h` auto-posting; start `double **compare` for the report |
| **15** | 2D pointers DMA, filing | Finish `spendGrid`; save/load accounts and transactions; free all memory | Save/load `budgets.txt`, `categories.txt` | Save/load `goals.txt`, `recurring.txt`; report to `report_YYYY_MM.txt` |
| After 15 | — | Integration, leak check, final bug fixing | Test bad input, `docs/TESTING.md`, polish screens | Sample data, README screenshots, final flowcharts |

**Notes**
- Recurring payments are auto-posted at startup using `time.h` (`time`, `localtime`) to get today's date, which also serves as the default date for new transactions. `time.h` is not in the outline, so confirm with the instructor; fallback is the user typing today's date.
- The category tree uses `parentId` plus recursion; no pointer-to-pointer children.
- Until week 15 data lives only in memory, so the program says so on exit.
- Varaa's recurring and report modules depend on Moaz's `addTransaction` and `getSpent`; Moaz delivers these in week 10 so she is not blocked.

**How to work together:**
- Before each member's weekly task, Moaz spends 15 minutes on that week's concept with a tiny example. Each person then writes their own module themselves.
- Pair review: Moaz reviews Maryam's and Varaa's code before they push; Maryam and Varaa also read each other's code once a week so both can explain the whole flow.
- `git pull` before starting and `git push` after each commit. Each person edits only their own files. Changes to `main.c` / `common.h` are Moaz's job, so anyone needing a new prototype or struct field asks him.
- Commit messages look like `Week 9: add yearly report totals`.
