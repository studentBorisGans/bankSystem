#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sqlite3.h>


#define MAX_CHAR 100 
// #define MAX_ENTRIES 100

// typedef struct {
//     char filename[MAX_CHAR];
// } DatabaseManager;




int main() {
    
    printf("--------------------------------------------------------------------------\n");
    printf("Welcome to BB (Boris' Bank)\n");
    bool cont = true;

    sqlite3 *db;
    int rc;
    rc = sqlite3_open("data/lamp.db", &db);

    sqlite3_stmt *stmt;
    const char *sql = "SELECT * FROM BANK_ACCOUNTS"; // Replace "your_table" with the name of your table

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare query: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // Execute the query and fetch results
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Process each row of the result set
        // Example: Retrieve data from columns
        int id = sqlite3_column_int(stmt, 0); // Assuming the first column is an integer
        const unsigned char *name = sqlite3_column_text(stmt, 1); // Assuming the second column is a text

        // Process the retrieved data
        printf("ID: %d, Name: %s\n", id, name);
    }

    // Finalize the statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    // here


  
    // convert the cJSON object to a JSON string 


    // FILE *fp = fopen("database.json", "w");
    // if (fp == NULL) {
    //     printf("Error: Unable to open the file\n");
    //     return 1;
    // }
    // fputs(json_str, fp);
    // fclose(fp);


    while (cont) {
        int option;
        printf("1. Create Account\n2. Login\nChoice: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                // createAccount();
                cont = false;
                break;
            case 2:
                // login();
                cont = false;
                break;
            default:
                printf("Invalid option- Please input either a 1 or 2\n\n");
        }


    }



    return 0;

}

// void makeJSON() {
    
// }