#include <stdio.h>
#include <string.h>
#include "functions.h"

struct Transaction t[1000];
int count = 0;
int currentUserId = -1;

// Login and Registration

int login()
{
    char u[30], p[30];
    struct User usr;
    FILE *fp = fopen("users.txt", "r");

    if (!fp)
    {
        printf("No users found. Please register first.\n");
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
    {
        fclose(fp);
        printf("No users found. Please register first.\n");
        return 0;
    }
    rewind(fp);

    printf("Username: ");
    fflush(stdout);
    scanf("%s", u);
    printf("Password: ");
    fflush(stdout);
    scanf("%s", p);

    while (fscanf(fp, "%d %s %s", &usr.id, usr.username, usr.password) != EOF)
    {
        if (strcmp(u, usr.username) == 0 &&
            strcmp(p, usr.password) == 0)
        {
            currentUserId = usr.id;
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    printf("Invalid login!\n");
    return 0;
}

void registerUser()
{
    struct User u;
    FILE *fp = fopen("users.txt", "a+");
    if (!fp)
    {
        printf("Error opening user file!\n");
        return;
    }

    int lastId = 0;
    rewind(fp);
    while (fscanf(fp, "%d %*s %*s", &lastId) != EOF)
        ;

    u.id = lastId + 1;

    printf("New username: ");
    fflush(stdout);
    scanf("%s", u.username);
    printf("New password: ");
    fflush(stdout);
    scanf("%s", u.password);

    fprintf(fp, "%d %s %s\n", u.id, u.username, u.password);
    fclose(fp);

    printf("User registered! Please login.\n");
}

/* ---------- DATA ---------- */

void loadData()
{
    FILE *fp = fopen("data.txt", "r");
    if (!fp)
    {
        printf("No data.txt found. Starting fresh.\n");
        return;
    }

    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        if (line[0] == '\n' || line[0] == '\0')
            continue;

        int tid, userId;
        char type[10], date[20], detail[100];
        float amount;

        int n = sscanf(line, "%d %d %s %f %s %[^\n]",
                       &tid, &userId, type, &amount, date, detail);

        if (n >= 5)
        {
            t[count].tid = tid;
            t[count].userId = userId;
            strcpy(t[count].type, type);
            t[count].amount = amount;
            strcpy(t[count].date, date);

            if (n == 6)
                strcpy(t[count].detail, detail);
            else
                t[count].detail[0] = '\0';

            count++;
        }
        else
        {
            printf("Skipping malformed line: %s", line);
        }
    }

    fclose(fp);
}

void saveData()
{
    FILE *fp = fopen("data.txt", "w");
    if (!fp)
    {
        printf("Error saving data!\n");
        return;
    }

    for (int i = 0; i < count; i++)
    {
        fprintf(fp, "%d %d %s %.2f %s %s\n",
                t[i].tid, t[i].userId, t[i].type,
                t[i].amount, t[i].date, t[i].detail);
    }
    fclose(fp);
}

/* ---------- MONEY ---------- */

void addIncome()
{
    struct Transaction n;
    n.tid = count + 1;
    n.userId = currentUserId;
    strcpy(n.type, "income");

    printf("Amount: ");
    fflush(stdout);
    scanf("%f", &n.amount);
    printf("Date (YYYY-MM-DD): ");
    fflush(stdout);
    scanf("%s", n.date);

    printf("Detail: ");
    fflush(stdout);
    getchar();
    fgets(n.detail, sizeof(n.detail), stdin);
    n.detail[strcspn(n.detail, "\n")] = 0;

    t[count++] = n;
    saveData();
    printf("Income added successfully!\n");
}

void addExpense()
{
    struct Transaction n;
    n.tid = count + 1;
    n.userId = currentUserId;
    strcpy(n.type, "expense");

    printf("Amount: ");
    fflush(stdout);
    scanf("%f", &n.amount);
    printf("Date (YYYY-MM-DD): ");
    fflush(stdout);
    scanf("%s", n.date);

    printf("Detail: ");
    fflush(stdout);
    getchar();
    fgets(n.detail, sizeof(n.detail), stdin);
    n.detail[strcspn(n.detail, "\n")] = 0;

    t[count++] = n;
    saveData();
    printf("Expense added successfully!\n");
}

void showReport()
{
    float in = 0, ex = 0;
    printf("\n------ Full Report ------\n");

    for (int i = 0; i < count; i++)
    {
        if (t[i].userId == currentUserId)
        {
            printf("%s %.2f %s %s\n",
                   t[i].type, t[i].amount,
                   t[i].date, t[i].detail);
            if (strcmp(t[i].type, "income") == 0)
                in += t[i].amount;
            else
                ex += t[i].amount;
        }
    }
    printf("Income: %.2f \nExpense: %.2f \nBalance: %.2f\n",
           in, ex, in - ex);
}

void reportByDate()
{
    char d[20];
    printf("Date (YYYY-MM-DD): ");
    fflush(stdout);
    scanf("%s", d);

    printf("\n------ Report for %s ------\n", d);
    for (int i = 0; i < count; i++)
    {
        if (t[i].userId == currentUserId &&
            strcmp(t[i].date, d) == 0)
        {
            printf("%s %.2f %s\n",
                   t[i].type, t[i].amount, t[i].detail);
        }
    }
}

void reportByMonth()
{
    int month, year;
    printf("Enter month (MM): ");
    fflush(stdout);
    scanf("%d", &month);
    printf("Enter year (YYYY): ");
    fflush(stdout);
    scanf("%d", &year);

    char monthStr[8];
    snprintf(monthStr, sizeof(monthStr), "%04d-%02d", year, month);

    float in = 0, ex = 0;
    printf("\n------ Report for %02d/%04d ------\n", month, year);

    for (int i = 0; i < count; i++)
    {
        if (t[i].userId == currentUserId &&
            strncmp(t[i].date, monthStr, 7) == 0)
        {
            printf("%s %.2f %s %s\n",
                   t[i].type, t[i].amount, t[i].date, t[i].detail);
            if (strcmp(t[i].type, "income") == 0)
                in += t[i].amount;
            else
                ex += t[i].amount;
        }
    }

    printf("Income: %.2f \nExpense: %.2f \nBalance: %.2f\n",
           in, ex, in - ex);
}
