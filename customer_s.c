#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "project_h.h"
#define LINE printf("-----------------------------------\n");


int customer_login() // returns to login()
{
    customer c;
    char ID[20], password[20];
    LINE
    printf("Enter your ID: ");
    scanf("%s", ID);
    int ID_found = 0, flag = 0;
    FILE *fp;
    fp = fopen("CUSTOMERS.DAT", "rb");
    if(fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    while(fread(&c, sizeof(c), 1, fp))
    {
        if(strcmp(c.customer_personal_details.ID, ID) == 0)
        {
            ID_found = 1;
            break;
        }
    }
    fclose(fp);
    if(ID_found == 0)
    {
        printf("\nID not found, please create an account");
    }
    else
    {
        char temp[20] = "\0";
        char* temp2 = decrypt(c.customer_personal_details.password, temp);
        for(int i = 0; i < 3; ++i)
        {
            printf("\nEnter password: ");
            scanf("%s", password);
            // remove this later
            // if(strcmp(password, "abc") == 0)
            // {
            //     flag = 1;
            //     break;
            // }
            //
            if(strcmp(temp2, password))
            {
                if(i == 2)
                {
                    printf("\nMaximum attempts reached. Your account is locked. Please contact your branch.");
                    break;
                }
                printf("\nIncorrect Password, try again");
            }
            else
            {
                if(c.ac_status == 0)
                {
                    printf("Sorry, your account has not been activated. Please contact your branch.\n");
                    break;
                }
                else 
                {
                    flag = 1;
                    break;
                }
            }
        }
    }
    if(flag == 1)
    {
        printf("\nSuccessfully Logged In\n\n");
        customer_menu(&c);
    }
    return 0;
}


int customer_details()
{
    //Initializing requirements of customer file
    FILE* fpointer = fopen("CUSTOMERS.dat", "rb");
    if(fpointer == NULL)
    {
        printf("File not found\n");
    }
    else
    {
        fseek(fpointer, 0, SEEK_END);
        long size1 = ftell(fpointer);
        fseek(fpointer, 0, SEEK_SET);
        customer cd[size1 / sizeof(customer)];
        fread(cd, sizeof(customer), size1 / sizeof(customer), fpointer);
        fclose(fpointer);


        char response = 'n';
        printf("Sl.no\tID\t\tAccount no.\t\tIFSC CODE\t\tBalance(in Rs.)\t\tUPI Handler\tType\n");
        for(int i = 0; i < (size1 / sizeof(customer)); i++)
        {
            printf("%d\t%s\t\t%s\t\t\t%s\t\t%0.2f\t\t\t%s\t\t%s\n", i + 1, cd[i].customer_personal_details.ID, cd[i].ac_no, cd[i].ifsc_code, cd[i].balance, cd[i].upi_handler, cd[i].type);
        }
    }
    return 0;
}


int customer_menu(customer* c) // returns to customer_login()
{
    LINE
    printf("Welcome %s\n", c -> customer_personal_details.ID);
    int ch;
    do
    {
        LINE
        printf("1. New Loan\n2. View Loan Status\n3. Make a transaction\n4. View previous transcation details\n5. Change password\n6. View Balance\n");
        printf("7. Withdraw Money\n8. Send Feedback\n9. Make a deposit\n10. View deposit records\n0. Logout\n");
        scanf("%d", &ch);
        switch (ch)
        {
            case 1:
                new_loan(c);
                break;
            case 2:
                view_loan_status(c);
                break;
            case 3:
                make_transaction(c);
                break;
            case 4:
                customer_transactions(c);
                break;
            case 5:
                change_password(c);
                break;
            case 6:
                LINE
                printf("Your balance is: Rs %0.2lf\n", c->balance);
                break;
            case 7:
                withdraw_money(c);
                break;
            case 8:
                send_feedback(c);
                break;
            case 9:
                make_deposit(c);
                break;
            case 10:
                customer_deposits(c);
                break;
            case 0:
                break;  
            default:
                printf("Invalid Input");
                break;
        }
    }while(ch != 0);
}


int new_loan(customer* c) // returns to customer_menu()
{
    int ch;
    LINE
    printf("1. Car Loan\n2. Home Loan\n3. Education Loan\n4. Medical Loan\n5. Other loan\n0. Exit\nEnter your choice: ");
    scanf("%d", &ch);
    float rate_of_interest;
    switch (ch)
    {
        case 1:
            rate_of_interest = 5.5;
            break;
        case 2:
            rate_of_interest = 5.9;
            break;
        case 3:
            rate_of_interest = 4.5;
            break;
        case 4:
            rate_of_interest = 2.5;
            break;
        case 5:
            printf("Please contact your branch for the necessary details\n");
            break;
        case 0:
            return 0;
            break;
        default:
            printf("Invalid Input\n");
            break;
    }
    printf("The rate of interest for this type of a loan is: %0.2f\n", rate_of_interest);
    loan_application(c, rate_of_interest, ch);
}


int loan_application(customer* c, float rate_of_interest, int type) // returns to new_loan
{
    LINE
    double requested_amount;
    double requested_duration;
    printf("Enter the amount you want to take a loan for: ");
    scanf("%lf", &requested_amount);
    c -> loan.loan_status = 0;
    c -> loan.rate_of_interest = rate_of_interest;
    printf("Enter the duration in years: ");
    scanf("%lf", &requested_duration);
    c -> loan.duration = requested_duration;
    double amount = requested_amount * pow((1 + (rate_of_interest / 100)), requested_duration);
    printf("The final amount will be: %0.2lf\n", amount);
    c -> loan.principal_amount = requested_amount;
    printf("Loan Application submitted successfully\n");
    c -> loan.loan_status = 2;
    update_records(c);
}


int view_loan_status(const customer* c) // returns to customer_menu()
{
    LINE
    if(c -> loan.loan_status == 1)
    {
        printf("Your loan application has been approved\n");
    }
    else if(c -> loan.loan_status == 0)
    {
        printf("You have not applied for any loan\n");
    }
    else if(c -> loan.loan_status == 2)
    {
        printf("Your loan application is still being processed\n");
    }
    else
    {
        printf("Your loan application has been rejected, please contact your branch\n");
    }
}


// int transaction(customer* c)
// {
    
// }


// int transaction_details(const customer* c)
// {
    
// }


void update_records(customer* c) // returns to customer_menu()
{
    FILE *fp1, *fp2;
    fp1 = fopen("CUSTOMERS.DAT", "rb");
    fp2 = fopen("TEMP.DAT", "wb");
    customer temp;
    while(fread(&temp, sizeof(customer), 1, fp1))
    {
        if(strcmp(temp.customer_personal_details.ID, c -> customer_personal_details.ID))
        {
            fwrite(&temp, sizeof(customer), 1, fp2);
        }
    }
    fwrite(c, sizeof(customer), 1, fp2);
    fclose(fp1);
    fclose(fp2);
    remove("CUSTOMERS.DAT");
    rename("TEMP.DAT", "CUSTOMERS.DAT");
    //printf("Records successfully updated (to test if function is working)\n");
}


/*void update_records(customer* c)
{
    FILE *fp;
    fp = fopen("CUSTOMERS.DAT", "ab");
    fseek(fp, (c -> customer_no) * sizeof(customer), SEEK_SET);
    fwrite(c, sizeof(customer), 1, fp);
    fclose(fp);
    printf("Records successfully updated (to test if function is working)\n");
}*/


void encrypt(char* string_to_be_encrypted)
{
    char key[10];
    int sum = find_time(key);
    for(int i = 0; i < strlen(string_to_be_encrypted); ++i)
    {
        if((int)(string_to_be_encrypted[i]) + sum != 10 || (int)(string_to_be_encrypted[i]) + sum - 256 != 10)
        {
            if((int)(string_to_be_encrypted[i]) + sum <= 255)
            {
                string_to_be_encrypted[i] = (char)((int)(string_to_be_encrypted[i]) + sum);
            }
            else
            {
                string_to_be_encrypted[i] = (char)((int)(string_to_be_encrypted[i]) + sum - 256);
            }
        }
        //(or) add this
        // if(string_to_be_encrypted[i] == '\n')
        // {
        //     if((int)(string_to_be_encrypted[i]) - sum <= 255)
        //     {
        //         string_to_be_encrypted[i] = (char)((int)(string_to_be_encrypted[i]) - sum + 256);
        //     }   
        //     else
        //     {
        //         string_to_be_encrypted[i] = (char)((int)(string_to_be_encrypted[i]) - sum);
        //     }
        // }
    }
    strcat(string_to_be_encrypted, key);
}


char* decrypt(const char* str, char* string_to_be_decrypted)
{
    char key[10];
    strcpy(string_to_be_decrypted, str);
    int i;
    int location_of_key = strlen(string_to_be_decrypted) - 8;
    for(i = 0; i < 8; ++i)
    {
        key[i] = string_to_be_decrypted[location_of_key + i];
    }
    key[i] = '\0';
    string_to_be_decrypted[location_of_key] = '\0';
    int h, m, s;
    h = atoi(strtok(key, ":"));
    m = atoi(strtok(NULL, ":"));
    s = atoi(strtok(NULL, ":"));
    int sum = h + m + s;
    for(int i = 0; i < strlen(string_to_be_decrypted); ++i)
    {
        if(string_to_be_decrypted[i] == '\n')
            continue;   
        else if((int)(string_to_be_decrypted[i]) - sum <= 255)
        {
            string_to_be_decrypted[i] = (char)((int)(string_to_be_decrypted[i]) - sum + 256);
        }
        else
        {
            string_to_be_decrypted[i] = (char)((int)(string_to_be_decrypted[i]) - sum);
        }
    }
    return string_to_be_decrypted;
}


int update_transaction_details(customer* c, char* recipient_ID)
{
    FILE *fp;
    char s[300] = "\0";
    strcpy(s, c -> customer_personal_details.ID);
    strcat(s, ", to ");
    strcat(s, recipient_ID);
    strcat(s, ", Date: ");
    fp = fopen("transaction_details.txt", "a+");
    if(fp == NULL)
    {
        printf("File not found\n");
        return 1;
    }
    strcpy(c -> transactions.date, __DATE__);
    char current_time[20];
    find_time(current_time);
    strcpy(c -> transactions.time, current_time);
    c -> transactions.transaction_count++;
    strcat(s, c -> transactions.date);
    strcat(s, ", Time: ");
    strcat(s, c -> transactions.time);
    strcat(s, ", Amount: ");
    //c -> transactions.transaction_type
    char temp[20];
    int size_of_amount = 0;
    long amt = c -> transactions.amount;
    while(amt > 0)
    {
        amt /= 10;
        size_of_amount++;
    }
    gcvt(c -> transactions.amount, size_of_amount + 2, temp);
    strcat(s, "Rs ");
    strcat(s, temp);
    encrypt(s);
    fputs(s, fp);
    fputs("\n", fp);
    fclose(fp);
}


int make_transaction(customer* c)
{
    LINE
    int ch;
    printf("1. NEFT\n2. Fund transfer\n");
    scanf("%d", &ch);
    switch(ch)
    {
        case 1:
            neft(c);
            break;
        case 2:
            fund_transfer(c);
            break;
        default:
            printf("Invalid input\n");
            break;
    }
}


int change_password(customer* c)
{
    char new_password[40], temp[40];
    int flag = 0;
    int ch;
    printf("1. Change Bank Account Password\n2. Change UPI Password\n");
    scanf("%d", &ch);
    if(ch == 1 || ch == 2)
    {
        do
        {
            printf("Enter new password: ");
            scanf("%s", new_password);
            printf("Confirm password: ");
            scanf("%s", temp);
            if(strcmp(new_password, temp))
            {
                printf("Passwords do not match, try again\n");
            }
            else
            {
                flag = 1;
            }
        }while(flag == 0);
        encrypt(new_password);
        if(ch == 1)
        {
            strcpy(c -> customer_personal_details.password, new_password);
            update_records(c);
            printf("Password changed successfully\n");
        }
        else
        {
            strcpy(c -> upi_passcode, new_password);
            update_records(c);
            printf("Password changed successfully\n");
        }
    }
    else
    {
        printf("Invalid Input\n");
    }
    return 0;
}


int find_time(char* key)
{
    time_t Time;
    struct tm* current_time;
    Time = time(NULL);
    current_time = localtime(&Time);
    int sum = current_time -> tm_hour + current_time -> tm_min + current_time -> tm_sec;
    printf("sum = %d\n", sum);
    char hour[5], min[5], sec[5];
    itoa(current_time -> tm_hour, hour, 10);
    itoa(current_time -> tm_min, min, 10);
    itoa(current_time -> tm_sec, sec, 10);
    if(strlen(hour) == 1)
    {
        hour[1] = hour[0];
        hour[0] = '0';
        hour[2] = '\0';
    }
    if(strlen(min) == 1)
    {
        min[1] = min[0];
        min[0] = '0';
        min[2] = '\0';
    }
    if(strlen(sec) == 1)
    {
        sec[1] = sec[0];
        sec[0] = '0';
        sec[2] = '\0';
    }
    strcpy(key, hour);
    strcat(key, ":");
    strcat(key, min);
    strcat(key, ":");
    strcat(key, sec);
    printf("key = %s\n", key);
    return sum;
}


void withdraw_money(customer* c)
{
    LINE
    double withdrawal_amount;
    printf("Enter the amount of money you want to withdraw: ");
    scanf("%lf", &withdrawal_amount);
    if(withdrawal_amount > c -> balance)
    {
        printf("Insufficent Balance\n");
    }
    else if(withdrawal_amount > 10000 && strcmpi(c -> type, "savings") == 0)
    {
        printf("Cannot withdraw such a high amount from savings account\n");
    }
    else
    {
        char password[20], temp[20] = "\0", *temp1;
        int flag = 0;
        printf("Enter password to confirm transaction: ");
        scanf("%s", password);
        temp1 = decrypt(c -> customer_personal_details.password, temp);
        if(strcmp(temp1, password) == 0)
        {
            c -> balance -= withdrawal_amount;
            flag = 1;
            printf("Withdrawal completed successfully\n");
            update_records(c);
        }
        else
        {
            printf("Incorrect password\n");
        }
    }
}


void send_feedback(customer* c)
{
    LINE    
    FILE *fp;
    fp = fopen("feedback.txt", "a");
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    char feedback[500], ch;
    printf("Enter your feedback\nEnter ctrl + z and press enter when you are done\n");
    fputs(c ->customer_personal_details.ID, fp);
    fputs(": ", fp);
    while((ch = getchar()) != EOF)
    {
        putc(ch, fp);
    }
    fputs("\n", fp);
    fclose(fp);
    printf("Thank you for your feedback\n");
}


void make_deposit(customer* c)
{
    LINE
    double deposit_amount;
    printf("Enter the amount you want to deposit: ");
    scanf("%lf", &deposit_amount);
    c -> balance += deposit_amount;
    update_deposit_records(c, deposit_amount);
    update_records(c);
    printf("Amount Successfully Deposited\n");
}


void update_deposit_records(customer* c, double deposit_amount)
{
    FILE *fp;
    fp = fopen("deposit_records.txt", "a");
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    char str[100];
    strcpy(str, c -> customer_personal_details.ID);
    strcat(str, ", Date: ");
    strcat(str, __DATE__);
    char time[20];
    find_time(time);
    strcat(str, ", Time: ");
    strcat(str, time);
    strcat(str, ", Amount: ");
    int size1 = 0;
    int temp = deposit_amount;
    while(temp > 0)
    {
        temp /= 10;
        size1++;
    }
    char amount[20];
    gcvt(deposit_amount, size1 + 2, amount);
    strcat(str, amount);
    printf("%s\n", str);
    encrypt(str);
    fputs(str, fp);
    fputs("\n", fp);
    fclose(fp);
    printf("Deposit records updated\n");
}


void customer_transactions(customer* c)
{
    FILE *fp;
    fp = fopen("transaction_details.txt", "r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    char s[400], temp[400] = "\0";
    LINE
    while(fgets(s, 400, fp) != NULL)
    {
        s[strlen(s) - 1] = '\0';
        char* temp1 = decrypt(s, temp);
        char temp2[400];
        strcpy(temp2, temp1);
        char* ID1 = strtok(temp1, ", ");
        strtok(NULL, " ");
        char* ID2 = strtok(NULL, ", ");
        if(strcmp(ID1, c -> customer_personal_details.ID) == 0 || strcmp(ID2, c -> customer_personal_details.ID) == 0)
        {
            printf("%s\n", temp2);
        }
    }
    fclose(fp);
}


// started here on 16 08 21
// int apply_for_upi(customer* c)
// {
//     printf("");
// }


void customer_deposits(customer* c)
{
    FILE *fp;
    fp = fopen("deposit_records.txt", "r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    char *ID, s[300], temp[300];
    LINE
    while(fgets(s, 300, fp) != NULL)
    {
        s[strlen(s) - 1] = '\0';
        char* temp1 = decrypt(s, temp);
        char temp2[100];
        strcpy(temp2, temp1);
        ID = strtok(temp1, ",");
        if(strcmp(ID, c -> customer_personal_details.ID) == 0)
        {
            printf("%s\n", temp2);
        }
    }
    fclose(fp);
}


void fund_transfer(customer* c)
{
    LINE
    char recipient_ID[10];
    printf("Enter the ID of the receiving customer: ");
    scanf("%s", recipient_ID);
    FILE *fp;
    fp = fopen("CUSTOMERS.DAT", "r");
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    customer recipient;
    int found = 0;
    while(fread(&recipient, sizeof(customer), 1, fp))
    {
        if(strcmp(recipient.customer_personal_details.ID, recipient_ID) == 0)
        {
            found = 1;
            break;
        }
    }
    fclose(fp);
    if(found == 0)
    {
        printf("Customer not found\n");
    }
    else
    {
        double amount_to_be_sent;
        printf("Enter the amount you wish to send: ");
        scanf("%lf", &amount_to_be_sent);
        if(c -> balance < amount_to_be_sent)
        {
            printf("Insufficient Balance\n");
        }
        else
        {
            char pin[20], *temp1, temp2[20] = "\0";
            int count = 0;
            do
            {
                printf("Enter your pin: ");
                scanf("%s", pin);
                temp1 = decrypt(c ->upi_passcode, temp2);
                if(strcmp(pin, temp1) == 0)
                {
                    c -> balance -= amount_to_be_sent;
                    recipient.balance += amount_to_be_sent;
                    c -> transactions.amount = amount_to_be_sent;
                    update_transaction_details(c, recipient_ID);
                    update_records(&recipient);
                    update_records(c);
                    printf("Transaction successfully completed\n");
                    break;
                }
                else
                {
                    printf("Incorrect pin, try again\n");
                    count++;
                }
                if(count == 3)
                {
                    printf("Maximum number of attempts reached, please try again later.\n");
                }
            }while(count != 3);
        }
    }
}


void neft(customer* c)
{
    LINE
    char recipient_ID[20], recipient_IFSC[20], recipient_ac_no[20];
    printf("Enter the ID of the recipient: ");
    scanf("%s", recipient_ID);
    printf("Enter the IFSC code of the recipient: ");
    scanf("%s", recipient_IFSC);
    printf("Enter the account number of the recipient: ");
    scanf("%s", recipient_ac_no);
    srand(time(0));
    int otp = rand();
    int temp_otp;
    printf("This is your OTP: %d\n", otp);
    int count = 0;
    do
    {
        printf("Enter OTP to activate payee: ");
        scanf("%d", &temp_otp);
        if(temp_otp != otp)
        {
            printf("Incorrect OTP, try again\n");
            count++;
        }
        if(count == 3)
        {
            printf("Too many unsuccessful attempts\n");
            return;
        }
    }while(temp_otp != otp);


    if(temp_otp == otp)
    {
        printf("Payee Activated\n");
        double amount;
        printf("Enter the amount to be transfered: ");
        scanf("%lf", &amount);
        if(amount > c -> balance)
        {
            printf("Insufficient balance\n");
            return;
        }
        else if(amount > 200000)
        {
            printf("Sorry, amount is too large\n");
            return;
        }
        srand(time(0));
        otp = rand();
        printf("Your OTP is: %d\n", otp);
        int count = 0;
        do
        {
            printf("Enter your OTP to authenticate the transaction: ");
            scanf("%d", &temp_otp);
            if(temp_otp != otp)
            {
                printf("Incorrect OTP, try again\n");
                count++;
            }
            if(count == 3)
            {
                printf("Too many unsuccessful attempts\n");
                return;
            }
        }while(temp_otp != otp);
        if(temp_otp == otp)
        {
            c -> balance -= amount;
            srand(time(0));
            int reference_no = rand();
            printf("The reference number is: %d\n", reference_no);
            update_neft_transaction_details(c, recipient_ID, recipient_IFSC, recipient_ac_no, reference_no, amount);
            update_records(c);
            printf("Transaction Successful\n");
        }
    }
    else
    {
        printf("Incorrect OTP\n");
    }
}


void update_neft_transaction_details(customer* c, char* recipient_ID, char* recipient_IFSC, char* recipient_ac_no, int reference_no, double amount)
{
    FILE *fp;
    fp = fopen("transaction_details.txt", "a");
    if(fp == NULL)
    {
        printf("File not found\n");
        return;
    }
    char s[300];
    strcpy(s, c ->customer_personal_details.ID);
    strcat(s, ", to ");
    strcat(s, recipient_ID);
    strcat(s, ", NEFT, Date: ");
    strcat(s, __DATE__);
    strcat(s, ", Time: ");
    char Time[20];
    find_time(Time);
    strcat(s, Time);
    strcat(s, ", Amount: ");
    char amt[20];
    itoa(amount, amt, 10);
    strcat(s, amt);
    strcat(s, ", IFSC: ");
    strcat(s, recipient_IFSC);
    strcat(s, ", AC NO: ");
    strcat(s, recipient_ac_no);
    strcat(s, ", REF NO: ");
    char refno[10];
    itoa(reference_no, refno, 10);
    strcat(s, refno);
    puts(s);
    encrypt(s);
    fputs(s, fp);
    fputs("\n", fp);
    printf("NEFT records updated\n");
    fclose(fp);
}