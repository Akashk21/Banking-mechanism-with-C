#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "project_h.h"
 
void login();
void sign_up();
void create_upi_handler(customer*);
void generate_ac_no(customer*);
int ID_exists(char*);
 
int main()
{
    int c;
    do
    {
        printf("\n-----------------------------------");
        printf("\n1. Create Account\n2. Login\n0. Exit\n");
        scanf("%d", &c);
        switch(c)
        {
            case 1:
                sign_up();
                break;
            case 2:
                login();
                break;
            case 0:
                break;
            default:
                printf("Invalid Input\n");
                break;
        }
    }while(c != 0);
    return 0;
}
 
void login()
{
    int ch;
    do
    {
        printf("\n------------------------------");
        printf("\n1. Login as customer\n2. Login as employee\n0. Go Back\n");
        scanf("%d", &ch);
        switch (ch)
        {
            case 1:
                customer_login();
                break;
            case 2:
                employee_login();
                break;
            case 0:
                break;
            default:
                printf("Invalid Input\n");
                break;
        }
    }while(ch != 0);
}
 
void sign_up()
{
    customer c;
    char temp[20];
    do
    {
        printf("\nEnter your ID: ");
        scanf("%s", c.customer_personal_details.ID);
        // if(ID_exists(c.customer_personal_details.ID))
        // {    
        //     printf("Sorry, ID already exists, please try again\n");
        //     continue;
        // }
        printf("\nEnter your password: ");
        scanf("%s", c.customer_personal_details.password);
        printf("\nConfirm Password: ");
        scanf("%s", temp);
        if(strcmp(temp, c.customer_personal_details.password))
        {
            printf("\nPasswords do not match, try again\n");
        }
    }while(strcmp(temp, c.customer_personal_details.password));
    encrypt(c.customer_personal_details.password);
    create_upi_handler(&c);
    c.transactions.transaction_count = 0;
    c.balance = 0;
    generate_ac_no(&c);
    strcpy(c.ifsc_code, "MYBANK");
    printf("\nEnter the type of account you intend to create: ");
    scanf("%s", c.type);
    c.loan.loan_status = 0;
    c.ac_status = 0;
    int valid_branch = 0;
    while(!valid_branch)
    {
        printf("\nEnter the branch in which you wish to create your account: ");
        scanf("%s",c.branch);
        if(!strcmp(c.branch,"A")||!strcmp(c.branch,"B")||!strcmp(c.branch,"C")||!strcmp(c.branch,"D")||!strcmp(c.branch,"E"))
        {
            valid_branch = 1;
        }
        else
        {
            strcpy(c.branch,"");
        }
    }
    // writing into file
    FILE *fp;
    fp = fopen("CUSTOMERS.DAT", "ab");
    c.customer_no = (ftell(fp) / sizeof(customer));
    fwrite(&c, sizeof(c), 1, fp);
    fclose(fp);
    printf("\nYour account creation application has been submitted.\n");
}
 
void create_upi_handler(customer* c)
{
    char temp[20];
    printf("\nEnter UPI Handler: ");
    scanf("%s", c -> upi_handler);
    strcat(c -> upi_handler,"@mybank.com");
    do
    {
        printf("\nEnter your 4 digit passcode: ");
        scanf("%s", c ->upi_passcode);
        if(strlen(c -> upi_passcode) != 4)
        {
            printf("Pin needs to be exactly four digits long\n");
            continue;
        }
        printf("\nConfirm Passcode: ");
        scanf("%s", temp);
        if(strcmp(temp, c -> upi_passcode))
        {
            printf("\nPasscodes do not match, try again\n");
        }
    }while(strcmp(temp, c -> upi_passcode));
    encrypt(c -> upi_passcode);
}
 
void generate_ac_no(customer* c)
{
    srand(time(0));
    int num = rand();
    char temp[20];
    strcpy(c -> ac_no, "BANK0");
    itoa(num, temp, 10);
    strcat(c -> ac_no, temp);
    printf("Your account number is: %s", c -> ac_no);
}
 
int ID_exists(char *ID)
{
    int found = 0;
    FILE *fp = fopen("CUSTOMERS.DAT", "r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return -1;
    }
    customer temp;
    while(fread(&temp, sizeof(customer), 1, fp))
    {
        if(strcmp(temp.customer_personal_details.ID, ID) == 0)
        {
            found = 1;
        }
    }
    fclose(fp);
    return found;
}
