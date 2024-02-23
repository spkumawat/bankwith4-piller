#include <iostream>
#include <iomanip>
#include <string>
#include <stdexcept>

using namespace std;

class User {
private:
    string username;

public:
    User(const string& name) : username(name) {}

    const string& getUsername() const {
        return username;
    }
};

class Account {
protected:
    int accountNumber;
    double balance;
    User user;

public:
    Account(int accNumber, double initialBalance, const string& username)
        : accountNumber(accNumber), balance(initialBalance), user(username) {}

    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual void display() const = 0;
    virtual void calculateInterest() {}

    virtual ~Account() {}
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(int accNumber, double initialBalance, double rate, const string& username)
        : Account(accNumber, initialBalance, username), interestRate(rate) {}

    void deposit(double amount) override {
        balance += amount;
    }

    void calculateInterest() override {
        double interest = balance * interestRate / 100;
        deposit(interest);
    }

    void withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
        } else {
            throw invalid_argument("Insufficient balance.");
        }
    }

    void display() const override {
        cout << left << setw(25) << "Username: " << setw(30) << user.getUsername() << endl;
        cout << left << setw(25) << "Account Number: " << setw(30) << accountNumber << endl;
        cout << left << setw(25) << "Current Balance (rupees): " << setw(30) << balance << endl;
    }
};

class CurrentAccount : public Account {
public:
    CurrentAccount(int accNumber, double initialBalance, const string& username)
        : Account(accNumber, initialBalance, username) {}

    void deposit(double amount) override {
        balance += amount;
    }

    void withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
        } else {
            throw invalid_argument("Insufficient balance.");
        }
    }

    void display() const override {
        cout << left << setw(25) << "Username: " << setw(30) << user.getUsername() << endl;
        cout << left << setw(25) << "Account Number: " << setw(30) << accountNumber << endl;
        cout << left << setw(25) << "Current Balance (rupees): " << setw(30) << balance << endl;
    }
};

int main() {
    int accNumber;
    double initialBalance, interestRate, withdrawalAmount = 0;
    string accountType, username;

    try {
        cout << "Enter Username: ";
        cin >> username;

        cout << "\nEnter Account Number: ";
        cin >> accNumber;

        if (cin.fail() || accNumber < 0) {
            throw invalid_argument("Invalid account number. Please enter a positive integer.");
        }

        cout << "\nEnter Initial Balance: $";
        cin >> initialBalance;

        if (cin.fail() || initialBalance < 100) {
            throw invalid_argument("Minimum initial balance required is 100 rupees.");
        }

        cout << "\nEnter Account Type (S/C): ";
        cin >> accountType;

        if (accountType != "S" && accountType != "s" && accountType != "C" && accountType != "c") {
            throw invalid_argument("Invalid account type. Please enter 'S' or 'C'.");
        }

        Account* account;

        if (accountType == "S" || accountType == "s") {
            cout << "\nEnter Interest Rate (%): ";
            cin >> interestRate;

            if (cin.fail() || interestRate < 0) {
                throw invalid_argument("Invalid interest rate. Please enter a non-negative number.");
            }

            account = new SavingsAccount(accNumber, initialBalance, interestRate, username);
            account->calculateInterest();
            cout << "\nSavings Account Details:" << endl;
            account->display();
        } else if (accountType == "C" || accountType == "c") {
            cout << "\nEnter Withdrawal Amount: $";
            cin >> withdrawalAmount;

            if (cin.fail() || withdrawalAmount <= 0) {
                throw invalid_argument("Invalid withdrawal amount. Please enter a positive number.");
            }

            account = new CurrentAccount(accNumber, initialBalance, username);
            account->withdraw(withdrawalAmount);
            cout << "\nCurrent Account Details:" << endl;
            account->display();
        } else {
            throw invalid_argument("Invalid account type.");
        }

        delete account;
    } catch (const invalid_argument& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown error occurred." << endl;
        return 1;
    }

    return 0;
}
