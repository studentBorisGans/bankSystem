#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cJSON.h"

#define MAX_CHAR 100 
// #define MAX_ENTRIES 100

// typedef struct {
//     char filename[MAX_CHAR];
// } DatabaseManager;




int main() {
    
    printf("--------------------------------------------------------------------------\n");
    printf("Welcome to BB (Boris' Bank)\n");
    bool cont = true;

    cJSON *json = cJSON_CreateObject(); 
    cJSON_AddStringToObject(json, "name", "John Doe"); 
    cJSON_AddNumberToObject(json, "age", 30); 
    cJSON_AddStringToObject(json, "email", "john.doe@example.com"); 
  
    // convert the cJSON object to a JSON string 
    char *json_str = cJSON_Print(json); 

    FILE *fp = fopen("database.json", "w");
    if (fp == NULL) {
        printf("Error: Unable to open the file\n");
        return 1;
    }
    fputs(json_str, fp);
    fclose(fp);


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