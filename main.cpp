#include <iostream>
#include <cstring>
#include <conio.h>
#include <limits>
#include <fstream>
using namespace std;

struct Transaction {
    int sender;
    int receiver;
    double amount;
};


struct Account
{
    int account_no;
    char phone[12];         
    char username[30];
    char password[30];
    char email[50];
    double balance;
    bool closed;
    double receivedMoney = 0;
    Transaction history[100];
    int historyCnt = 0;

};

int capacity = 5;
int curr_accounts = 0;
int next_acc = 1;
Account *acc = nullptr;
const char adminpass[] = "admin123";
const char DATA_FILE[] = "accounts.dat";


void clearInput()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isValidEmail(const char email[])
{
    int len = (int)strlen(email);
    if (len < 5) return false;  
    int atCount = 0,
     atPos = -1;
    for (int i = 0; i < len; ++i) {
        if (email[i] == ' ')
            return false;
        if (email[i] == '@') { 
            ++atCount; atPos = i; }
    }
    if (atCount != 1)
        return false;
    if (atPos <= 0 || atPos >= len - 1) 
        return false;
    bool dotAfter = false;
    for (int i = atPos + 1; i < len; ++i) 
        if (email[i] == '.') 
            dotAfter = true;
    return dotAfter;
}

bool isEmailUnique(const char email[])
{
    for (int i = 0; i < curr_accounts; ++i) {
        if (!acc[i].closed && strcmp(acc[i].email, email) == 0) 
            return false;
    }
    return true;
}

bool isValidPhone(const char ph[])
{
    int len = (int)strlen(ph);
    if (len != 11) 
        return false;
    if (ph[0] != '0' || ph[1] != '3') 
        return false;
    for (int i = 0; i < len; ++i) 
        if (!isdigit((unsigned char)ph[i])) 
            return false;
    return true;
}


char *getPassword(char pass_word[], int maxLen)
{
    int idx = 0;
    while (true)
    {
        int ch = _getch();
        if (ch == 13) break; 
        if (ch == 8) {      
            if (idx > 0) {
                idx--;
                cout << "\b \b";
            }
        } else if (ch >= 32 && ch <= 126 && idx < maxLen - 1) {
            pass_word[idx++] = (char)ch;
            cout << "*";
        }
    }
    pass_word[idx] = '\0';
    return pass_word;
}


void grow_capacity()
{
    int newcap = capacity * 2;
    Account *temp = new Account[newcap];
    for (int i = 0; i < curr_accounts; ++i) 
        temp[i] = acc[i];
    delete[] acc;
    acc = temp;
    capacity = newcap;
}

int findAccountNo(int acc_no)
{
    for (int i = 0; i < curr_accounts; ++i)
        if (!acc[i].closed && acc[i].account_no == acc_no)
            return i;
    return -1;
}


void saveToFile()
{
    ofstream fout(DATA_FILE, ios::binary | ios::out | ios::trunc);
    if (!fout) {
        cerr << "Error: cannot open file to save data.\n";
        return;
    }

    fout.write((char*)&curr_accounts, sizeof(curr_accounts));
    fout.write((char*)&capacity, sizeof(capacity));
    fout.write((char*)&next_acc, sizeof(next_acc));

    for (int i = 0; i < curr_accounts; ++i) {
        fout.write((char*)&acc[i].account_no, sizeof(acc[i].account_no));
        fout.write(acc[i].phone, sizeof(acc[i].phone));
        fout.write(acc[i].username, sizeof(acc[i].username));
        fout.write(acc[i].password, sizeof(acc[i].password));
        fout.write(acc[i].email, sizeof(acc[i].email));
        fout.write((char*)&acc[i].balance, sizeof(acc[i].balance));
        fout.write((char*)&acc[i].closed, sizeof(acc[i].closed));
        fout.write((char*)&acc[i].receivedMoney, sizeof(acc[i].receivedMoney));
        fout.write((char*)&acc[i].historyCnt, sizeof(acc[i].historyCnt));
        for (int j = 0; j < acc[i].historyCnt; j++) {
            fout.write((char*)&acc[i].history[j], sizeof(Transaction));
        }
    }
    fout.close();
}

