# Personal Finance & Budget Manager — Design, Work Split & Weekly Roadmap

**Team (BCS-1C):** Syed Moaz Ali (26K-0630) · Syeda Maryam Fatima (26K-0618)

## Context
The project must be built by semester end with **weekly commits**. At the start, course weeks 1–5 are done (basics, if/else, switch, loops, intro pointers). The final proposal needs structs, recursion, strings, single/2D DMA and file handling, and those topics come later in the course. So the project **grows alongside the course**. Each week's commit uses only what has been taught so far, and later weeks refactor the code up (parallel arrays → structs → DMA → files).

**Team balance:** Moaz has more coding experience, so he takes the harder technical parts: pointers, DMA, the category tree, date logic and integration. That's about 60% of the code. Maryam is newer to programming. She owns simpler, self-contained modules (about 40%) that still **cover every course topic at least once**, so she has real commits every week and can explain her own code in the viva.

---

## 1. Final Architecture (target at semester end)

### Files
```
project/
├── main.c            (Moaz)    main menu, program loop, calls every module
├── common.h          (Moaz)    constants, all struct definitions, prototypes
├── menus.c / .h      (Maryam)  all sub-menu screens (nested switch)
├── utils.c / .h      (Maryam)  input validation: readInt, readDouble, readDate
├── account.c / .h    (Moaz)    accounts + transaction DMA array (realloc)
├── category.c / .h   (Moaz)    category tree (DMA) + recursive drill-down selectCategory()
│                     (Maryam)  addCategory() name checks + recursive printTree()
├── grid.c / .h       (Moaz)    month×category spend grid (2D DMA)
├── recurring.c / .h  (Moaz)    recurring transactions, auto-posting with time.h
├── search.c / .h     (Moaz)    search/filter by date range, category, amount, keyword
├── budget.c / .h     (Maryam)  budget limits, % used, WARNING/OVERSPENT alerts, bars
├── goals.c / .h      (Maryam)  savings goals, deposit, % progress
├── report.c / .h     (Maryam)  monthly summary on screen + report_YYYY_MM.txt
├── storage.c / .h    (split)   Moaz = accounts, transactions, categories, recurring
│                               Maryam = budgets, goals
├── data/             saved .txt data files
├── docs/             plan, overview, flowcharts
├── README.md
└── .gitignore
```
Build: `gcc -Wall *.c -o finance` (MinGW on Windows, gcc on Linux).

### Core structures (final form, in `common.h`)
```c
typedef struct { int day, month, year; } Date;                 // nested struct

typedef struct Category {
    int id; char name[30]; int parentId;
    struct Category **children; int childCount;                // recursive tree, DMA
} Category;

typedef struct {
    int id; double amount; char type;                          // 'I' income / 'E' expense
    Date date; int categoryId; char description[60];
} Transaction;

typedef struct {
    int id; char name[30]; char kind[15];                      // cash/bank/savings
    double balance;
    Transaction *txns; int txnCount, txnCapacity;              // single-pointer DMA, realloc x2
} Account;

typedef struct {
    int accountId; double amount; int categoryId;
    char description[60]; int dayOfMonth; Date lastPosted;
} Recurring;

typedef struct { char name[40]; double target, saved; int accountId; } Goal;

// grid.c:    double **spendGrid;  // [12 months][categoryCount], 2D DMA   (Moaz)
// budget.c:  double *budgetLimit;  // [categoryCount], one malloc          (Maryam)
```

### Syllabus coverage: both members touch every topic
| Course topic | Moaz | Maryam |
|---|---|---|
| switch / nested switch | main menu | all sub-menus |
| loops, 1D arrays | accounts list | budget limits, progress bars |
| nested loops, 2D arrays | filling the spend grid | printing the month×category table |
| functions, pass-by-reference | `updateBalance(double *bal, ...)` | `readInt(min, max)`, `checkBudget()` |
| string library | search (`strstr`, `strcmp`, keyword filters) | category name checks (`strcpy`, `strcmp`) |
| recursion | `selectCategory()` drill-down, `freeTree()` | `printTree(parentId, depth)` |
| static / const | `static int nextTxnId` | `const` params in utils |
| structs, nested, struct arrays | Account, Transaction, Date, Category | Goal struct array |
| single-pointer DMA | `realloc` growing transactions, tree children | `malloc` the goals and budget-limit arrays |
| 2D DMA | `double **spendGrid` | — (reads it through a function) |
| file handling | accounts, transactions, categories, recurring | budgets, goals, report file |

---

