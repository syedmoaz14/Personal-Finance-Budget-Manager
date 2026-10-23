#include <stdio.h>
int readInt(int min,int max);

const char *categoryName(int category){
    switch(category){
        case 0:
        return "Food";
        break;
        case 1:
        return "Transport";
        break;
        case 2:
        return "Rent";
        break;
        case 3:
        return "Utilities";
        break;
        case 4:
        return "Other";
        break;
        default:
        return "Unknown";
        break;
    }
}

int selectCategory(){
    printf("Category:\n");
    for(int i=0;i<5;i++){
        printf(" %d. %s\n", i+1, categoryName(i));
    }
    printf("Enter Category: ");
    return readInt(1, 5) - 1;
}