struct Account {
    int current_balance;
    int min_balance;
    int max_balance;
    bool is_frozen;
};

class Bank {
private:
    std::vector<Account> accounts;
    std::mutex mtx; // For thread-safety
public:
    Bank(int N, int max_balance);
    ~Bank();
    void print_balance(int account_number);
    void freeze_account(int account_number);
    void unfreeze_account(int account_number);
    void transfer(int from_account, int to_account, int amount);
    void modify_all_balances(int amount);
    void set_min_balance(int account_number, int min_balance);
    void set_max_balance(int account_number, int max_balance);
};
