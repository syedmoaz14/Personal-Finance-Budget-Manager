#include <stdio.h>
const char *categoryName(int category);
int selectCategory(void);

int readInt(int min, int max)
{
    int v;
    scanf("%d", &v);
    return v;
}

int main()
{
    printf("%s\n", categoryName(0));   /* Food */
    printf("%s\n", categoryName(2));   /* Rent */
    printf("%s\n", categoryName(9));   /* Unknown */

    int picked = selectCategory();
    printf("You picked number %d, which is %s\n", picked, categoryName(picked));
    return 0;
}
