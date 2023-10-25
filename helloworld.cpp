#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>
/*
void *thread_fnc(void * arg);
char thread_msg[] = "Hello Thread!"; // global
int main(){
 const int N = 5;
 int ret;
 pthread_t my_thread[N];
 void *ret_join;
 int numbs[N] = {1, 2, 3, 4, 5};
for (int i = 0; i != N; i++){
 ret = pthread_create(my_thread + i, NULL, thread_fnc, numbs+i);
 if (ret != 0) {
 perror("pthread_create failed\n");
 exit(EXIT_FAILURE);
 }
}
for (int i = 0; i != N; i++){
 ret = pthread_join(*(my_thread+i), &ret_join);
 if (ret != 0) {
 perror("pthread_join failed\n");
 exit(EXIT_FAILURE);
 }
}
getchar();
exit(EXIT_SUCCESS);
}
// дочерний поток
 void *thread_fnc(void *arg){
 int *p = (int *)arg;
 printf("%d\n", *p);
 return 0;
} 
*/


/*

const int bufferSize = 5;
std::vector<int> buffer;
std::mutex mtx;
std::condition_variable bufferNotEmpty, bufferNotFull;

void producer() {
    for (int i = 0; i < 10; ++i) { // Производитель создает 10 чисел
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Имитируем производство
        {
            std::unique_lock<std::mutex> lock(mtx);
            bufferNotFull.wait(lock, [] { return buffer.size() < bufferSize; });
            buffer.push_back(i);
            std::cout << "Произведено: " << i << std::endl;
            lock.unlock();
            bufferNotEmpty.notify_one();
        }
    }
}

void consumer() {
    for (int i = 0; i < 10; ++i) { // Потребитель должен забрать 10 чисел
        int value = -1; // Инициализируем значение, чтобы избежать предупреждения о неинициализированной переменной
        {
            std::unique_lock<std::mutex> lock(mtx);
            bufferNotEmpty.wait(lock, [] { return !buffer.empty(); });
            value = buffer.back();
            buffer.pop_back();
            std::cout << "Потреблено: " << value << std::endl;
            lock.unlock();
            bufferNotFull.notify_one();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(700)); // Имитируем обработку
    }
}

int main() {
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);

    producerThread.join();
    consumerThread.join();

    return 0;
}
*/

/*

char my_string[7]; // Массив для хранения строки  
// Бинарный семафор 
sem_t semaphore; 

// Функция, которая будет выполнена в дочернем потоке 
void *child_thread_function(void *arg) {  
 while (1)  {  
  // Ожидаем, пока семафор будет установлен в "1"  
  sem_wait(&semaphore); 
  // Подсчитываем количество символов в my_string и выводим результат  
  int length = strlen(my_string); 
  printf("Количество символов в строке: %d\n", length);
 } 
}  
*/




/*

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define N 5 // Количество итераций производителя

int buffer[BUFFER_SIZE];
sem_t empty, full;

void *producer(void *arg) {
    int item;
    for (int i = 0; i < N; i++) {
        // Ожидание, пока буфер опустеет
        sem_wait(&empty);

        // Запись в буфер
        item = i;
        buffer[i % BUFFER_SIZE] = item;
        printf("Произведено: %d\n", item);

        // Увеличение счетчика full
        sem_post(&full);
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    for (int i = 0; i < N; i++) {
        // Ожидание, пока буфер заполнится
        sem_wait(&full);

        // Чтение и удаление записи из буфера
        item = buffer[i % BUFFER_SIZE];
        printf("Потреблено: %d\n", item);

        // Увеличение счетчика empty
        sem_post(&empty);
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    // Инициализация семафоров
    sem_init(&empty, 0, BUFFER_SIZE); // empty инициализируется значением BUFFER_SIZE
    sem_init(&full, 0, 0); // full инициализируется значением 0

    // Создание потоков
    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    // Ожидание завершения потоков
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    // Уничтожение семафоров
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}
*/



/*
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5

pthread_barrier_t barrier;

void *thread_function(void *arg) {
    int thread_num = *((int *)arg);
    
    printf("Thread %d: before barrier\n", thread_num);
    
    // Ожидание на барьере
    pthread_barrier_wait(&barrier);
    
    printf("Thread %d: after barrier\n", thread_num);
    
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];
    
    // Инициализация барьера
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);
    
    // Создание и запуск потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }
    
    // Ожидание завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Уничтожение барьера
    pthread_barrier_destroy(&barrier);
    
    return 0;
}

*/


/*
#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 5
#define N 100

pthread_mutex_t mutex;
int sum = 0;

void *thread_function(void *arg) {
    int thread_num = *((int *)arg);
    int partial_sum = 0;

    // Вычисление частичной суммы
    for (int i = 1 + (thread_num - 1) * 20; i <= thread_num * 20; i++) {
        partial_sum += i;
    }

    // Защита доступа к общей переменной sum с помощью мьютекса
    pthread_mutex_lock(&mutex);
    sum += partial_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    // Инициализация мьютекса
    pthread_mutex_init(&mutex, NULL);

    // Создание и запуск потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_args[i] = i + 1;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    // Ожидание завершения всех потоков
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Уничтожение мьютекса
    pthread_mutex_destroy(&mutex);

    // Печать результата
    printf("Сумма чисел от 1 до %d: %d (ожидаемая сумма: %d)\n", N, sum, N * (N + 1) / 2);

    return 0;
}
*/



#include <stdio.h>
#include <pthread.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
pthread_mutex_t mutex;

void *producer(void *arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&mutex); // Заблокировать мьютекс перед доступом к буферу
        buffer[i % BUFFER_SIZE] = i;
        printf("Произведено: %d\n", i);
        pthread_mutex_unlock(&mutex); // Разблокировать мьютекс
        // Усыпить поток на некоторое время, чтобы другой поток мог выполнить свою работу
        sleep(100000); // Пауза в микросекундах
    }
    return NULL;
}

void *consumer(void *arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&mutex); // Заблокировать мьютекс перед доступом к буферу
        printf("Потреблено: %d\n", buffer[i % BUFFER_SIZE]);
        pthread_mutex_unlock(&mutex); // Разблокировать мьютекс
        // Усыпить поток на некоторое время, чтобы другой поток мог выполнить свою работу
        sleep(100000); // Пауза в микросекундах
    }
    return NULL;
}

int main() {
    pthread_t producer_thread, consumer_thread;

    pthread_mutex_init(&mutex, NULL); // Инициализировать мьютекс

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&mutex); // Уничтожить мьютекс

    return 0;
}