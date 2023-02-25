// Author: Miliano Mikol (mmikol@usc.edu)

typedef struct My402Transaction {
    int index;
    char type;
    time_t date;
    int amount;
    int balance;
    char description[25];
} Transaction;