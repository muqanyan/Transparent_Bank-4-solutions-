#include <iostream>
#include <pthread.h>
#include <vector>

const int SIZE = 8;

std::vector<char> buffer(SIZE, 0);
int in = 0;
int out = 0;

pthread_mutex_t mutex;
pthread_cond_t cond_empty;
pthread_cond_t cond_full;

void* produce(void* arg)
{
    char c = *((char*)arg);
    for(int i = 0; i < 2; ++i)
    {
        pthread_mutex_lock(&mutex);
        while ((in + 1) % SIZE == out) // Buffer full
        {
            pthread_cond_wait(&cond_empty, &mutex);
        }
        buffer[in] = c;
        in = (in + 1) % SIZE;
        std::cout << "produced: " << c << std::endl;
        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

void* consume(void* arg)
{
    for(int i = 0; i < 2; ++i) 
    {
        pthread_mutex_lock(&mutex);
        while (in == out) // Buffer empty
        {
            pthread_cond_wait(&cond_full, &mutex);
        }
        char c = buffer[out];
        out = (out + 1) % SIZE;
        std::cout << "consumed: " << c << std::endl;
        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

int main()
{
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond_empty, nullptr);
    pthread_cond_init(&cond_full, nullptr);

    std::vector<pthread_t> threads(20);
    std::vector<char> data{'a','b','c','d','e','f','g','h','i','j'};
    for(int i = 0; i < 20; i+=2)
    {
        pthread_create(&threads[i], nullptr, produce, &data[i/2]);
        pthread_create(&threads[i+1], nullptr, consume, nullptr);
    }

    for(int i = 0; i < 20; ++i)
    {
        pthread_join(threads[i], nullptr); 
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_empty);
    pthread_cond_destroy(&cond_full);

    return 0;
}
