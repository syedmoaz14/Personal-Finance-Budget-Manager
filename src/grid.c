#include <stdio.h>
double spend[12][5];

void clearGrid(void){
    for(int i=0;i<12;i++){
        for(int j=0;j<5;j++){
            spend[i][j]=0;
        }
    }
}

void addToGrid(int month,int category,double amount){
    if(month<1 || month>12 || category<0 || category>4){
        return;
    }
    spend[month-1][category]+=amount;
}
double getSpent(int month,int category){
    if(month<1 || month>12 || category<0 || category>4){
        return 0;
    }
    return spend[month-1][category];
}

double monthTotal(int month){
    if(month<1 || month>12){
        return 0;
    }
    double total=0;
    for(int x=0;x<5;x++){
        total=total+spend[month-1][x];
    }
    return total;
}

double categoryYearTotal(int category){
    if(category<0 || category>4){
        return 0;
    }
    double total=0;
    for(int i=0;i<12;i++){
        total=total+spend[i][category];
    }
    return total;
}