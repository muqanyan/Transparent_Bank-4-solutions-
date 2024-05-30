// Создадим демонстрационное приложение, которое использует библиотеку.
#include "parallel_scheduler.h"
#include <iostream>
#include <chrono>

void example_task(int n) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << n << " completed" << std::endl;
}

int main() {
    parallel_scheduler scheduler(4); // 4 threads in the pool

    for (int i = 0; i < 10; ++i) { // Enqueue more tasks than threads
        scheduler.run(example_task, i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(5)); // Wait for all tasks to finish
    return 0;
}
