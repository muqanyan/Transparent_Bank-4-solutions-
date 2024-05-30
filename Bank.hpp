#include <iostream>
#include <vector>
#include <mutex>

struct Account {
    int current_balance = 0;
    int min_balance = 0;
    int max_balance;
    bool is_frozen = false;
};

class Bank {
private:
    std::vector<Account> accounts;
    std::mutex mtx; // For thread-safety
public:
    Bank(int N, int max_balance) {
        accounts.resize(N);
        for(auto& account : accounts) {
            account.max_balance = max_balance;
        }
    }

    ~Bank() {
        // Cleanup if necessary
    }

    void print_balance(int account_number) {
        std::lock_guard<std::mutex> lock(mtx);
        if(account_number < 0 || account_number >= accounts.size()) {
            std::cout << "Wrong command, please try it again later" << std::endl;
            return;
        }
        std::cout << "Current balance: " << accounts[account_number].current_balance << std::endl;
        std::cout << "Minimum balance: " << accounts[account_number].min_balance << std::endl;
        std::cout << "Maximum balance: " << accounts[account_number].max_balance << std::endl;
    }

    void freeze_account(int account_number) {
        std::lock_guard<std::mutex> lock(mtx);
        if(account_number < 0 || account_number >= accounts.size()) {
            std::cout << "Wrong command, please try it again later" << std::endl;
            return;
        }
        accounts[account_number].is_frozen = true;
        std::cout << "Successfully ended" << std::endl;
    }

    void unfreeze_account(int account_number) {
        std::lock_guard<std::mutex> lock(mtx);
        if(account_number < 0 || account_number >= accounts.size()) {
            std::cout << "Wrong command, please try it again later" << std::endl;
            return;
        }
        accounts[account_number].is_frozen = false;
        std::cout << "Successfully ended" << std::endl;
    }

    void transfer(int from_account, int to_account, int amount) {
        std::lock_guard<std::mutex> lock(mtx);
        if(from_account < 0 || from_account >= accounts.size() || to_account < 0 || to_account >= accounts.size() || amount <= 0) {
            std::cout << "Wrong command, please try it again later" << std::endl;
            return;
        }
        if(accounts[from_account].is_frozen || accounts[to_account].is_frozen || accounts[from_account].current_balance - amount < accounts[from_account].min_balance || accounts[to_account].current_balance + amount > accounts[to_account].max_balance) {
            std::cout << "Wrong command, please try it again later" << std::endl;
            return;
        }
        accounts[from_account].current_balance -= amount;
        accounts[to_account].current_balance += amount;
        std::cout << "Successfully ended" << std::endl;
    }

    void modify_all_balances(int amount) {
        std::lock_guard<std::mutex> lock(mtx);
        for(auto& account : accounts) {
            if(!account.is_frozen && account.current_balance + amount >= account.min_balance && account.current_balance + amount <= account.max_balance) {
                account.current_balance += amount;
            } else {
                std::cout << "Wrong command, please try it again later" << std::endl;
                return;
            }
        }
        std::cout << "Successfully ended" << std::endl;
    }

    void set_min_balance(int account_number, int min_balance) {
        std::lock_guard<std::mutex> lock(mtx);
        if(account_number < 0 || account_number >= accounts.size() || min_balance > accounts[account_number].current_balance) {
            std::cout << "Wrong command, please try it again later" << std::endl;
            return;
        }
        accounts[account_number].min_balance = min_balance;
        std::cout << "Successfully ended" << std::endl;
    }

    void set_max_balance(int account_number, int max_balance) {
        std::lock_guard<std::mutex> lock(mtx);
        if(account_number < 0 || account_number >= accounts.size() || max_balance < accounts[account_number].current_balance) {
            std::cout << "Wrong command, please try it again later" << std::endl;
            return;
        }
        accounts[account_number].max_balance = max_balance;
        std::cout << "Successfully ended" << std::endl;
    }
};