void loadFromFile()
{
    ifstream fin(DATA_FILE, ios::binary);
    if (!fin) {
        acc = new Account[capacity];
        curr_accounts = 0;
        next_acc = 1;
        return;
    }

    int file_count = 0;
    fin.read((char*)&file_count, sizeof(file_count));
    if (!fin) { 
        fin.close(); 
        acc = new Account[capacity]; 
        curr_accounts = 0; 
        next_acc = 1; 
        return; }

    int file_capacity = 0;
    fin.read((char*)&file_capacity, sizeof(file_capacity));
    fin.read((char*)&next_acc, sizeof(next_acc));

    // allocate
    int allocCap = max(file_capacity, capacity);
    delete[] acc;
    acc = new Account[allocCap];
    capacity = allocCap;

    curr_accounts = 0;
    for (int i = 0; i < file_count; ++i) {
        Account a;
        fin.read((char*)&a.account_no, sizeof(a.account_no));
        fin.read(a.phone, sizeof(a.phone));
        a.phone[sizeof(a.phone)-1] = '\0';
        fin.read(a.username, sizeof(a.username));
        a.username[sizeof(a.username)-1] = '\0';
        fin.read(a.password, sizeof(a.password));
        a.password[sizeof(a.password)-1] = '\0';
        fin.read(a.email, sizeof(a.email));
        a.email[sizeof(a.email)-1] = '\0';
        fin.read((char*)&a.balance, sizeof(a.balance));
        fin.read((char*)&a.closed, sizeof(a.closed));
        fin.read((char*)&a.receivedMoney, sizeof(a.receivedMoney));
        acc[curr_accounts] = a;        
        fin.read((char*)&acc[curr_accounts].historyCnt, sizeof(acc[curr_accounts].historyCnt));
        for (int j = 0; j < acc[curr_accounts].historyCnt; j++) {
            fin.read((char*)&acc[curr_accounts].history[j], sizeof(Transaction));
        }

        curr_accounts++;
    }
    fin.close();
}


void showAccountInfo(int i)
{
    if (i < 0 || i >= curr_accounts) { cout << "Invalid account index.\n"; return; }
    cout << "\n-------------------------------\n";
    cout << "Account No : " << acc[i].account_no << endl;
    cout << "Username   : " << acc[i].username << endl;
    cout << "Email      : " << acc[i].email << endl;
    cout << "Phone      : " << acc[i].phone << endl;
    cout << "Balance    : " << acc[i].balance << endl;
    cout << "Status     : " << (acc[i].closed ? "Closed" : "Open") << endl;
    cout << "-------------------------------\n";
}

void depositToAccount(int i)
{
    if (i < 0 || i >= curr_accounts) { 
        cout << "Invalid account.\n"; 
        return; }

    if (acc[i].closed) { 
        cout << "Account is closed.\n"; 
        return; }

    double amt;
    cout << "Enter amount to deposit: ";
    if (!(cin >> amt)) { 
        clearInput(); 
        cout<<"Invalid input.\n"; 
        return; }

    if (amt <= 0) { 
        cout << "Invalid amount.\n"; 
        return; }

    acc[i].balance += amt;
    saveToFile();
    cout << "Deposit successful. New balance: " << acc[i].balance << endl;
}

void withdrawFromAccount(int i)
{
    if (i < 0 || i >= curr_accounts) { 
        cout << "Invalid account.\n"; 
        return; }
    if (acc[i].closed) { 
        cout << "Account is closed.\n"; 
        return; }

    double amt;
    cout << "Enter amount to withdraw: ";
    if (!(cin >> amt)) { 
        clearInput(); 
        cout<<"Invalid input.\n"; 
        return; }
    if (amt <= 0) { 
        cout << "Invalid amount.\n"; 
        return; }
    if (amt > acc[i].balance) { 
        cout << "Insufficient balance.\n"; 
        return; }

    acc[i].balance -= amt;
    saveToFile();
    cout << "Withdraw successful. New balance: " << acc[i].balance << endl;
}


void create(int idx)
{
    clearInput();

    cout << "Assigned Account No: " << acc[idx].account_no << endl;

    while (true) {
        cout << "Enter Username : ";
        cin.getline(acc[idx].username, sizeof(acc[idx].username));
        if (strlen(acc[idx].username) == 0) { 
            cout << "Username cannot be empty.\n"; 
            continue; }
        break;
    }

    char mail[50];
    while (true) {
        cout << "Enter Email: ";
        cin.getline(mail, sizeof(mail));
        if (!isValidEmail(mail)) { 
            cout << "Invalid email format. Try again.\n"; 
            continue; }
        if (!isEmailUnique(mail)) { 
            cout << "Email already exists! Enter a different email.\n"; 
            continue; }
        strcpy(acc[idx].email, mail);
        break;
    }

    char ph[20];
    while (true) {
        cout << "Enter Phone (11 digits, starting with 03): ";
        cin.getline(ph, sizeof(ph));
        if (!isValidPhone(ph)) { 
            cout << "Invalid phone format. Try again.\n"; 
            continue; }
        strcpy(acc[idx].phone, ph);
        break;
    }

    cout << "Enter Password: ";
    getPassword(acc[idx].password, (int)sizeof(acc[idx].password));
    cout << endl;
}

