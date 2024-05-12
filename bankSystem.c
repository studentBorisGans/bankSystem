// things to fix:
// at the top of login add a cancel button, same for register; otherwise you can be forever stuck if you choose the wrong option; also return value problem
// if a user exits the program without using the logout button, their changes do not get saved to the file. maybe add like a "feature" so they input their password to execute their changes before they leave?

// thing I can add:
// terminal width thing; at the start ask user for width otherwise it's assumed to be the normal one

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
    float balance;
} accountDetails;

char* toLowerCase(char *str);
void clearBuffer();
void readNumAccounts(int *numAccounts);
void writeNumAccounts(int numAccounts);
void readAccountNumber(int *accountNumber);
void writeAccountNumber(int accountNumber);

int initialize(DatabaseManager *db);
void saveAccounts(accountDetails *accounts, int *numAccounts, DatabaseManager *db);
int loadAccounts(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db);

void createAccount(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db);
accountDetails login(accountDetails *accounts, int *numAccounts);

void deposit(accountDetails *loggedInAccount, accountDetails *accounts);
void withdraw(accountDetails *loggedInAccount, accountDetails *accounts);


int main() {
    DatabaseManager db;
    strcpy(db.filename, "userData.csv");

    accountDetails accounts[MAX_ACCOUNTS];
    accountDetails loggedInAccount;
    int numAccounts = 0;

    
    if (initialize(&db)) {
        printf("Welcome!!!\n");
    } else {
        printf("Not first time\n");

    }

    printf("Num Accounts in Main: %d\n", numAccounts);
    int accountNumber = 0;

    printf("--------------------------------------------------------------------------\n");
    printf("Welcome to BB (Boris' Bank)\n");
    bool contFirst = true;
    bool contSecond = true;
    loadAccounts(accounts, &numAccounts, &accountNumber, &db);

    while (contFirst) {
        int option1;

        // loadAccounts here; so it's just once for one

        printf("1. Create Account\n2. Login\nChoice: ");
        scanf("%d", &option1);

        switch (option1) {
            case 1:
                createAccount(accounts, &numAccounts, &accountNumber, &db);
                loggedInAccount = login(accounts, &numAccounts);
                contFirst = false;
                break;

            case 2:
                // loadAccounts(accounts, &numAccounts, &accountNumber, &db);
                loggedInAccount = login(accounts, &numAccounts);
                contFirst = false;
                break;

            default:
                printf("Invalid option- Please input either a 1 or 2\n\n");
                break;
        }
    }
    // inside account
    // printf("\033[2J"); // Clear entire screen

    while (contSecond) {
        int option2;
        char errorMessage[MAX_CHAR];

        printf("\033[2J"); // Clear entire screen
        printf("\033[0m"); //reset color
        printf("------------------------------------------------------------------------------------\n");
        printf("\033[0;34m"); //blue text
        printf("\033[1m"); // Bold text
        printf("%s", loggedInAccount.username);
        printf("\t\t\t\t\t%d", loggedInAccount.accountNum);
        printf("\t\t\t\t\t%.2f\n\n", loggedInAccount.balance);
        printf("\033[0m"); //reset text
        printf("%s", errorMessage);
        printf("Welcome %s!\n", loggedInAccount.username);
        printf("\t\b\b\b1. Transfer menu\n\t\b\b\b2. Deposit\n\t\b\b\b3. Withdraw\n\t\b\b\b4. View + Edit Account Details\n\t\b\b\b5. Log Out\nChoice: ");

        scanf("%d", &option2);
        errorMessage[0] = '\0';

        switch(option2) {
            case 1:
                // transfer();
                contSecond = false;
                break;
            case 2: 
                deposit(&loggedInAccount, accounts);
                break;
            case 3:
                withdraw(&loggedInAccount, accounts);
                break;
            case 4:
                // details();
                break;
            case 5:
                // logout();
                contSecond = false;
                break;
            default:
                strcpy(errorMessage, "Invalid input, please input one integer coressponding to the options listed :)\n");
                contSecond = true;
                break;
        }


        // contSecond = false;
    }
    saveAccounts(accounts, &numAccounts, &db);    


    return 0;

}


