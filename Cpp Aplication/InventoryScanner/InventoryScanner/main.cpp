/*
    Harve Criqui
    Inventory Scanner
    Date : 12/27/2022
*/


#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>

using namespace std;


static int callback(void* unused, int count, char** argv, char** azColName)
{
    return 0;
}

// callback function for select fuction

static int callback2(void* data, int argc, char** argv, char** azColName) {
    int i;
    fprintf(stderr, "%s: ", (const char*)data);

    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

    }

    printf("\n");
    return 0;
}


int main() {

    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    const char* sql;
    bool engaged = true;

    // Open database 

    rc = sqlite3_open("test.db", &db);

    // if database doesn't open print error message, otherwise print opened database successfully

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return(0);
    }
    else {
        fprintf(stderr, "Opened database successfully\n");
    }

    // Table Create Statement

     sql = "CREATE TABLE IF NOT EXISTS Inventory("  \
        "ID INT PRIMARY KEY     AUTOINCREMENT," \
        "NAME           TEXT    NOT NULL," \
        "PRICE          INT     NOT NULL," \
        "NOTES          CHAR(50)," \
        "LOCATION       REAL );";

     rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);



     // Temporary Command Line Interface
     while(engaged == true)
     {
         string sql2;
         printf("Would you like to add to a product to Inventory?(please answer with y/n)\n");
         char ans;
         cin >> ans;
         printf("\n");

         // Obtain Product information

         if (ans == 'y')
         {
             printf("What is the name of the product?\n");
             string name;
             cin >> name;

             // TODO: Find out why it currently skips over price input and fix

             printf("\nWhat is the PRICE of the product?\n");
             string price;
             cin >> price;
             printf("\nAny NOTES about the product?\n");
             string notes;
             cin >> notes;
             printf("\nWhat is the LOCATION of the product?\n");
             string location;
             cin >> location;

             // Input Information into an sql statement (note that this is a string and must be converted to const char* with .c_str() for sqlite3_exec later)

             sql2 = "INSERT INTO Inventory(NAME, PRICE, NOTES, LOCATION)" \
                 "VALUES (" + name + ", " + price + ", " + notes + ", " + location + ");";

             char ans2;

             // double check that the statement is correct before executing

             cout << "do you wish to execute " << sql2 << "?(y/n)" << endl;
             cin >> ans2;
             
             // if correct execute statement
             
             if (ans2 == 'y')
             {
                 printf("Executing code...");
                 rc = sqlite3_exec(db, sql2.c_str(), callback, 0, &zErrMsg);
             }
             
             // if not correct ignore and restart questioning


         }
         else if (ans == 'n')
         {
             // if initial answer is no then set engaged to false and end operation
             engaged = false;
         }
         else
         {
             // if initial answer is anything but y or n then restart and tell user to only input y or n
             printf("Please only input 'y' or 'n'\n");
         }
     }
     // TODO: Select * from database and print them out
    
    // Close database 

    sqlite3_close(db);
	return 0;
}