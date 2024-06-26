#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> //for tolower function



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
    // int numFavs;
} accountDetails;


char* toLowerCase(char *str);
void clearBuffer();

int initialize(DatabaseManager *db);
void saveAccounts(accountDetails *accounts, int *numAccounts, DatabaseManager *db, int shift);
int loadAccounts(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, int shift);

void createAccount(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, int shift);
accountDetails login(accountDetails *accounts, int *numAccounts, bool *contFirst, bool *contSecond);

float deposit(accountDetails *loggedInAccount, accountDetails *accounts, bool *changeHasBeenMade);
float withdraw(accountDetails *loggedInAccount, accountDetails *accounts, bool *changeHasBeenMade);

void encryptString(char *str, int shift);
void decryptString(char *str, int shift);
int encryptInteger(int num, int shift);
int decryptInteger(int num, int shift);


int main() {
    printf("\033[2J"); // Clear entire screen
    printf("\033[0m"); //reset text

    DatabaseManager db;
    strcpy(db.filename, "userData.csv");

    accountDetails accounts[MAX_ACCOUNTS];
    accountDetails loggedInAccount;

    int numAccounts = 0;
    int shift = 10;

    printf("---------------------------------------------------------------------------------------\n");
    printf("\033[1m"); // Bold text
    printf("\t\t\t\tWelcome to BB\n");
    printf("\033[0m"); //reset text
    int accountNumber = 0;
    
    if (initialize(&db)) {
        printf("It looks like this is the first time you're using my program, please select Create Account to get started\n\n");
    } else {
        loadAccounts(accounts, &numAccounts, &accountNumber, &db, shift);
    }

    bool contProgram = true;
    bool contFirst = true;
    bool contSecond = true;

    while (contProgram) {
        while (contFirst) {
            int option1;
            contSecond = true;


            printf("1. Create Account\n2. Login\n3. Terminate Session\nChoice: ");
            scanf("%d", &option1);
    
            switch (option1) {
                case 1:
                    contFirst = false;
                    createAccount(accounts, &numAccounts, &accountNumber, &db, shift);
                    loggedInAccount = login(accounts, &numAccounts, &contFirst, &contSecond);
                    if (!strcmp(loggedInAccount.username, "FAKEUSER")) {
                        contFirst = true;
                        strcpy(loggedInAccount.username, "");
                    }
                    break;

                case 2:
                    contFirst = false;
                    loggedInAccount = login(accounts, &numAccounts, &contFirst, &contSecond);
                    if (!strcmp(loggedInAccount.username, "FAKEUSER")) {
                        contFirst = true;
                        strcpy(loggedInAccount.username, "");
                    }
                    break;
                case 3:
                    printf("Thank you for visiting BB\n\n");
                    exit(EXIT_SUCCESS);
                default:
                    printf("Invalid option- Please input either a 1 or 2\n\n");
                    break;
            }
        }

        bool changeHasBeenMade = false;
        bool green = false;
        float change = 0;

        while (contSecond) {
            int option2;
            char errorMessage[MAX_CHAR];
            

            printf("\033[2J"); // Clear entire screen
            printf("\033[0m"); //reset color
            printf("\033[0;37m"); //grey
            printf("User\t\t\t\t  Account Number\t\t\t\tBalance\n");
            printf("\033[0m"); //reset color
            printf("---------------------------------------------------------------------------------------\n");
            printf("\033[0;34m"); //blue text
            printf("\033[1m"); // Bold text
            printf("%s", loggedInAccount.username);
            printf("\t\t\t\t\t%d", loggedInAccount.accountNum);
            printf("\t\t\t\t\t%.2f\n", loggedInAccount.balance);
            printf("\033[0m"); //reset text

            if (changeHasBeenMade) {
                if (green) {
                    printf("\033[0;32m"); //green text
                } else {
                    printf("\033[0;31m"); //red text
                }
                printf("\033[1m"); // Bold text
                printf("\t\t\t\t\t\t\t\t\t\t%.2f", change);
                printf("\033[0m"); //reset text

                changeHasBeenMade = false;
            }
            printf("\n");

            printf("%s", errorMessage);
            printf("Welcome \033[1m%s\033[0m!\n", loggedInAccount.username);
            printf("\t\b\b\b\033[38;2;255;165;0m1. Transfer menu\n\033[0m\t\b\b\b\033[0;32m2. Deposit\n\033[0m\t\b\b\b\033[0;31m3. Withdraw\n\033[0m\t\b\b\b4. Log Out\nChoice: ");

            scanf("%d", &option2);

            errorMessage[0] = '\0';


            switch(option2) {
                case 1:
                    strcpy(errorMessage, "The transfer menu is currently out of order :( Sorry for the inconvenience.\n");
                    break;

                case 2: 
                    clearBuffer();
                    change = deposit(&loggedInAccount, accounts, &changeHasBeenMade);
                    saveAccounts(accounts, &numAccounts, &db, shift);    
                    green = true;
                    break;

                case 3:
                    clearBuffer();
                    if (loggedInAccount.balance == 0) {
                        strcpy(errorMessage, "Please deposit some money before you withdraw\n");
                    } else {
                        change = withdraw(&loggedInAccount, accounts, &changeHasBeenMade);
                        saveAccounts(accounts, &numAccounts, &db, shift);    
                        green = false;
                    }
                    break;

                case 4:
                    printf("Logging out...\n\n");
                    contSecond = false;
                    contFirst = true;
                    break;
                default:
                    strcpy(errorMessage, "Invalid input, please input one integer coressponding to the options listed :)\n");
                    contSecond = true;
                    break;
            }

        }
    }
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
        return 1;
    } else {
        return 0;
    }
}

