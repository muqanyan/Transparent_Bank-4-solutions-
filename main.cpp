#include <iostream>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstring>

// Глобальные семафоры для каждого ресурса
sem_t* sem_tobacco;
sem_t* sem_paper;
sem_t* sem_matches;

void smoker(sem_t* sem, char item) {
    while (true) {
        sem_wait(sem); // Ожидание ресурса
        std::cout << item << std::flush; // Вывод идентификатора курильщика
        sleep(1); // Моделирование курения
        raise(SIGTERM); // Отправка сигнала завершения себе
    }
}

void handle_sigterm(int sig) {
    exit(0); // Корректное завершение процесса
}

int main() {
    // Инициализация семафоров
    sem_tobacco = sem_open("/sem_tobacco", O_CREAT, 0644, 0);
    sem_paper = sem_open("/sem_paper", O_CREAT, 0644, 0);
    sem_matches = sem_open("/sem_matches", O_CREAT, 0644, 0);

    signal(SIGTERM, handle_sigterm); // Установка обработчика SIGTERM

    pid_t pid = fork();
    if (pid == 0) {
        smoker(sem_tobacco, 'T');
    } else {
        pid = fork();
        if (pid == 0) {
            smoker(sem_paper, 'P');
        } else {
            pid = fork();
            if (pid == 0) {
                smoker(sem_matches, 'M');
            } else {
                // Бармен читает последовательность предметов и выдает их
                char input;
                while (std::cin >> input) {
                    switch (input) {
                        case 't':
                            sem_post(sem_tobacco);
                            break;
                        case 'p':
                            sem_post(sem_paper);
                            break;
                        case 'm':
                            sem_post(sem_matches);
                            break;
                        default:
                            break;
                    }
                }
                // Отправка SIGTERM всем процессам после окончания ввода
                kill(0, SIGTERM);
                // Ожидание завершения всех дочерних процессов
                while (wait(nullptr) > 0);
            }
        }
    }

    // Закрытие и удаление семафоров
    sem_close(sem_tobacco);
    sem_close(sem_paper);
    sem_close(sem_matches);
    sem_unlink("/sem_tobacco");
    sem_unlink("/sem_paper");
    sem_unlink("/sem_matches");

    return 0;
}