char* toLowerCase(char *str) { // 'char*' returns a pointer to a character which represents the lowercase version of the input string 
    size_t length = strlen(str); //calculates length 
    char *lowerStr = malloc(length + 1); // dynamically allocate memory for the lowercase string ( the +1 ensures that theres enough space for the null terminator
    for (size_t i = 0; i < length; i++) { // this loop iterates over each character. for each character toLower is called. The lowercase character is then assigned to the corresponding position in the Lowerstr array
        lowerStr[i] = (char)tolower((unsigned char)str[i]); // Convert each character to lowercase
    }
    lowerStr[length] = '\0'; // Null-terminate the string . signifies end of the string
    return lowerStr;
}

void clearBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Consume characters until newline or EOF
    }
}

int initialize(DatabaseManager *db) {
    FILE *fp = fopen(db->filename, "r");

    if (fp == NULL) {
        printf("It looks like this is the first time you're running my program\n");
        return 1;
    } else {
        return 0;
    }
}

int loadAccounts(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db) {
    printf("\nBefore loadAccounts: %d\n", *numAccounts);
    FILE *fp = fopen(db->filename, "r");
    if (fp == NULL) {
        printf("\nError opening file for reading :(");
        exit(1);
    }
    rewind(fp);

    printf("Loading accounts...\n");
    while ((fscanf(fp, "%99[^,],%99[^,],%d,%f\n", accounts[*numAccounts].username, accounts[*numAccounts].pw, &accounts[*numAccounts].accountNum, &accounts[*numAccounts].balance)== 4) && 
    (*numAccounts < MAX_ACCOUNTS)) {
        (*numAccounts)++;
        (*accountNumber)++;

    }

    fclose(fp);
    printf("\nafter loadAccounts: %d\n", *numAccounts);
    return *numAccounts;
}

void saveAccounts(accountDetails *accounts, int *numAccounts, DatabaseManager *db) {
    FILE *fp = fopen(db->filename, "w");
    for (int i = 0; i<*numAccounts; i++) {
        accountDetails *account = &accounts[i];
        fprintf(fp, "%s,%s,%d,%f\n", account->username, account->pw, account->accountNum, account->balance);
        
    }
}

void createAccount(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db) {
    accountDetails *lastAccount = &accounts[*numAccounts];
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

        char dummyUser[MAX_CHAR] = "";
        char dummyPw[MAX_CHAR] = "";

        for (int i = 0; i < *numAccounts; i++) {
            accountDetails *account = &accounts[i];
            strcpy(dummyUser, account->username);
            strcpy(dummyPw, account->pw);

            if (!strcmp(toLowerCase(dummyUser), toLowerCase(user))) {
                printf("\nAccount with this username already exists, please choose a different username\n\n");
                cont = true;
                break; //to prevent a repeat of the above message
            }
            if (i == (*numAccounts-1) && strcmp(toLowerCase(dummyUser), toLowerCase(user))) {
                printf("\nGreat choices!!! :)");

            }
        }
    }
    (*numAccounts)++;
    printf("\nNum Accounts: %d\n", *numAccounts);

    float balance = 0;
    (*accountNumber)++;
    printf("Accounts nums: %d\n", *accountNumber);

    strcpy(lastAccount->username,toLowerCase(user));
    strcpy(lastAccount->pw, toLowerCase(pw));
    lastAccount->accountNum = *accountNumber;
    lastAccount->balance = balance;

    printf("User: %s", lastAccount->username);
    printf("User: %s", lastAccount->pw);
    printf("User: %d", lastAccount->accountNum);
    printf("User: %f", lastAccount->balance);

    FILE *file = fopen(db->filename, "a"); // Open the file in append mode
    if (file == NULL) { // Check if file opening was successful
        printf("Error opening file for writing!\n");
        exit(1);
    } else {
        fprintf(file, "%s,%s,%d,%f\n", lastAccount->username, lastAccount->pw, lastAccount->accountNum, lastAccount->balance);
        printf("Submission Saved!\n\n");
    }

    fclose(file);
    printf("\nAccount created!!! Please write down your Username and Password somewhere safe and login now.\n\n");
}