int loadAccounts(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, int shift) {
    FILE *fp = fopen(db->filename, "r");
    if (fp == NULL) {
        printf("\nError opening file for reading :(");
        exit(1);
    }
    rewind(fp);

    printf("\t\t\t     Loading accounts...\n");
    while ((fscanf(fp, "%99[^,],%99[^,],%d,%f\n", accounts[*numAccounts].username, accounts[*numAccounts].pw, &accounts[*numAccounts].accountNum, &accounts[*numAccounts].balance)== 4) && 
    (*numAccounts < MAX_ACCOUNTS)) {

        size_t length = strlen(accounts[*numAccounts].username);
        char *decryptedUser = malloc(length + 1);
        if (decryptedUser == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
        }
        
        strcpy(decryptedUser, accounts[*numAccounts].username);
        decryptString(decryptedUser, shift);
        strcpy(accounts[*numAccounts].username, decryptedUser);

        int decryptedPw;
        decryptedPw = atoi(accounts[*numAccounts].pw);
        decryptedPw = decryptInteger(decryptedPw, shift);
        char stringPw[20];
        sprintf(stringPw, "%d", decryptedPw);

        int encryptedNum = decryptInteger(accounts[*numAccounts].accountNum, shift);

        strcpy(accounts[*numAccounts].pw, stringPw);
        accounts[*numAccounts].accountNum = encryptedNum;

        (*numAccounts)++;
        (*accountNumber)++;
    }

    fclose(fp);
    return *numAccounts;
}


void saveAccounts(accountDetails *accounts, int *numAccounts, DatabaseManager *db, int shift) {
    FILE *fp = fopen(db->filename, "w");
    for (int i = 0; i<*numAccounts; i++) {
        accountDetails *account = &accounts[i];

        char *encryptedUser;
        encryptedUser = malloc(strlen(account->username) + 1);
        strcpy(encryptedUser, account->username);
        encryptString(encryptedUser, shift);

        int encryptedPw;
        encryptedPw = atoi(account->pw);
        encryptedPw = encryptInteger(encryptedPw, shift);
        char stringPw[20];
        sprintf(stringPw, "%d", encryptedPw);

        int encryptedNum = encryptInteger(account->accountNum, shift);

        fprintf(fp, "%s,%s,%d,%f\n", encryptedUser, stringPw, encryptedNum, account->balance);
    }
    fclose(fp);
}

