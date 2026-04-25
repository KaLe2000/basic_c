# Многопоточность (POSIX Threads)

## Основы

```c
#include <pthread.h>

pthread_t thread;  // идентификатор потока

// Функция потока
void* worker(void* arg) {
    int id = *(int*)arg;
    printf("Thread %d started\n", id);
    return NULL;  // или pthread_exit()
}

int main() {
    int id = 1;
    pthread_create(&thread, NULL, worker, &id);
    pthread_join(thread, NULL);  // ожидание завершения
}
```

## Создание потока

### pthread_create
```c
int pthread_create(
    pthread_t *thread,           // out: идентификатор
    const pthread_attr_t *attr,   // in: атрибуты (NULL = default)
    void *(*start_routine)(void*), // in: функция
    void *arg                   // in: аргумент
);
```

### pthread_attr_t — атрибуты
```c
pthread_attr_t attr;
pthread_attr_init(&attr);

pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
pthread_attr_setstacksize(&attr, 1024 * 1024);
pthread_attr_setguardsize(&attr, 4096);

// Создание с атрибутами
pthread_create(&thread, &attr, worker, arg);

pthread_attr_destroy(&attr);
```

## Завершение потока

### pthread_join — ожидание
```c
void* result;
pthread_join(thread, &result);  // блокирует до завершения
// result = то что вернул start_routine
```

### pthread_detach
```c
pthread_detach(thread);  // автоматическое освобождение ресурсов
// join не нужен
```

### pthread_exit
```c
void* worker(void* arg) {
    int *input = (int*)arg;
    int result = *input * 2;

    pthread_exit((void*)result);  // выход с кодом
}
```

## Синхронизация

### Mutex (взаимное исключение)
```c
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* worker(void* arg) {
    pthread_mutex_lock(&mutex);
    shared_counter++;
    pthread_mutex_unlock(&mutex);
    return NULL;
}
```

### Инициализация mutex
```c
pthread_mutex_t mutex;
pthread_mutex_init(&mutex, NULL);   // динамическая
// или
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // статическая
```

### Try lock
```c
if (pthread_mutex_trylock(&mutex) == 0) {
    // успешно захвачен
    pthread_mutex_unlock(&mutex);
} else {
    // занят другим потоком
}
```

### Recursive mutex
```c
pthread_mutexattr_t attr;
pthread_mutexattr_init(&attr);
pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

pthread_mutex_t mutex;
pthread_mutex_init(&mutex, &attr);
// позволяет multiple lock в одном потоке
```

## Condition variables

### Ожидание события
```c
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int ready = 0;

// Поток-производитель
void* producer(void* arg) {
    pthread_mutex_lock(&mutex);
    ready = 1;
    pthread_cond_signal(&cond);  // уведомить один
    // или pthread_cond_broadcast(&cond); для всех
    pthread_mutex_unlock(&mutex);
}

// Поток-потребитель
void* consumer(void* arg) {
    pthread_mutex_lock(&mutex);
    while (!ready) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
}
```

### Функции condition variables

| Функция | Описание |
|---------|---------|
| `pthread_cond_init()` | инициализация |
| `pthread_cond_wait()` | ожидание signal |
| `pthread_cond_signal()` | пробуждение одного |
| `pthread_cond_broadcast()` | пробуждение всех |
| `pthread_cond_timedwait()` | ожидание с таймаутом |
| `pthread_cond_destroy()` | уничтожение |

## Thread-local storage

```c
pthread_key_t key;
pthread_key_create(&key, destructor);

void* worker(void* arg) {
    pthread_setspecific(key, arg);  // установить значение
    void* value = pthread_getspecific(key);  // получить
    return NULL;
}

pthread_key_delete(key);
```

## Barriers

```c
pthread_barrier_t barrier;
pthread_barrier_init(&barrier, NULL, 3);  // 3 потока

void* worker(void* arg) {
    // синхронизация всех
    pthread_barrier_wait(&barrier);
    // продолжение
}

pthread_barrier_destroy(&barrier);
```

## Пример: параллельная обработка

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000

typedef struct {
    int* data;
    size_t start;
    size_t end;
    long long sum;
} Task;

void* sumRange(void* arg) {
    Task* task = (Task*)arg;
    long long sum = 0;

    for (size_t i = task->start; i < task->end; i++) {
        sum += task->data[i];
    }

    task->sum = sum;
    pthread_exit((void*)sum);
}

