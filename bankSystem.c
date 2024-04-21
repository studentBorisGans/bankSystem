#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> //for tolower function

// #include <sqlite3.h>


#define MAX_CHAR 100
#define MAX_ACCOUNTS 100
#define NUM_ACCOUNTS_FILE "numAccounts.text"
#define ACCOUNT_NUMS_FILE "accountNums.text"
#define DEV_USER "boMoney"
#define DEV_PW "2005"



typedef struct {
    char filename[MAX_CHAR];
} DatabaseManager;

typedef struct {
    char username[MAX_CHAR];
    char pw[MAX_CHAR];
    int accountNum;
    int balance;
} accountDetails;

// #define MAX_ENTRIES 100

// typedef struct {
//     char filename[MAX_CHAR];
// } DatabaseManager;

// int checkDevAccess(char user[MAX_CHAR], char pw[MAX_CHAR]);
// void openDev(DatabaseManager db);
char* toLowerCase(char *str);
void readNumAccounts(int *numAccounts);
void writeNumAccounts(int numAccounts);
void readAccountNumber(int *accountNumber);
void writeAccountNumber(int accountNumber);


void createAccount(DatabaseManager dp, int *numAccounts, int *accountNumber);
int login(DatabaseManager dp, int *numAccounts);
void accountMain(DatabaseManager db, int *numAccounts);

int main() {
    DatabaseManager db;
    strcpy(db.filename, "userData.csv");

    accountDetails accounts[MAX_ACCOUNTS];

    int numAccounts;
    readNumAccounts(&numAccounts);
    printf("Num Accounts: %d\n", numAccounts);

    int accountNumber;
    readAccountNumber(&accountNumber);
    printf("Account number: %d\n", accountNumber);

    // FILE *fp = fopen(FILE_NAME, "w");
    
    // fclose(fp);

    printf("--------------------------------------------------------------------------\n");
    printf("Welcome to BB (Boris' Bank)\n");
    bool cont = true;

    while (cont) {
        int option;
        printf("1. Create Account\n2. Login\nChoice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                createAccount(db, &numAccounts, &accountNumber);
                login(db, &numAccounts);
                cont = false;
                break;
            case 2:
                if (login(db, &numAccounts) == 0) {
                    cont = false;
                }
                break;
            default:
                printf("Invalid option- Please input either a 1 or 2\n\n");
                break;
        }
    }
    accountMain(db, &numAccounts);



    return 0;

}


// int checkDevAccess(char user[MAX_CHAR], char pw[MAX_CHAR]) {
//     printf("Check DEv Access\n");
//     if (!strcmp(DEV_USER, user)) {
//         if (!strcmp(DEV_PW, pw)) {
//             return 0;
//         }
//     }
//     return 1;
// }

// void openDev(DatabaseManager db) {
//     bool cont = true;
//     printf("----------------------------------------------------------------------------");

//     while (cont) {
//         int choice;
//         printf("DEV MODE\n");
//         printf("   1. Reset DB");
//         printf("   2. Exit\n");
//         scanf("Choice: %d", &choice);
//         switch (choice) {
//             case 1:
//                 if (remove(db.filename) == 0) {
//                     printf("File deleted succesfully");
//                     writeNumAccounts(0);
//                 } else {
//                     printf("Unable to delete file");
//                 }
                
//             case 2:
//             default:
//                 printf("Invalid option; please input one of the numbers\n");
//         }
//     }
    
// }

char* toLowerCase(char *str) { // 'char*' returns a pointer to a character which represents the lowercase version of the input string 
    size_t length = strlen(str); //calculates length 
    char *lowerStr = malloc(length + 1); // dynamically allocate memory for the lowercase string ( the +1 ensures that theres enough space for the null terminator
    for (size_t i = 0; i < length; i++) { // this loop iterates over each character. for each character toLower is called. The lowercase character is then assigned to the corresponding position in the Lowerstr array
        lowerStr[i] = (char)tolower((unsigned char)str[i]); // Convert each character to lowercase
    }
    lowerStr[length] = '\0'; // Null-terminate the string . signifies end of the string
    return lowerStr;
}

void readNumAccounts(int *numAccounts) {
    FILE *fp = fopen(NUM_ACCOUNTS_FILE, "r");

    if (fp != NULL) {
        fscanf(fp, "%d", numAccounts);
        fclose(fp);
    } else {
        *numAccounts = 0;
    }
}

void writeNumAccounts(int numAccounts) {
    FILE *fp = fopen(NUM_ACCOUNTS_FILE, "w");
    if (fp != NULL) {
        fprintf(fp, "%d", numAccounts);
        fclose(fp);
    }
}

void readAccountNumber(int *accountNumber) {
    FILE *fp = fopen(ACCOUNT_NUMS_FILE, "r");

    if (fp != NULL) {
        fscanf(fp, "%d", accountNumber);
        fclose(fp);
    } else {
        *accountNumber = 1;
    }
}

