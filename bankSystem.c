// things to fix:
// at the top of login add a cancel button, same for register; otherwise you can be forever stuck if you choose the wrong option; also return value problem
// change it so that whenever they go back to the main screen, the welcome thing and everything reappears
// !!!!: make a global array of representing the structures; for usernames etc. so you compare that in edit entries; 

// thing I can add:
// terminal width thing; at the start ask user for width otherwise it's assumed to be the normal one

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> //for tolower function
#include <time.h> // for encryption

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
    // int numFavs;
} accountDetails;

typedef struct {
    char user[MAX_CHAR];
    char fav1[MAX_CHAR];
    char fav2[MAX_CHAR];
    char fav3[MAX_CHAR];
} favoriteDetails;

char* toLowerCase(char *str);
void clearBuffer();

int initialize(DatabaseManager *db, DatabaseManager *favs);
void saveAccounts(accountDetails *accounts, int *numAccounts, DatabaseManager *db, int shift);
// void saveFavorites(accountDetails *accounts, int *numAccounts, DatabaseManager *db);
int loadAccounts(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, int shift);
void loadFavorites(favoriteDetails *favorites, DatabaseManager *fav, int *numAccounts);
// , char *users[MAX_ACCOUNTS], size_t usersSize)


void createAccount(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, DatabaseManager *favs, int shift);
accountDetails login(accountDetails *accounts, int *numAccounts, bool *contFirst, bool *contSecond);

float deposit(accountDetails *loggedInAccount, accountDetails *accounts, bool *changeHasBeenMade);
float withdraw(accountDetails *loggedInAccount, accountDetails *accounts, bool *changeHasBeenMade);
void transfer(accountDetails *loggedInAccount, accountDetails *accounts, favoriteDetails *favorite, int *numAcconts);

void encryptString(char *str, int shift);
void decryptString(char *str, int shift);
int encryptInteger(int num, int shift);
int decryptInteger(int num, int shift);

// void request(accountDetails *loggedInAccount, accountDetails **accounts, favoriteDetails **favorites);
void editFavorites(accountDetails **accounts, favoriteDetails *favorite, int *numAccounts);

void addFavorite(favoriteDetails *favorite, int numberOfFavorites, accountDetails **accounts, int *numAccounts);

int userExists(char *user, accountDetails *accounts, int *numAccounts);