int main() {
    int data[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        data[i] = i + 1;
    }

    pthread_t threads[NUM_THREADS];
    Task tasks[NUM_THREADS];
    size_t chunk = ARRAY_SIZE / NUM_THREADS;

    for (int i = 0; i < NUM_THREADS; i++) {
        tasks[i].data = data;
        tasks[i].start = i * chunk;
        tasks[i].end = (i + 1) * chunk;
        tasks[i].sum = 0;

        pthread_create(&threads[i], NULL, sumRange, &tasks[i]);
    }

    long long total = 0;
    for (int i = 0; i < NUM_THREADS; i++) {
        void* result;
        pthread_join(threads[i], &result);
        total += (long long)result;
    }

    printf("Sum: %lld (expected: %lld)\n",
           total, (long long)ARRAY_SIZE * (ARRAY_SIZE + 1) / 2);

    return 0;
}
```

## Пример: reader-writer lock

```c
#include <pthread.h>

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;
int shared_data = 0;

// Читатели (могут параллельно)
void* reader(void* arg) {
    pthread_rwlock_rdlock(&rwlock);
    int value = shared_data;  // read
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}

// Писатель (эксклюзивный доступ)
void* writer(void* arg) {
    pthread_rwlock_wrlock(&rwlock);
    shared_data = *(int*)arg;  // write
    pthread_rwlock_unlock(&rwlock);
    return NULL;
}
```

## Пример: producer-consumer

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define BUFFER_SIZE 10
#define MAX_ITEMS 100

typedef struct {
    int buffer[BUFFER_SIZE];
    size_t in;
    size_t out;
    size_t count;
    pthread_mutex_t mutex;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} Buffer;

void bufferInit(Buffer* buf) {
    buf->in = buf->out = buf->count = 0;
    pthread_mutex_init(&buf->mutex, NULL);
    pthread_cond_init(&buf->not_full, NULL);
    pthread_cond_init(&buf->not_empty, NULL);
}

void bufferPut(Buffer* buf, int value) {
    pthread_mutex_lock(&buf->mutex);

    while (buf->count >= BUFFER_SIZE) {
        pthread_cond_wait(&buf->not_full, &buf->mutex);
    }

    buf->buffer[buf->in] = value;
    buf->in = (buf->in + 1) % BUFFER_SIZE;
    buf->count++;

    pthread_cond_signal(&buf->not_empty);
    pthread_mutex_unlock(&buf->mutex);
}

int bufferGet(Buffer* buf) {
    pthread_mutex_lock(&buf->mutex);

    while (buf->count == 0) {
        pthread_cond_wait(&buf->not_empty, &buf->mutex);
    }

    int value = buf->buffer[buf->out];
    buf->out = (buf->out + 1) % BUFFER_SIZE;
    buf->count--;

    pthread_cond_signal(&buf->not_full);
    pthread_mutex_unlock(&buf->mutex);

    return value;
}

void bufferDestroy(Buffer* buf) {
    pthread_mutex_destroy(&buf->mutex);
    pthread_cond_destroy(&buf->not_full);
    pthread_cond_destroy(&buf->not_empty);
}
```

## Подводные камни

### 1. Race conditions
```c
// НЕПРАВИЛЬНО
if (ptr == NULL) {
    ptr = malloc(sizeof(int));  // два потока могут оба выделить
}
```

### 2. Deadlock
```c
// Два mutex, разный порядок захвата
void* task1(void* arg) {
    pthread_mutex_lock(&a);
    pthread_mutex_lock(&b);  // deadlock если task2 захватит b первым
    ...
}

void* task2(void* arg) {
    pthread_mutex_lock(&b);
    pthread_mutex_lock(&a);  // deadlock
    ...
}
```

### 3. Lost wakeup
```c
// signal без mutex
pthread_mutex_lock(&mutex);
ready = 1;
pthread_cond_signal(&cond);  // может потеряться
pthread_mutex_unlock(&mutex);

// ПРАВИЛЬНО: signal внутри mutex
```

### 4. Spurious wakeup
```c
// while, не if!
while (!ready) {
    pthread_cond_wait(&cond, &mutex);  // spurious wakeup возможен
}
```

### 5. Memory ordering
```c
// Компилятор может reorder!
// Для данных между потоками нужен барьер
pthread_mutex_lock(&mutex);
ready = 1;
pthread_mutex_unlock(&mutex);
```

## pthread функции

| Функция | Описание |
|---------|---------|
| `pthread_create()` | создать поток |
| `pthread_join()` | ожидать завершения |
| `pthread_detach()` | отсоединить |
| `pthread_exit()` | завершить текущий |
| `pthread_cancel()` | отменить |
| `pthread_mutex_*()` | mutex операции |
| `pthread_cond_*()` | condition variable |
| `pthread_rwlock_*()` | reader-writer lock |
| `pthread_barrier_*()` | barrier |

## Компиляция

```bash
gcc -pthread program.c -o program
```

`-pthread` добавляет `-lpthread` и определяет `_REENTRANT`.