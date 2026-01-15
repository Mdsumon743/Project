#include <stdio.h>
#include "functions.h"

int main()
{
    int ch;

    fflush(stdout);

    loadData();

    fflush(stdout);

    FILE *fp = fopen("users.txt", "a+");

    if (!fp)
    {
        printf("Cannot open users file!\n");
        return 1;
    }
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
    {
        printf("No users found. Please register a new user.\n");
        fflush(stdout);
        registerUser();
    }
    fclose(fp);

    while (1)
    {
        printf("\n1.Login  \n2.Register  \n3.Exit\nEnter choice: ");
        fflush(stdout);

        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input! Enter a number.\n");
            continue;
        }

        if (ch == 2)
            registerUser();
        else if (ch == 1 && login())
            break;
        else if (ch == 3)
            return 0;
        else
            printf("Invalid choice! Try again.\n");
    }

    while (1)
    {
        printf("\n1.Income \n2.Expense \n3.Report \n4.Date Report \n5.Monthly Report \n6.Exit \nEnter choice: ");
        fflush(stdout);

        if (scanf("%d", &ch) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input! Enter a number.\n");
            continue;
        }

        if (ch == 1)
            addIncome();
        else if (ch == 2)
            addExpense();
        else if (ch == 3)
            showReport();
        else if (ch == 4)
            reportByDate();
        else if (ch == 5)
            reportByMonth();
        else if (ch == 6)
            return 0;
        else
            printf("Invalid choice! Try again.\n");
    }

    return 0;
}