void createAccount(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, int shift) {
    accountDetails *lastAccount = &accounts[*numAccounts];
    char user[MAX_CHAR];
    char pw[MAX_CHAR];
    bool cont = true;

    while (cont) {
        cont = false;

        printf("\n\nCREATE ACCOUNT (Input any sequence of characters and/or integers for both fields; however do not input spaces)\n");
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
    float balance = 0;
    (*accountNumber)++;

    strcpy(lastAccount->username,toLowerCase(user));
    strcpy(lastAccount->pw, toLowerCase(pw));
    lastAccount->accountNum = *accountNumber;
    lastAccount->balance = balance;

    char *encryptedUser;
    encryptedUser = malloc(strlen(lastAccount->username) + 1);
    strcpy(encryptedUser, lastAccount->username);
    encryptString(encryptedUser, shift);

    int encryptedPw;
    encryptedPw = atoi(lastAccount->pw);
    encryptedPw = encryptInteger(encryptedPw, shift);
    char stringPw[20];
    sprintf(stringPw, "%d", encryptedPw);


    int encryptedNum = encryptInteger(lastAccount->accountNum, shift);


    FILE *file = fopen(db->filename, "a"); // Open the file in append mode
    if (file == NULL) { // Check if file opening was successful
        printf("Error opening file for writing!\n");
        exit(1);
    } else {
        fprintf(file, "%s,%s,%d,%f\n", encryptedUser, stringPw, encryptedNum, lastAccount->balance);
        printf("Submission Saved!\n\n");
    }
    fclose(file);
    printf("\nAccount created!!! Please write down your Username and Password somewhere safe and login now.\n\n");
}

accountDetails login(accountDetails *accounts, int *numAccounts, bool *contFirst, bool *contSecond) {

    bool cont = true;
    char user[MAX_CHAR];
    char pw[MAX_CHAR];

    if (*numAccounts == 0) {
        printf("No accounts detected; please select create account to get started\n\n");
        cont = false;
        *contSecond = false;
        *contFirst = true;
    }

    while (cont) {
        printf("\nLOGIN (input a 0 as Username to return to the main menu)\n\n");
        printf("   Username: ");
        scanf("%s", user);
        printf("\n   Password: ");
        scanf("%s", pw);

        if (strcmp(user, "0")) {
            for (int i = 0; i < *numAccounts; i++) {
                accountDetails *account = &accounts[i];
                int len = (int)strlen(account->username);
                char dummyUser[len];
                char dummyPw[MAX_CHAR];

                strcpy(dummyUser, account->username);
                strcpy(dummyPw, account->pw);

                if (i == (*numAccounts - 1) && strcmp(toLowerCase(user), dummyUser)) { //checks if the function is on the last element of the listed accounts and if the username is incorrect; tells the program that the search is unsuccesful
                    printf("\nCould not find account with that username, please try again\n");
                }
                if (!strcmp(toLowerCase(user), dummyUser)) {
                    if (!strcmp(toLowerCase(pw), dummyPw)) {
                        printf("\nCorrect Username and Password\n");
                        printf("Logging in...\n");
                        return *account;
                    } else {
                        printf("\nIncorrect Password\n");
                        printf("Please try again\n");
                        break;
                    }
                }
            }
        } else {
            cont = false;
        } 
    }
    accountDetails fakeAccount;
    strcpy(fakeAccount.username, "FAKEUSER");
    return fakeAccount;
}

float deposit(accountDetails *loggedInAccount, accountDetails *accounts, bool *changeHasBeenMade) {
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

            continueDepositing = false;
            *changeHasBeenMade = true;
            return deposit;
        } 
    }
    *changeHasBeenMade = false;

    return 0;
}

float withdraw(accountDetails *loggedInAccount, accountDetails *accounts, bool *changeHasBeenMade) {
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
            printf("Withdrawing %.2f from %s's account with account number %d\n", withdraw, loggedInAccount->username, loggedInAccount->accountNum);

            loggedInAccount->balance -= withdraw;
            accounts[index].balance -= withdraw;

            continueWithdrawing = false;
            *changeHasBeenMade = true;
            return withdraw;
        } 
    }
    *changeHasBeenMade = false;
    return 0;
}


void encryptString(char *str, int shift) {
    while (*str) {
        if(*str >= 'a' && *str <= 'z') {
            *str = ((*str- 'a') + shift) % 26 + 'a';
        }
        str++;
    }
}

void decryptString(char *str, int shift) {
    while(*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str = ((*str - 'a') - shift + 26) % 26 + 'a';
        }
        str++;
    }
}

int encryptInteger(int num, int shift) {
    return (num+shift);
}

int decryptInteger(int num, int shift) {
    return (num-shift);
}
