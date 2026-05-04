#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

class Account {
    int acno;
    char name[50];
    int deposit;
    char type;

public:
    void create_account();    // Get data from user
    void show_account() const; // Display data
    void modify();            // Update data
    void dep(int);            // Add to balance
    void draw(int);           // Subtract from balance
    void report() const;      // Show tabular data
    int retacno() const;      // Return account number
};

void Account::create_account() {
    cout << "\n[NEW ACCOUNT FORM]";
    cout << "\nEnter Account No. : ";
    cin >> acno;
    cout << "Enter Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Type (S/C): ";
    cin >> type;
    cout << "Initial Deposit: ";
    cin >> deposit;
    cout << "\nAccount Created Successfully!";
}

void Account::show_account() const {
    cout << "\nAcc No. : " << acno;
    cout << "\nHolder : " << name;
    cout << "\nType   : " << type;
    cout << "\nBalance: " << deposit;
}

void Account::modify() {
    cout << "\nAcc No. : " << acno;
    cout << "\nModify Name: ";
    cin.ignore();
    cin.getline(name, 50);
    cout << "Modify Type (S/C): ";
    cin >> type;
}

void Account::dep(int x) { deposit += x; }
void Account::draw(int x) { deposit -= x; }
void Account::report() const {
    cout << acno << setw(10) << " " << name << setw(10) << " " << type << setw(6) << deposit << endl;
}
int Account::retacno() const { return acno; }

// --- FILE HANDLING FUNCTIONS ---

void write_account() {
    Account ac;
    ofstream outFile("account.dat", ios::binary | ios::app);
    ac.create_account();
    outFile.write(reinterpret_cast<char*>(&ac), sizeof(Account));
    outFile.close();
}

void display_sp(int n) {
    Account ac;
    bool flag = false;
    ifstream inFile("account.dat", ios::binary);
    if (!inFile) { cout << "File could not be opened!!"; return; }
    while (inFile.read(reinterpret_cast<char*>(&ac), sizeof(Account))) {
        if (ac.retacno() == n) {
            ac.show_account();
            flag = true;
        }
    }
    inFile.close();
    if (!flag) cout << "\nAccount number does not exist";
}

void deposit_withdraw(int n, int sel) {
    int amt;
    bool found = false;
    Account ac;
    fstream File("account.dat", ios::binary | ios::in | ios::out);
    if (!File) { cout << "File could not be opened!!"; return; }
    while (!File.eof() && found == false) {
        File.read(reinterpret_cast<char*>(&ac), sizeof(Account));
        if (ac.retacno() == n) {
            ac.show_account();
            if (sel == 1) {
                cout << "\n\nTO DEPOSIT: ";
                cin >> amt;
                ac.dep(amt);
            } else {
                cout << "\n\nTO WITHDRAW: ";
                cin >> amt;
                ac.draw(amt);
            }
            int pos = (-1) * static_cast<int>(sizeof(ac));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char*>(&ac), sizeof(Account));
            cout << "\nRecord Updated";
            found = true;
        }
    }
    File.close();
    if (!found) cout << "\nRecord Not Found";
}

int main() {
    char ch;
    int num;
    do {
        system("cls"); // Use "clear" for Linux/macOS
        cout << "\n\n\t--- BANK MANAGEMENT SYSTEM ---";
        cout << "\n\t01. NEW ACCOUNT";
        cout << "\n\t02. DEPOSIT AMOUNT";
        cout << "\n\t03. WITHDRAW AMOUNT";
        cout << "\n\t04. BALANCE ENQUIRY";
        cout << "\n\t05. EXIT";
        cout << "\n\n\tSelect Option (1-5): ";
        cin >> ch;
        switch (ch) {
            case '1': write_account(); break;
            case '2': cout << "\nEnter Acc No: "; cin >> num; deposit_withdraw(num, 1); break;
            case '3': cout << "\nEnter Acc No: "; cin >> num; deposit_withdraw(num, 2); break;
            case '4': cout << "\nEnter Acc No: "; cin >> num; display_sp(num); break;
            case '5': cout << "\nThanks for using the system!"; break;
            default: cout << "\a";
        }
        cin.ignore();
        cin.get();
    } while (ch != '5');
    return 0;
}