void writeAccountNumber(int accountNumber) {
    FILE *fp = fopen(ACCOUNT_NUMS_FILE, "w");
    if (fp != NULL) {
        fprintf(fp, "%d", accountNumber);
        fclose(fp);
    }
}

void createAccount(DatabaseManager db, int *numAccounts, int *accountNumber) {
    char user[MAX_CHAR];
    char pw[MAX_CHAR];
    bool cont = true;

    while (cont) {
        cont = false;
        printf("\n\nCREATE ACCOUNT\n");
        printf("   Username: ");
        scanf("%s", user);
        printf("\n   Password: ");
        scanf("%s", pw);
        // printf("\nUser: %s, PW: %s", user, pw);

        FILE *fp = fopen(db.filename, "r");
        if (fp == NULL) { // Check if file opening was successful
            printf("Error opening file for reading!\n");
            FILE *newfp = fopen(db.filename, "w");
            fclose(newfp);
            FILE *fp = fopen(db.filename, "r");

            // exit(1);
        } 

        char dummyUser[MAX_CHAR] = "";
        char dummyPw[MAX_CHAR] = "";
        int garbageAccount;
        int garbageBalance;

        for (int i = 0; i < *numAccounts; i++) {
            fscanf(fp, "%99[^,]%99[^,]%d[^,]%d[^\n]\n", dummyUser, dummyPw, &garbageAccount, &garbageBalance);
            if (!strcmp(toLowerCase(dummyUser), toLowerCase(user))) {
                printf("\nAccount with this username already exists, please choose a different username\n\n");
                cont = true;
                break; //to prevent a repeat of the above message
            }
            if (i == (*numAccounts-1) && strcmp(toLowerCase(dummyUser), toLowerCase(user))) {
                printf("\nAccount username does not already exist :)");

            }
        }
        fclose(fp);

        

    }
    (*numAccounts)++;
    writeNumAccounts(*numAccounts);
    printf("\nNum Accounts: %d\n", *numAccounts);

    int balance = 0;
    (*accountNumber)++;
    writeAccountNumber(*accountNumber);
    printf("Accounts nums: %d\n", *accountNumber);

    


    FILE *newfp = fopen(db.filename, "a");
    if (newfp == NULL) { // Check if file opening was successful
            printf("Error opening file for writing!\n");
            exit(1);
        } 
    fprintf(newfp, "%s,%s,%d,%d\n", toLowerCase(user), toLowerCase(pw), *accountNumber, balance);
    fclose(newfp);
    printf("\nAccount created!!! Please write down your Username and Password somewhere safe and login now.\n\n");

    // fprintf(fp, "%s,%s\n", user, pw);
    // fprintf(file, "%s,%s,%s,%s,%s,%s\n", entry->title, entry->author, entry->duration, entry->genre, entry->comments, entry->link);

}

int login(DatabaseManager db, int *numAccounts) {
    bool cont = true;
    char user[MAX_CHAR];
    char pw[MAX_CHAR];


    if (*numAccounts == 0) {
        printf("No accounts detected; please select create account to get started \n");
        return 1;
    }

    while (cont) {
        printf("\nLOGIN\n\n");
        printf("   Username: ");
        scanf("%s", user);
        printf("\n   Password: ");
        scanf("%s", pw);

        // if (checkDevAccess(user, pw) == 0) {
        //     openDev(db);
        //     cont = false;
        //     break;
        // }

        FILE *fp = fopen(db.filename, "r");
        if (fp == NULL) { // Check if file opening was successful
            printf("\nError opening file for reading!\n");
            exit(1);
        } 

        for (int i = 0; i < *numAccounts; i++) {
            char dummyUser[MAX_CHAR];
            char dummyPw[MAX_CHAR];
            int garbageAccount;
            int garbageBalance;
            // &garbageAccount, &garbageBalance
            
            fscanf(fp, "%99[^,],%99[^,],%d[^,],%d[^\n]\n", dummyUser, dummyPw, &garbageAccount, &garbageBalance);
            printf("\ndummyUser: %s dummyPw: %s i is: %d\n", dummyUser, dummyPw, i);

            // if statement if last element AND username doesnt match
            if (i == (*numAccounts - 1) && strcmp(toLowerCase(user), dummyUser)) { //checks if the function is on the last element of the listed accounts and if the username is incorrect; tells the program that the search is unsuccesful
                printf("\nCould not find account with that username, please try again\n");
            }
            if (!strcmp(toLowerCase(user), dummyUser)) {
                if (!strcmp(toLowerCase(pw), dummyPw)) {
                    printf("\nCorrect Username and Password\n");
                    printf("Logging in...\n");
                    cont = false;
                    break;
                } else {
                    printf("\nIncorrect Password\n");
                    printf("Please try again\n");
                    break;
                }
            }

        }
    }
    return 0;

}

void accountMain(DatabaseManager db, int *numAccounts) {

}