accountDetails login(accountDetails *accounts, int *numAccounts) {

    // for (int i = 0; i < *numAccounts; i++) {
    //     printf("%d : User: %s, Pw: %s, num: %d, balance: %d", i, accounts[i].username, accounts[i].pw, accounts[i].accountNum, accounts[i].balance);
    // }

    bool cont = true;
    char user[MAX_CHAR];
    char pw[MAX_CHAR];


    if (*numAccounts == 0) {
        printf("No accounts detected; please select create account to get started \n");
        cont = false;
    }

    while (cont) {
        printf("\nLOGIN\n\n");
        printf("   Username: ");
        scanf("%s", user);
        printf("\n   Password: ");
        scanf("%s", pw);

        for (int i = 0; i < *numAccounts; i++) {
            accountDetails *account = &accounts[i];
            int len = (int)strlen(account->username);
            char dummyUser[len];
            char dummyPw[MAX_CHAR];

            strcpy(dummyUser, account->username);
            strcpy(dummyPw, account->pw);

            // printf("\ndummyUser: %s dummyPw: %s i is: %d\n", dummyUser, dummyPw, i);

            // if statement if last element AND username doesnt match
            if (i == (*numAccounts - 1) && strcmp(toLowerCase(user), dummyUser)) { //checks if the function is on the last element of the listed accounts and if the username is incorrect; tells the program that the search is unsuccesful
                printf("\nCould not find account with that username, please try again\n");
            }
            if (!strcmp(toLowerCase(user), dummyUser)) {
                if (!strcmp(toLowerCase(pw), dummyPw)) {
                    printf("\nCorrect Username and Password\n");
                    printf("Logging in...\n");
                    return *account;
                    // cont = false;
                    // break;
                } else {
                    printf("\nIncorrect Password\n");
                    printf("Please try again\n");
                    break;
                }
            }

        }
    }
    

}

void deposit(accountDetails *loggedInAccount, accountDetails *accounts) {
    bool continueDepositing = true;
    printf("\033[1m"); // Bold text
    printf("\nDEPOSIT");
    printf("\033[0m"); //reset text

    while (continueDepositing) {
        float deposit;
        int index = loggedInAccount->accountNum - 1;

        printf("\nHow much would you like to deposit? Input any float. If you wish to go back to the previous menu, input a 0\n");
        printf("Choice: ");
        scanf("%f", &deposit);
        if (deposit < 0) {
            printf("\nYou can not input a number less than 0. If you wish to withdraw, please go to the previous menu and select Withdraw.");
        } else if (deposit == 0) {
            continueDepositing = false;
        } else {
            printf("Depositing %.2f into %s's account with account number %d\n", deposit, loggedInAccount->username, loggedInAccount->accountNum);

            loggedInAccount->balance += deposit;
            accounts[index].balance += deposit;

            // printf("%.2f deposited. Have a good day\n"); //should do after save account
            continueDepositing = false;
        } 
    }
}

void withdraw(accountDetails *loggedInAccount, accountDetails *accounts) {
    bool continueWithdrawing = true;
    printf("\033[1m"); // Bold text
    printf("\nWITHDRAW");
    printf("\033[0m"); //reset text

    while (continueWithdrawing) {
        float withdraw;
        int index = loggedInAccount->accountNum - 1;

        printf("\nHow much would you like to withdraw? Input any float. If you wish to go back to the previous menu, input a 0\n");
        printf("Choice: ");
        scanf("%f", &withdraw);
        if (withdraw < 0) {
            printf("\nYou can not input a number less than 0. If you wish to deposit, please go to the previous menu and select Deposit.");
        } else if (withdraw > loggedInAccount->balance) {
            printf("\nYou do not have enough money for this transaction. Remeber that your current account balance is %.2f and you are not authorized to withdraw more money than this.", loggedInAccount->balance);
        } else if (withdraw == 0) {
            continueWithdrawing = false;
        } else {
            printf("Depositing %.2f into %s's account with account number %d\n", withdraw, loggedInAccount->username, loggedInAccount->accountNum);

            loggedInAccount->balance -= withdraw;
            accounts[index].balance -= withdraw;

            continueWithdrawing = false;
        } 
    }
}