void Signup()
{
    if (curr_accounts == capacity) grow_capacity();
    int i = curr_accounts;
    acc[i].account_no = next_acc++;
    acc[i].balance = 0.0;
    acc[i].closed = false;
    acc[i].receivedMoney = 0.0;
    acc[i].username[0] = acc[i].email[0] = acc[i].password[0] = '\0';
    acc[i].phone[0] = '\0';

    cout << "\n----------- SIGNUP -----------\n";
    create(i);
    curr_accounts++;
    saveToFile();
    cout << "Signup successful! Your Account No: " << acc[i].account_no << "\n";
}


int customerLogin()
{
    cout << "\n----------- LOGIN -----------\n";
    int accNo;
    cout << "Enter Account No: ";
    if (!(cin >> accNo)) { 
        clearInput(); 
        cout << "Invalid input.\n"; 
        return -1; }
    clearInput();

    char pass[30];
    cout << "Enter Password: ";
    getPassword(pass, (int)sizeof(pass));
    cout << endl;

    int idx = findAccountNo(accNo);
    if (idx == -1) { 
        cout << "Account not found or closed.\n"; 
        return -1; }
    if (strcmp(acc[idx].password, pass) != 0) { 
        cout << "Incorrect password.\n"; 
        return -1; }
    cout << "Login successful!\n";
    return idx;
}


void sendMoney(int senderidx)
{
    if (senderidx < 0 || senderidx >= curr_accounts) { 
        cout << "Invalid sender.\n"; 
        return; }
    if (acc[senderidx].closed) { 
        cout << "Account is closed.\n"; 
        return; }

    int receiverAcc;
    double amount;
    cout << "Enter receiver account number: ";
    if (!(cin >> receiverAcc)) { 
        clearInput(); 
        cout << "Invalid input.\n"; 
        return; }
    int receiverIndex = findAccountNo(receiverAcc);
    if (receiverIndex == -1) { 
        cout << "Receiver not found.\n"; 
        return; }
    if (receiverIndex == senderidx) {
        cout << "You cannot send money to yourself.\n";
        return;
    }
    cout << "Enter amount: ";
    if (!(cin >> amount)) { 
        clearInput(); 
        cout << "Invalid input.\n"; 
        return; }

    if (amount <= 0) { 
        cout << "Invalid amount.\n"; 
        return; }
    if (acc[senderidx].balance < amount) { 
        cout << "Insufficient balance.\n"; 
        return; }

    acc[senderidx].balance -= amount;
    acc[receiverIndex].balance += amount;
    acc[receiverIndex].receivedMoney += amount;
    acc[senderidx].history[acc[senderidx].historyCnt].sender = acc[senderidx].account_no;
    acc[senderidx].history[acc[senderidx].historyCnt].receiver = acc[receiverIndex].account_no;
    acc[senderidx].history[acc[senderidx].historyCnt].amount = amount;
    acc[senderidx].historyCnt++;
    acc[receiverIndex].history[acc[receiverIndex].historyCnt].sender = acc[senderidx].account_no;
    acc[receiverIndex].history[acc[receiverIndex].historyCnt].receiver = acc[receiverIndex].account_no;
    acc[receiverIndex].history[acc[receiverIndex].historyCnt].amount = amount;
    acc[receiverIndex].historyCnt++;

    saveToFile();
    cout << "Money sent successfully!\n";
}

void checkReceivedMoney(int i)
{
    if (i < 0 || i >= curr_accounts) { 
        cout << "Invalid account.\n"; 
        return; }
    cout << "You have received: " << acc[i].receivedMoney << endl;
    acc[i].receivedMoney = 0;
    saveToFile();
}
void showTransHistory(int i){
    cout << "--- TRANSACTION HISTORY ---\n";

    if(acc[i].historyCnt == 0){
        cout << "No transaction found.\n";
        return;
    }

    for(int j=0; j<acc[i].historyCnt; j++)
    {
        Transaction t = acc[i].history[j];
        if(t.sender == acc[i].account_no){
            cout << "Sent: " << t.amount << " to account " << t.receiver << endl;
        }
        else{
            cout << "Received: " << t.amount << " from Account " << t.sender << endl;
        }
    }
}

