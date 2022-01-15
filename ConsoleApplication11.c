#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
typedef struct{
    int day;
    int month;
    int year;
}Date;

typedef struct{
    Date date;
    float amount;
    char text[40];
}deposite_withdraw;

typedef struct{
    int id;
    float money;
    int len_deposit;
    int len_withdraw;
    deposite_withdraw* deposit[250];
    deposite_withdraw* withdraw[750];
}Account;

typedef struct{
    char first_name[40];
    char last_name[40];
    int id;
    int len_accounts;
    Account* accounts[250];
}Client;

typedef struct{
    int len_clients;
    Client* clients[100];
}Bank;
int date_to_days(Date a) {
    return a.day + a.month * 31 + a.year * 365;
}
int is_date_between(Date current,Date a, Date b) {
    return date_to_days(current)>=date_to_days(a)&&date_to_days(current)<=date_to_days(b);
}
void clean_buff() {
    while (getchar() != '\n');
}
void func1_add_Client(Bank *b) {
    b->len_clients++;
    b->clients[b->len_clients - 1] = (Client*)malloc(sizeof(Client));
    puts("Enter client first name");
    gets((b->clients[b->len_clients - 1])->first_name);
    puts("Enter clien last name");
    gets((b->clients[b->len_clients - 1])->last_name);
    puts("Enter client id");
    scanf("%d", &((b->clients[b->len_clients - 1])->id));
    (b->clients[b->len_clients - 1])->len_accounts = 0;
    clean_buff();
}
Client* get_client(Bank* b, int id) {
    int i;
    for (i = 0; i < b->len_clients; i++)
        if (b->clients[i]->id == id)
            return b->clients[i];
    return NULL;
}
void func2_add_acoount(Bank* b) {
    int id,i;
    puts("Enter id of client");
    scanf("%d", &id);
    clean_buff();
    Client* run = get_client(b,id);
    if (run == NULL) {
        puts("No such id");
        return;
    }
    run->len_accounts++;
    run->accounts[run->len_accounts - 1] = (Account*)malloc(sizeof(Account));
    puts("Enter acoount id");
    scanf("%d", &id);
    clean_buff();
    run->accounts[run->len_accounts - 1]->id = id;
    run->accounts[run->len_accounts - 1]->len_deposit = 0;
    run->accounts[run->len_accounts - 1]->len_withdraw = 0;
    run->accounts[run->len_accounts - 1]->money = 0;
}
Account* get_account(Bank* b,int id) {
    int i, r;
    for (i = 0; i < b->len_clients; i++)
        for (r = 0; r < b->clients[i]->len_accounts; r++)
            if (b->clients[i]->accounts[r]->id == id)
                return b->clients[i]->accounts[r];
    return NULL;
}
void func3_deposit(Bank* b) {
    int id;
    puts("Enter account id");
    scanf("%d", &id);
    clean_buff();
    Account* run = get_account(b, id);
    if (run == NULL) {
        puts("NO such account id");
        return;
    }
    run->len_deposit++;
    run->deposit[run->len_deposit - 1] = (deposite_withdraw*)malloc(sizeof(deposite_withdraw));
    puts("Enter deposit amount");
    scanf("%f", &(run->deposit[run->len_deposit - 1]->amount));
    clean_buff();
    run->money += (run->deposit[run->len_deposit - 1]->amount);
    puts("Enter deposit name");
    gets((run->deposit[run->len_deposit-1]->text));
    puts("Enter Date day month year(with space between them)");
    scanf("%d %d %d", &(run->deposit[run->len_deposit - 1]->date.day),
        &(run->deposit[run->len_deposit - 1]->date.month), &(run->deposit[run->len_deposit - 1]->date.year));
}
void func4_withdraw(Bank* b) {
    int id;
    float amount;
    puts("Enter account id");
    scanf("%d", &id);
    clean_buff();
    Account* run = get_account(b, id);
    if (run == NULL) {
        puts("NO such account id");
        return;
    }
    run->len_withdraw++;
    run->withdraw[run->len_withdraw - 1] = (deposite_withdraw*)malloc(sizeof(deposite_withdraw));
    puts("Enter withdraw amount");
    scanf("%f", &amount);
    clean_buff();
    run->withdraw[run->len_withdraw - 1]->amount = amount;
    run->money -= amount;
    puts("Enter withdraw reason");
    gets((run->withdraw[run->len_withdraw - 1]->text));
    puts("Enter Date day month year(with space between them)");
    scanf("%d %d %d", &(run->withdraw[run->len_withdraw - 1]->date.day),
        &(run->withdraw[run->len_withdraw - 1]->date.month), &(run->withdraw[run->len_withdraw - 1]->date.year));
    clean_buff();
}
void func5_list_of_clients(Bank* b) {
    int i, r;
    for (i = 0; i < b->len_clients; i++) {
        printf("Client id: %d\tname: %s\tfamily: %s\n", b->clients[i]->id, b->clients[i]->first_name, b->clients[i]->last_name);
        for (r = 0; r < b->clients[i]->len_accounts; r++)
            printf("Account id: %d\t ammount: %.2f\n", b->clients[i]->accounts[r]->id, b->clients[i]->accounts[r]->money);
        putchar('\n');
    }
}
deposite_withdraw** list_of_dep_wi_between_dates(Account* c, Date a, Date b,int* len_result,int *flags) {
    int i;
    deposite_withdraw* result[1000];
    *len_result = 0;
    for (i = 0; i < c->len_deposit; i++)
        if (is_date_between(c->deposit[i]->date, a, b)) {
            (*len_result)++;
            flags[*len_result - 1] = 1;
            result[*len_result - 1] = c->deposit[i];
        }
    for (i = 0; i < c->len_withdraw; i++)
        if (is_date_between(c->withdraw[i]->date, a, b)) {
            (*len_result)++;
            flags[*len_result - 1] = 2;
            result[*len_result - 1] = c->withdraw[i];
        }
    return result;
}
void sort(deposite_withdraw** arr, int* flags, int len) {
    int i, r,temp2;
    deposite_withdraw* temp;
    for (i = 0; i < len - 1; i++)
        for (r = i + 1; r < len; r++)
            if (date_to_days(arr[i]->date) > date_to_days(arr[r]->date)) {
                temp = arr[i];
                arr[i] = arr[r];
                arr[r] = temp;
                temp2 = flags[i];
                flags[i] = flags[r];
                flags[r] = temp2;
            }
}
Client* get_client_by_acoount_id(Bank* b, int id) {
    int i, r;
    for (i = 0; i < b->len_clients; i++)
        for (r = 0; r < b->clients[i]->len_accounts; r++)
            if (b->clients[i]->accounts[r]->id == id)
                return b->clients[i];
    return NULL;

}
void func6_list_of_movements_account(Bank* bank) {
    int id;
    Date a;
    Date b;
    deposite_withdraw** arr;
    int len_arr,i;
    int flags[1000];
    float total;
    puts("Enter account id");
    scanf("%d", &id);
    clean_buff();
    Account* run = get_account(bank, id);
    if (run == NULL) {
        puts("NO such account id");
        return;
    }
    puts("Enter start date day month year(with space between them)");
    scanf("%d %d %d",&a.day,&a.month,&a.year);
    clean_buff();
    puts("Enter end date day month year(with space between them)");
    scanf("%d %d %d", &b.day, &b.month, &b.year);
    clean_buff();
    arr = list_of_dep_wi_between_dates(run, a, b, &len_arr, flags);
    if (len_arr == 0) {
        puts("No such movements between dates");
        return;
    }
    sort(arr, flags, len_arr);
    Client* run2 = get_client_by_acoount_id(bank,run->id);
    printf("\nName: %s\tFamily: %s\tAccount id: %d\tMovements: %d\tTotal:%.2f\n", run2->first_name, run2->last_name, run->id, len_arr,run->money);
    puts("Date    |Deposit|Withdraw|Total");
    total = run->money;
    for (i = 0; i < len_arr; i++) {
        if (flags[i] == 1)
            total -= arr[i]->amount;
        else
            total += arr[i]->amount;
    }
    for (i = 0; i < len_arr; i++) {
        if (flags[i] == 1)
            total += arr[i]->amount;
        else
            total -= arr[i]->amount;
        printf("%d/%d/%d|%.2lf|%.2f|%.2f\n", arr[i]->date.day, arr[i]->date.month, arr[i]->date.year,
            (flags[i] == 1) ? arr[i]->amount : 0, (flags[i] == 2) ? arr[i]->amount : 0,total);
    }
}
void run() {
    Bank* bank = (Bank*)malloc(sizeof(Bank));
    bank->len_clients = 0;
    int chose;
    do {
        puts("\n1 - Add Client\t2 - Add Account \t3 - Deposit");
        puts("4 - Withdraw  \t5 - List Clients\t6 - Page of Account");
        puts("Other exit");
        scanf("%d", &chose);
        clean_buff();
        switch (chose) {
            case 1:
                func1_add_Client(bank);
                break;
            case 2:
                func2_add_acoount(bank);
                break;
            case 3:
                func3_deposit(bank);
                break;
            case 4:
                func4_withdraw(bank);
                break;
            case 5:
                func5_list_of_clients(bank);
                break;
            case 6:
                func6_list_of_movements_account(bank);
                break;
        }
    } while (chose < 7 && chose>0);
}
void main(){
    run();
    puts("Good Bye");
    system("pause");
}
