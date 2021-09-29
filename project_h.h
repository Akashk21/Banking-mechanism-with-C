typedef struct branch_details
{
    char branch[5];
    float total_deposit;
    int no_of_accounts;
    int loans_granted;
    int customer_ID;
}branch_details;
typedef struct transcation_details
{
    char date[20];
    char time[20];
    double amount;
    char transaction_type[10];
    int transaction_count;
}transaction_details;
 
typedef struct loan_details
{
    char type[20];
    double principal_amount;
    int duration;
    float rate_of_interest;
    int loan_status;
}loan_details;
 
typedef struct personal_details
{
    char name[20];
    char ID[20];
    char password[40];
}personal_details;
 
typedef struct customer
{
    personal_details customer_personal_details;
char ac_no[20];
    char ifsc_code[10];
    float balance;
    char upi_handler[20];
    char upi_passcode[20];
    int ac_status;
    char type[15];
    char branch[5];
    int customer_no;
    transaction_details transactions;
    loan_details loan;
}customer;
 
struct customer_t
{
    customer c;
    struct customer_t* node;
};
typedef struct customer_t customer_t;
#define size 500
 
struct employee_details
{
    char name[20];
    char ID[20];
    char password[20];
    char role[20];
    char branch[20];
};
 
struct employee
{
    struct employee_details ed;
    struct employee* node;
};
typedef struct bank_details
{
    float rate_sb;
    float rate_rd;
 int update_count;
}bank_details;
typedef struct customer customer;
typedef struct employee employee;
#define LINE printf("--------------------------------------------------------\n");
int employee_login();
void create_upi_handler(customer*);
void generate_ac_no(customer*);
void login();
void sign_up();
int ID_exists(char*);
int employee_details();
int manager(char* branch);
int cashier(char* branch);
int view_customers(char* man_branch);
int block(char* branch,branch_details load[],int flag);
int verify_applicants(char* man_branch,branch_details load[],int flag);
int search_customers(customer_t* hash_table[],char* man_branch);
int approve_loan_applicants(char* man_branch,branch_details load[],int flag);
void division_manager(employee* head);
int add_employees(struct employee_details,employee*);
void remove_employee(char* emp_id,char* branch,employee* head);
void display();
int customer_login();
int customer_details();
int customer_menu(customer*);
int loan_application(customer* , float, int);
int new_loan(customer*);
void update_records(customer*);
int view_loan_status(const customer*);
void encrypt(char*);
char* decrypt(const char*, char*);
int make_transaction(customer*);
int update_transaction_details(customer*, char*);
int change_password(customer*);
int find_time(char*);
void withdraw_money(customer*);
void send_feedback(customer*);
void make_deposit(customer*);
void customer_transactions(customer*);
int apply_for_upi(customer*);
void update_deposit_records(customer*, double);
void customer_deposits(customer*);
void fund_transfer(customer*);
void neft(customer*);
void update_neft_transaction_details(customer*, char*, char*, char*, int, double);
void view_details(char);
void view_feedback();