void transactions(int i)
{
    while (true) {
        cout << "\n--- TRANSACTION MENU ---\n";
        cout << "1. Send Money\n2. Check Received Money\n3. Show Transaction history\n0. Back\nEnter choice: ";
        int ch; 
        if (!(cin >> ch)) { 
            clearInput(); 
            cout<<"Invalid input.\n"; 
            continue; }
        if (ch == 1) 
            sendMoney(i);
        else if (ch == 2) 
            checkReceivedMoney(i);
        else if(ch == 3)
            showTransHistory(i);
        else if (ch == 0) 
            break;
        else 
            cout << "Invalid choice.\n";
    }
}



void adminCreateAccount()
{
    if (curr_accounts == capacity) grow_capacity();
    int i = curr_accounts;
    acc[i].account_no = next_acc++;
    acc[i].balance = 0.0;
    acc[i].closed = false;
    acc[i].receivedMoney = 0.0;
    acc[i].username[0] = acc[i].email[0] = acc[i].password[0] = '\0';
    acc[i].phone[0] = '\0';

    cout << "\n------ ADMIN: CREATE ACCOUNT ------\n";
    create(i);
    cout << "Enter Initial Balance: ";
    if (!(cin >> acc[i].balance)) { 
        clearInput(); 
        cout<<"Invalid input. Init balance set to 0.\n"; 
        acc[i].balance = 0.0; }
    curr_accounts++;
    saveToFile();
    cout << "Account Created Successfully! Account No: " << acc[i].account_no << "\n";
}

void showAllAccounts()
{
    if (curr_accounts == 0) { cout << "No accounts.\n"; return; }
    cout << "\n------ ALL ACCOUNTS ------\n";
    for (int i = 0; i < curr_accounts; ++i) {
        showAccountInfo(i);
    }
}

void searchAccount()
{
    int id;
    cout << "Enter Account No: ";
    if (!(cin >> id)) { 
        clearInput(); 
        cout<<"Invalid input.\n"; 
        return; }
    int idx = findAccountNo(id);
    if (idx == -1) { 
        cout << "Account not found or closed.\n"; 
        return; }
    showAccountInfo(idx);
}

void changeUsername(int acc_no)
{
    int idx = findAccountNo(acc_no);
    if (idx == -1) { 
        cout << "Invalid account.\n"; 
        return; }
    clearInput();
    char oldname[30], newname[30];
    cout << "Enter old username: ";
    cin.getline(oldname, sizeof(oldname));
    if (strcmp(acc[idx].username, oldname) != 0) { 
        cout << "Incorrect old username.\n"; 
        return; }
    cout << "Enter new username: ";
    cin.getline(newname, sizeof(newname));
    if (strlen(newname) == 0) { 
        cout << "New username cannot be empty.\n"; 
        return; }
    strcpy(acc[idx].username, newname);
    saveToFile();
    cout << "Username updated.\n";
}

void changeEmail(int acc_no)
{
    int idx = findAccountNo(acc_no);
    if (idx == -1) { 
        cout << "Invalid account.\n"; 
        return; }
    clearInput();
    char oldmail[50], newmail[50];
    cout << "Enter old email: ";
    cin.getline(oldmail, sizeof(oldmail));
    if (strcmp(acc[idx].email, oldmail) != 0) { 
        cout << "Incorrect old email.\n"; 
        return; }
    while (true) {
        cout << "Enter new email: ";
        cin.getline(newmail, sizeof(newmail));
        if (!isValidEmail(newmail)) { 
            cout << "Invalid email format.\n"; 
            continue; }
        if (!isEmailUnique(newmail)) { 
            cout << "Email already exists.\n"; 
            continue; }
        strcpy(acc[idx].email, newmail);
        saveToFile();
        cout << "Email updated.\n";
        break;
    }
}

void changePassword(int acc_no)
{
    int idx = findAccountNo(acc_no);
    if (idx == -1) { 
        cout << "Invalid account.\n"; 
        return;
    }
    clearInput();
    char oldpass[30], newpass[30];
    cout << "Enter old password: ";
    cin.getline(oldpass, sizeof(oldpass));
    if (strcmp(acc[idx].password, oldpass) != 0) { 
        cout << "Incorrect old password.\n"; 
        return; 
    }
    cout << "Enter new password (masked): ";
    getPassword(newpass, (int)sizeof(newpass));
    cout << endl;
    strcpy(acc[idx].password, newpass);
    saveToFile();
    cout << "Password updated.\n";
}

