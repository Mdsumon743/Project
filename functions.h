#ifndef FUNCTIONS_H
#define FUNCTIONS_H

struct Transaction {
    int tid;
    int userId;
    char type[10];
    float amount;
    char date[20];
    char detail[100];
};

struct User {
    int id;
    char username[30];
    char password[30];
};

extern struct Transaction t[1000];
extern int count;
extern int currentUserId;


int login();
void registerUser();


void loadData();
void saveData();
void addIncome();
void addExpense();
void showReport();
void reportByDate();
void reportByMonth();

#endif