int main() {
    printf("\033[2J"); // Clear entire screen
    printf("\033[0m"); //reset text

    DatabaseManager db;
    strcpy(db.filename, "userData.csv");

    DatabaseManager favs;
    strcpy(favs.filename, "userFavs.csv");

    accountDetails accounts[MAX_ACCOUNTS];
    accountDetails loggedInAccount;

    favoriteDetails favorites[MAX_ACCOUNTS];

    char *users  = NULL;
    size_t usersSize = 0;

    int numAccounts = 0;

    time_t rawtime;
    struct tm * timeinfo;
    int hour;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    hour = timeinfo->tm_hour;
    // int shift = hour % 26;
    int shift = 10;

    printf("---------------------------------------------------------------------------------------\n");
    printf("\033[1m"); // Bold text
    printf("\t\t\tWelcome to BB (Boris' Bank)\n");
    printf("\033[0m"); //reset text
    int accountNumber = 0;
    // int numFavs = 0;
    
    if (initialize(&db, &favs)) {
        printf("It looks like this is the first time you're using my program, please select Create Account to get started\n\n");
    } else {
        loadAccounts(accounts, &numAccounts, &accountNumber, &db, shift);
        loadFavorites(favorites, &favs, &numAccounts); 

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
                    createAccount(accounts, &numAccounts, &accountNumber, &db, &favs, shift);
                    loggedInAccount = login(accounts, &numAccounts, &contFirst, &contSecond);
                    break;

                case 2:
                    // loadAccounts(accounts, &numAccounts, &accountNumber, &db);
                    contFirst = false;
                    loggedInAccount = login(accounts, &numAccounts, &contFirst, &contSecond);
                    break;
                case 3:
                    printf("Thank you for visiting BB\n\n");
                    exit(EXIT_SUCCESS);
                default:
                    printf("Invalid option- Please input either a 1 or 2\n\n");
                    break;
            }
        }
        // inside account
        // printf("\033[2J"); // Clear entire screen

        bool changeHasBeenMade = false;
        bool green = false;
        float change = 0;

        while (contSecond) {
            int option2;
            char errorMessage[MAX_CHAR];
            favoriteDetails *favorite = &favorites[loggedInAccount.accountNum - 1];
            

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
            printf("\t\b\b\b\033[38;2;255;165;0m1. Transfer menu\n\033[0m\t\b\b\b\033[0;32m2. Deposit\n\033[0m\t\b\b\b\033[0;31m3. Withdraw\n\033[0m\t\b\b\b4. View + Edit Account Details\n\t\b\b\b5. Log Out\nChoice: ");

            scanf("%d", &option2);
            clearBuffer();

            errorMessage[0] = '\0';

            // accountDetails *passedAccounts = accounts;

            switch(option2) {
                case 1:
                    if (numAccounts == 1) {
                        strcpy(errorMessage, "There is only 1 account, therefore you cannot transfer money between accounts.\n");
                    } else {
                        char str[] = "message";
                        int i = 10;
                        encryptString(str, i);
                        printf("Encrypted string: %s\n", str);
                        // transfer(&loggedInAccount, accounts, favorite, &numAccounts);
                        decryptString(str, i);
                        printf("Decrypted string: %s\n", str);
                        contSecond = false;
                    }
                    break;
                case 2: 
                    change = deposit(&loggedInAccount, accounts, &changeHasBeenMade);
                    saveAccounts(accounts, &numAccounts, &db, shift);    

                    green = true;
                    break;
                case 3:
                    if (loggedInAccount.balance == 0) {
                        strcpy(errorMessage, "Please deposit some money before you withdraw\n");
                        // printf("Please deposit some money before you withdraw\n");
                    } else {
                        change = withdraw(&loggedInAccount, accounts, &changeHasBeenMade);
                        saveAccounts(accounts, &numAccounts, &db, shift);    
                        green = false;
                    }
                    
                    break;
                case 4:
                    // details();
                    break;
                case 5:
                    // logout();
                    printf("Logging out...\n\n");
                    contSecond = false;
                    contFirst = true;
                    break;
                default:
                    strcpy(errorMessage, "Invalid input, please input one integer coressponding to the options listed :)\n");
                    contSecond = true;
                    break;
            }


            // contSecond = false;
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

int initialize(DatabaseManager *db, DatabaseManager *favs) {
    FILE *fav = fopen(favs->filename, "r");
    fclose(fav);

    FILE *fp = fopen(db->filename, "r");
    if (fp == NULL) {
        perror("fp error");
        return 1;
    } else {
        return 0;
    }
}
// , char *users, size_t usersSize
int loadAccounts(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, int shift) {
    FILE *fp = fopen(db->filename, "r");
    if (fp == NULL) {
        printf("\nError opening file for reading :(");
        exit(1);
    }
    rewind(fp);


    printf("\t\t\t    Loading accounts...\n");
    while ((fscanf(fp, "%99[^,],%99[^,],%d,%f\n", accounts[*numAccounts].username, accounts[*numAccounts].pw, &accounts[*numAccounts].accountNum, &accounts[*numAccounts].balance)== 4) && 
    (*numAccounts < MAX_ACCOUNTS)) {
        (*numAccounts)++;
        (*accountNumber)++;

        // char encryptedUser = ;
        // encryptedUser = malloc(strlen(account->username) + 1);
        // strcpy(encryptedUser, accounts[*numAccounts].username);
        size_t length = strlen(accounts[*numAccounts].username);
        char *encryptedUser = malloc(length + 1);
        if (encryptedUser == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            // Handle the error or exit the program
            // For example, you might return from the function with an error status
        }

        strcpy(encryptedUser, accounts[*numAccounts].username);

        printf("Encrypted string: %s\n", encryptedUser);
        decryptString(encryptedUser, shift);
        printf("Decrypted string: %s\n", encryptedUser);
        // strcpy(encryptedUser, accounts[*numAccounts].username);
        // strcpy(accounts[*numAccounts].username, encryptedUser);


        // char *encryptedPw;
        // encryptedPw = malloc(strlen(account->pw) + 1);
        // strcpy(encryptedPw, account->pw);
        // encryptString(encryptedPw, shift);

        // int encryptedNum = encryptInteger(account->accountNum, shift);

        // usersSize++; 
        // char *temp = realloc(users, usersSize * sizeof(int));
        // users = temp;
        // users[usersSize-1] = accounts[*numAccounts].username;


    }

    fclose(fp);
    return *numAccounts;
}

void loadFavorites(favoriteDetails *favorites, DatabaseManager *fav, int *numAccounts) {
    FILE *fp = fopen(fav->filename, "r");
    if (fp == NULL) {
        printf("\nError opening favorite file for reading :(");
        exit(1);
    }
    rewind(fp);

    printf("\t\t\t    Loading favorites...\n\n");
    for (int i = 0; i < *numAccounts; i++) {
        fscanf(fp, "%99[^,\n],%99[^,\n],%99[^,\n],%99[^\n]\n", favorites[i].user, favorites[i].fav1, favorites[i].fav2, favorites[i].fav3);
    }
    // while ((fscanf(fp, "%99[^,],%99[^,],%99[^,],%99[^\n]\n", favorites[*numAccounts].username, accounts[*numAccounts].pw, &accounts[*numAccounts].accountNum, &accounts[*numAccounts].balance, &accounts[*numAccounts].numFavs)== 5) && 
    // (*numAccounts < MAX_ACCOUNTS)) {
    //     (*numAccounts)++;
    //     (*accountNumber)++;

    // }

    fclose(fp);
}

void saveAccounts(accountDetails *accounts, int *numAccounts, DatabaseManager *db, int shift) {
    FILE *fp = fopen(db->filename, "w");
    for (int i = 0; i<*numAccounts; i++) {
        accountDetails *account = &accounts[i];

        char *encryptedUser;
        encryptedUser = malloc(strlen(account->username) + 1);
        strcpy(encryptedUser, account->username);
        encryptString(encryptedUser, shift);

        char *encryptedPw;
        encryptedPw = malloc(strlen(account->pw) + 1);
        strcpy(encryptedPw, account->pw);
        encryptString(encryptedPw, shift);

        int encryptedNum = encryptInteger(account->accountNum, shift);

        fprintf(fp, "%s,%s,%d,%f\n", encryptedUser, encryptedPw, encryptedNum, account->balance);
        
    }
    fclose(fp);
}

void saveFavorites(accountDetails *accounts, int *numAccounts, DatabaseManager *favs, favoriteDetails *favorite) {
    FILE *fp = fopen(favs->filename, "w");
    for (int i = 0; i<*numAccounts; i++) {
        accountDetails *account = &accounts[i];
        fprintf(fp, "%s,", account->username);
    }
    fclose(fp);
}

void createAccount(accountDetails *accounts, int *numAccounts, int *accountNumber, DatabaseManager *db, DatabaseManager *favs, int shift) {
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

    char *encryptedUser;
    encryptedUser = malloc(strlen(lastAccount->username) + 1);
    strcpy(encryptedUser, lastAccount->username);
    encryptString(encryptedUser, shift);

    char *encryptedPw;
    encryptedPw = malloc(strlen(lastAccount->pw) + 1);
    strcpy(encryptedPw, lastAccount->pw);
    encryptString(encryptedPw, shift);

    int encryptedNum = encryptInteger(lastAccount->accountNum, shift);


    FILE *favsFile = fopen(favs->filename, "a"); // Open the file in append mode
    if (favsFile == NULL) { // Check if file opening was successful
        printf("Error opening favs file for writing!\n");
        exit(1);
    } else {
        fprintf(favsFile, "%s,%s,%s,%s\n", lastAccount->username, "{}", "{}", "{}");
    }
    fclose(favsFile);

    FILE *file = fopen(db->filename, "a"); // Open the file in append mode
    if (file == NULL) { // Check if file opening was successful
        printf("Error opening file for writing!\n");
        exit(1);
    } else {
        fprintf(file, "%s,%s,%d,%f\n", encryptedUser, encryptedPw, encryptedNum, lastAccount->balance);
        printf("Submission Saved!\n\n");
    }
    fclose(file);

    printf("\nAccount created!!! Please write down your Username and Password somewhere safe and login now.\n\n");
}

accountDetails login(accountDetails *accounts, int *numAccounts, bool *contFirst, bool *contSecond) {

    // for (int i = 0; i < *numAccounts; i++) {
    //     printf("%d : User: %s, Pw: %s, num: %d, balance: %d", i, accounts[i].username, accounts[i].pw, accounts[i].accountNum, accounts[i].balance);
    // }

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

            // printf("%.2f deposited. Have a good day\n"); //should do after save account
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
    return (num+shift) %10;
}

int decryptInteger(int num, int shift) {
    return (num-shift + 10) % 10;
}











void transfer(accountDetails *loggedInAccount, accountDetails *accounts, favoriteDetails *favorite, int *numAccounts) {
    printf("\033[2J"); //Clear entire screen
    printf("\033[0m"); //reset color
    printf("\033[0;37m"); //grey
    printf("User\t\t\t\t  Account Number\t\t\t\tBalance\n");
    printf("\033[0m"); //reset color
    printf("---------------------------------------------------------------------------------------\n");
    printf("\033[0;34m"); //blue text
    printf("\033[1m"); //Bold text
    printf("%s", loggedInAccount->username);
    printf("\t\t\t\t\t%d", loggedInAccount->accountNum);
    printf("\t\t\t\t\t%.2f\n", loggedInAccount->balance);
    printf("\033[0m"); //reset text
    printf("\033[1m"); //Bold text
    printf("\n\t\t\t\t  TRANSFER MENU\n\n");
    printf("\033[0m"); //reset text

    int transferChoice;
    bool continueTransfer = true;

    while (continueTransfer) {
        printf("\t\b\b\b1. Request\n\t\b\b\b2. Send\n\t\b\b\b3. View Requests\n\t\b\b\b4. Edit Favorites\n\t\b\b\b5. Back\nChoice: ");
        scanf("%d", &transferChoice);

        switch(transferChoice) {
            case 1: 
                // request(loggedInAccount, accounts);
                break;
            case 2:
                // send()
                break;
            case 3:
                // viewRequest()
                break;
            case 4:
                editFavorites(&accounts, favorite, numAccounts);
                break;
            case 5: 
                // back()
                break;
            default:
                printf("Invalid input- please input either a 1, 2, 3, or 4");
                break;
        }
        // continueTransfer = false;
    }
}

// void request(accountDetails *loggedInAccount, accountDetails *accounts, favoriteDetails *favorites) {
//     int requestChoice;
//     printf("\t\b\b\b1. Search by Favorites\n\t\b\b\b2. Search by Username\n\t\b\b\b3. Back\nChoice: ");
//     scanf("%d", &requestChoice);

//     switch(requestChoice) {
//         case 1:
//             // search favs
//         case 2:
//             // search user
//         case 3:
//             // back
//         default:
//             printf("Invalid input- please input either a 1, 2, or 3");
//     }

// }


void editFavorites(accountDetails **accounts, favoriteDetails *favorite, int *numAccounts) {
    bool continueEditing = true;
    bool *continueEditingPointer = &continueEditing;
    // 0x16fdf01b7
    // 0x16fdf0240000064
    // <read memory from 0x747365776566 failed (0 of 8 bytes read)>
    // favoriteDetails *favorite2 = favorite;

    while(*continueEditingPointer) {
        int favoriteChoice;
        int numberOfFavorites = 0;
        bool hasFavorite = false;
        // char 
        // <parent failed to evaluate: parent failed to evaluate: read memory from 0x747365776566 failed (0 of 8 bytes read)>
        // <read memory from 0x747365776566 failed (0 of 8 bytes read)>
        printf("\nYour favorites: \n");
        if (strcmp(favorite->fav1, "{}")) {
            numberOfFavorites++;
            printf("\t\b\b\b%s\n", favorite->fav1);
            hasFavorite = true;
        }
        if (strcmp(favorite->fav2, "{}")) {
            numberOfFavorites++;
            printf("\t\b\b\b%s\n", favorite->fav2);
            hasFavorite = true;
        }
        if (strcmp(favorite->fav3, "{}")) {
            numberOfFavorites++;
            printf("\t\b\b\b%s\n", favorite->fav3);
            hasFavorite = true;
        }
        if (!hasFavorite) {
            printf("\t\b\b\bYou have no favorites :(\n");
        }


        printf("You have %d favorites out of 3.\n\t\b\b\b1. Back\n\t\b\b\b2. Edit Existing Favorite\n\t\b\b\b", numberOfFavorites);
        if (numberOfFavorites < 3) {
            printf("3. Add Favorite");
        }

        printf("\nChoice: ");
        scanf("%d", &favoriteChoice);

        switch(favoriteChoice) {
            case 1:
                *continueEditingPointer = false;
                break;
            case 2:
                // edit
                break;
            case 3:
                if (numberOfFavorites < 3) {
                    addFavorite(favorite, numberOfFavorites, accounts, numAccounts);

                } else {
                    printf("\nYou already have the maximum amount of favorites, please select edit favorites instead\n");
                }
                *continueEditingPointer = true;

                break;
            default:
                printf("Invalid input- please input either a 1, 2, or 3 (only if you have less than 3 favorites)");
                break;
        }
    }
    
}

void addFavorite(favoriteDetails *favorite, int numberOfFavorites, accountDetails **accounts, int *numAccounts) {
    bool goodUser = false;
    char *user;

    while (!goodUser) {
        printf("\nEnter a valid Username to add as favorite: ");
        scanf("%s", user);

        if (userExists(user, &accounts, numAccounts)) {
            goodUser = true;
        } else {
            printf("\nCould not find a user with that name. Please try again.\n");
            user = "\0"; //so that user can be global 
        }
    }

    
    if (numberOfFavorites == 0) {
        strcpy(favorite->fav1, user);
    } else if (numberOfFavorites == 1) {
        strcpy(favorite->fav2, user);
    } else if (numberOfFavorites == 2) {
        strcpy(favorite->fav3, user);
    }

    // favorite = NULL;
    // accounts = NULL;
    // numAccounts = NULL;
}

int userExists(char *user, accountDetails *accounts, int *numAccounts) {

    for (int i = 0; i < *numAccounts; i++) {
        // accountDetails account = *accounts[i];
        if (!strcmp(&accounts[i].username, user)) {
            return 1;
        }
    }
    // user = NULL;
    // accounts = NULL;
    // numAccounts = NULL;
    return 0;
}