void closeAccounts()
{
    int close_id;
    cout << "Enter account to close: ";
    if (!(cin >> close_id)) { 
        clearInput(); cout<<"Invalid input.\n"; 
        return; 
    }
    int idx = findAccountNo(close_id);
    if (idx != -1) {
        acc[idx].closed = true;
        saveToFile();
        cout << "Account closed successfully\n";
    } else {
        cout << "Incorrect Account no or account already closed\n";
    }
}

void modifyAccount()
{
    int id;
    cout << "Enter Account No: ";
    if (!(cin >> id)) { 
        clearInput(); 
        cout<<"Invalid input.\n"; 
        return; 
    }
    int idx = findAccountNo(id);
    if (idx == -1) { 
        cout << "Account not found.\n"; 
        return; 
    }
    cout << "\n--- Modify Menu ---\n1. Change Username\n2. Change Email\n3. Change Password\nChoice: ";
    int ch; if (!(cin >> ch)) { 
        clearInput(); 
        cout<<"Invalid input.\n"; 
        return; 
    }
    if (ch == 1) 
        changeUsername(id);
    else if (ch == 2) 
        changeEmail(id);
    else if (ch == 3) 
        changePassword(id);
    else 
        cout << "Invalid option.\n";
}


void customerMenu()
{
    while (true) {
        cout << "\n------- CUSTOMER -------\n1. Login\n2. Sign Up\n0. Back\nEnter choice: ";
        int choice; if (!(cin >> choice)) { 
            clearInput(); 
            cout<<"Invalid input.\n"; 
            continue; 
        }
        if (choice == 1) {
            int idx = customerLogin();
            if (idx != -1) {
                while (true) {
                    cout << "\n--- CUSTOMER MENU ---\n1. View Account\n2. Deposit\n3. Withdraw\n4. Transactions\n0. Logout\nChoice: ";
                    int ch;
                    if (!(cin >> ch)) { 
                        clearInput(); 
                        cout<<"Invalid input.\n"; 
                        continue; 
                    }
                    if (ch == 1) 
                        showAccountInfo(idx);
                    else if (ch == 2) 
                        depositToAccount(idx);
                    else if (ch == 3) 
                        withdrawFromAccount(idx);
                    else if (ch == 4) 
                        transactions(idx);
                    else if (ch == 0) 
                        break;
                    else 
                        cout << "Invalid choice.\n";
                }
            }
        } 
        else if (choice == 2) 
            Signup();
        else if (choice == 0) 
            break;
        else 
            cout << "Invalid choice.\n";
    }
}

void adminMenu()
{
    cout << "ADMIN LOGIN (password masked): ";
    char pass[30]; 
    getPassword(pass, (int)sizeof(pass)); 
    cout << endl;

    if (strcmp(adminpass, pass) != 0) { 
        cout << "Incorrect password.\n"; 
        return; 
    }

    while (true) {
        cout << "\n------- ADMIN MENU -------\n1. Create Account\n2. Show All Accounts\n3. Search Account\n4. Modify Account\n5. Close Accounts\n0. Logout\nChoice: ";
        int ch;
        if (!(cin >> ch)) { 
            clearInput(); 
            cout<<"Invalid input.\n"; 
            continue; 
        }
        if (ch == 1) 
            adminCreateAccount();
        else if (ch == 2) 
            showAllAccounts();
        else if (ch == 3) 
            searchAccount();
        else if (ch == 4) 
            modifyAccount();
        else if (ch == 5) 
            closeAccounts();
        else if (ch == 0) 
            break;
        else 
            cout << "Invalid choice.\n";
    }
}


int main()
{
    
    acc = new Account[capacity];


    loadFromFile();

    cout << "Welcome to Simple Bank (binary persistence, validated)\n";

    while (true) {
        cout << "\n=========== MAIN MENU ===========\n1. Customer\n2. Admin\n0. Exit\nEnter your choice: ";
        int mainChoice;
        if (!(cin >> mainChoice)) { 
            clearInput(); 
            cout<<"Invalid input.\n"; 
            continue; 
        }
        if (mainChoice == 1) 
            customerMenu();
        else if (mainChoice == 2) 
            adminMenu();
        else if (mainChoice == 0) {
            cout << "Goodbye! Saving data...\n";
            saveToFile();
            break;
        } else 
            cout << "Invalid option.\n";
    }

    delete[] acc;
    return 0;
}