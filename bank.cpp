#include <iostream>
#include <string>
#include "Bank.h" // Assuming Bank.h contains the above definitions

void process_command(Bank& bank, const std::string& command) {
    // Parse and execute commands
    // Example: "print_balance 0", "freeze_account 1", "transfer 0 1 100"
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_accounts>" << std::endl;
        return 1;
    }

    int N = std::stoi(argv[1]);
    int max_balance = 1000; // Some default max balance

    Bank bank(N, max_balance);

    std::string command;
    while(std::getline(std::cin, command)) {
        process_command(bank, command);
    }

    return 0;
}