## 2. Responsibility Split

| | **Moaz — core engine (~60%)** | **Maryam — features & interface (~40%)** |
|---|---|---|
| Modules | main, account, category tree, grid, recurring, search | menus, utils, budget, goals, report |
| Hard parts | realloc, 2D DMA, recursive drill-down + tree freeing, date logic, integration | — |
| Recursion | `selectCategory()` drill-down | `printTree()` (simple, uses parentId) |
| DMA | transactions (realloc), tree children, 2D grid | one `malloc` each for goals and budget limits, plus `free` |
| Files | accounts, transactions, categories, recurring (dynamic data) | budgets.txt, goals.txt, report file (simple fprintf/fscanf) |
| Extra | reviews Maryam's code and explains concepts before she starts each week | writes README updates, test cases, flowcharts |

**Why this is safe for evaluation:** Maryam commits from her own GitHub account every week. Every course topic appears in her files, and each of her modules is a complete feature she can demo and explain alone. Moaz can explain concepts to her, but she types her own code.

**Interface contract** (agreed in week 8 when functions arrive):
- Moaz provides `int selectCategory(void)` (drill-down, returns categoryId) and `const char *categoryName(int id)`.
- Moaz provides `double getSpent(int month, int categoryId)`. Maryam's budget and report code reads the grid through this function only, so she never touches `double **`.
- Maryam provides `void checkBudget(int month, int categoryId)`. Moaz calls it after every expense and it prints the WARNING / OVERSPENT alert.
- Maryam provides `void addToGoal(int goalIndex, double amount)`. Moaz calls it when a deposit is tagged to a goal.
- Maryam provides `int readInt(int min, int max)`, `double readDouble(double min)` and `int readDate(int *d, int *m, int *y)`. Everyone uses these for input.

---

## 3. Weekly Roadmap (≥1 commit per person per week)

Week numbers follow the course outline. Assumes about a 16-week semester.

| Wk | Topic learned | Moaz | Maryam |
|---|---|---|---|
| **5** | loops, switch | Repo setup, `.gitignore`, README, `main.c` main menu loop | Sub-menu screens for 3–8 using nested switch (stubs). Flowchart of main flow in `docs/` |
| **6** | 1D arrays | Accounts as parallel arrays: add/list, deposit/withdraw, net worth | Category names in an array + budget limit array; "set limit" and "view limits" |
| **7** | nested loops, 2D arrays | Transactions as 2D arrays per account; fill `spend[12][MAX_CAT]` | Print the month×category table with nested loops; progress bar `[#####-----]` |
| **8** | functions, pointers | Split into `account.c`, `grid.c`; `updateBalance()` by pointer; `getSpent()` | `utils.c`: `readInt/readDouble/readDate` with validation; `checkBudget()` alerts |
| — | **MID II** | Light week: bug fixes | Light week: README progress update |
| **9** | strings, recursion, static | `search.c` (keyword, category, date, amount); recursive `selectCategory()` | Category names with `strcpy`/`strcmp` (no duplicates); **recursive `printTree()`** |
| **10** | structs | Refactor to `Account`, `Transaction`, `Date`, `Category` structs | `Goal` struct array; `goals.c`: create goal, deposit, % progress |
| **11** | single-pointer DMA | `Transaction *txns` with `realloc`; tree children via DMA | Goals and budget limits via `malloc` (size from user/category count) + `free` |
| **12** | 2D DMA, filing | `spendGrid` as `double**`; save/load accounts, transactions, categories | Save/load `budgets.txt` and `goals.txt` with `fprintf`/`fscanf` |
| **13** | — | `recurring.c`: auto-post on startup using `time.h` | `report.c`: monthly summary on screen + written to `report_YYYY_MM.txt` |
| **14** | — | Integration, edge cases, free all memory, leak check | Test every menu with bad input, write `docs/TESTING.md`, polish screens |
| **15** | — | Joint: sample data file, final bug fixing | Joint: README with screenshots, final flowchart |
| **16** | — | Viva prep: explain the whole program flow | Viva prep: explain own modules + the data flow |

**How to work together:**
- Before Maryam's weekly task, spend 15 minutes with her going over that week's concept on a tiny example. She then writes her module herself.
- Moaz reviews her code before she pushes (pair review, not rewriting).
- `git pull` before starting and `git push` after each commit. Each person edits only their own files. Changes to `main.c` / `common.h` are Moaz's job, so if Maryam needs a new prototype she asks him.
- Commit messages look like `Week 9: add recursive category tree print`.
