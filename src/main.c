#include <stdio.h>
#include <stdlib.h>
#include "common.h"



void loadDataFromFiles(void);
void processRecurring(void);
void buildSpendGrid(void);
void displayInitialAlerts(void);

int main(){

    printf("\nInitializing Personal Finance & Budget Manager...\n");
    loadDataFromFiles();
    processRecurring();
    buildSpendGrid();
    displayInitialAlerts();
    printf("Intialization complete.\n\n");

    int choice=-1;


    while(choice!=0){
        printf("========== PERSONAL FINANCE MANAGER ==========\n\n");
        printf(" 1. Accounts\n");
        printf(" 2. Transactions\n");
        printf(" 3. Categories\n");
        printf(" 4. Budgets\n");
        printf(" 5. Savings Goals\n");
        printf(" 6. Search & Filter\n");
        printf(" 7. Recurring\n");
        printf(" 8. Reports\n");
        printf(" 0. Save & Exit\n\n");
        printf("===============================================\n");


        print("\nEnter choice: ");
        // Assumes Maryam's validated input function is used[cite: 1, 2]
        choice=readInt(0,8);


        switch(choice){
            case 1:
            accountsMenu();
            break;
            case 2:
            transactionMenu();
            break;
            case 3:
            categoriesMenu();
            break;
            case 4:
            budgetMenu();
            break;
            case 5:
            goalsMenu();
            break;
            case 6:
            searchMenu();
            break;
            case 7:
            recurringMenu();
            break;
            case 8:
            reportsMenu();
            break;
            case 0;
            printf("\nSaving Data and Closing System.\n");
            break;
            default:
            printf("Invalid selection. Please try again.\n");

        }

    }
    saveAllData();
    freeAllMemory();

void loadDataFromFiles(void) {
    // Moaz will write the real file handling (fscanf) for this in Week 12[cite: 2].
    printf("[System] Loading accounts and transactions from data/*.txt... (Stub)\n");
}

void processRecurring(void) {
    // Moaz will add time.h auto-posting logic here in Week 13[cite: 2].
    printf("[System] Checking for due recurring transactions... (Stub)\n");
}

void buildSpendGrid(void) {
    // Moaz will build the 2D array logic for this in Week 7, and 2D DMA in Week 12[cite: 2].
    printf("[System] Building month-by-category spend grid... (Stub)\n");
}

void displayInitialAlerts(void) {
    // Maryam will provide checkBudget() in Week 8 for you to call here[cite: 2].
    printf("[System] Checking budgets for OVERSPENT alerts... (Stub)\n");
}

    return 0;
}