#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include <string.h>

// 定义 chess_clock_t 结构体，包含状态+同步对象
typedef struct {
    size_t white_time;  // 白方时间（秒）
    size_t black_time;  // 黑方时间（秒）
    int is_white_turn;  // 1-白方计时，0-黑方计时
    int running;        // 1-运行中，0-停止
    pthread_mutex_t mutex;  // 互斥锁
    pthread_cond_t cond;    // 条件变量
    pthread_t tid_white;    // 白方计时线程ID
    pthread_t tid_black;    // 黑方计时线程ID
} chess_clock_t;

// 定时器结构体（高精度计时）
typedef struct {
    LARGE_INTEGER frequency;
    LARGE_INTEGER lastTime;
    double interval;  // 间隔（秒）
} Timer;

// 定时器初始化
static void timer_init(Timer* timer, double interval_seconds) {
    QueryPerformanceFrequency(&timer->frequency);
    QueryPerformanceCounter(&timer->lastTime);
    timer->interval = interval_seconds;
}

// 检查定时器是否触发
static int timer_should_trigger(Timer* timer) {
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    double elapsed = (double)(currentTime.QuadPart - timer->lastTime.QuadPart) / timer->frequency.QuadPart;
    if (elapsed >= timer->interval) {
        timer->lastTime = currentTime;
        return 1;
    }
    return 0;
}

// 高精度阻塞（替代 Sleep）
static void precise_delay(double seconds) {
    Timer timer;
    timer_init(&timer, seconds);
    while (!timer_should_trigger(&timer));
}

// 白方计时线程函数
static void* timer_white(void* arg) {
    chess_clock_t* clock = (chess_clock_t*)arg;
    Timer timer;
    timer_init(&timer, 1.0);  // 1秒递减一次

    while (1) {
        pthread_mutex_lock(&clock->mutex);
        // 等待条件：运行中 + 非白方回合 + 时间未耗尽
        while (clock->running && clock->is_white_turn != 1 && clock->white_time > 0) {
            pthread_cond_wait(&clock->cond, &clock->mutex);
        }
        // 退出条件：停止运行 或 时间耗尽
        if (!clock->running || clock->white_time <= 0) {
            if (clock->white_time <= 0) {
                printf("\n=== 白方时间耗尽! ===\n");
                clock->running = 0;
                pthread_cond_broadcast(&clock->cond);
            }
            pthread_mutex_unlock(&clock->mutex);
            break;
        }
        pthread_mutex_unlock(&clock->mutex);

        // 计时逻辑：每秒递减
        if (timer_should_trigger(&timer)) {
            pthread_mutex_lock(&clock->mutex);
            if (clock->is_white_turn == 1 && clock->running) {
                clock->white_time--;
                printf("白方剩余: %02zu:%02zu\r", clock->white_time / 60, clock->white_time % 60);
                fflush(stdout);  // 强制刷新输出
            }
            pthread_mutex_unlock(&clock->mutex);
        }
    }
    return NULL;
}

// 黑方计时线程函数
static void* timer_black(void* arg) {
    chess_clock_t* clock = (chess_clock_t*)arg;
    Timer timer;
    timer_init(&timer, 1.0);

    while (1) {
        pthread_mutex_lock(&clock->mutex);
        while (clock->running && clock->is_white_turn != 0 && clock->black_time > 0) {
            pthread_cond_wait(&clock->cond, &clock->mutex);
        }
        if (!clock->running || clock->black_time <= 0) {
            if (clock->black_time <= 0) {
                printf("\n=== 黑方时间耗尽! ===\n");
                clock->running = 0;
                pthread_cond_broadcast(&clock->cond);
            }
            pthread_mutex_unlock(&clock->mutex);
            break;
        }
        pthread_mutex_unlock(&clock->mutex);

        if (timer_should_trigger(&timer)) {
            pthread_mutex_lock(&clock->mutex);
            if (clock->is_white_turn == 0 && clock->running) {
                clock->black_time--;
                printf("黑方剩余: %02zu:%02zu\r", clock->black_time / 60, clock->black_time % 60);
                fflush(stdout);
            }
            pthread_mutex_unlock(&clock->mutex);
        }
    }
    return NULL;
}

// 1. 创建时钟实例，默认白方计时
chess_clock_t *create_clock() {
    chess_clock_t *clock = (chess_clock_t*)malloc(sizeof(chess_clock_t));
    if (clock == NULL) {
        perror("create_clock malloc failed");
        return NULL;
    }

    // 初始化成员变量
    clock->white_time = 0;
    clock->black_time = 0;
    clock->is_white_turn = 1;  // 默认白方先计时
    clock->running = 0;        

    // 初始化互斥锁和条件变量
    pthread_mutex_init(&clock->mutex, NULL);
    pthread_cond_init(&clock->cond, NULL);

    return clock;
}

// 2. 设置双方时间
void set_clock(chess_clock_t *clock, size_t white_t, size_t black_t) {
    if (clock == NULL) {
        fprintf(stderr, "set_clock: clock is NULL\n");
        return;
    }

    pthread_mutex_lock(&clock->mutex);
    clock->white_time = white_t;
    clock->black_time = black_t;
    pthread_mutex_unlock(&clock->mutex);
}

// 3. 获取白方时间
size_t get_white_time(chess_clock_t *clock) {
    if (clock == NULL) {
        fprintf(stderr, "get_white_time: clock is NULL\n");
        return 0;
    }

    size_t time;
    pthread_mutex_lock(&clock->mutex);
    time = clock->white_time;
    pthread_mutex_unlock(&clock->mutex);
    return time;
}

// 4. 获取黑方时间
size_t get_black_time(chess_clock_t *clock) {
    if (clock == NULL) {
        fprintf(stderr, "get_black_time: clock is NULL\n");
        return 0;
    }

    size_t time;
    pthread_mutex_lock(&clock->mutex);
    time = clock->black_time;
    pthread_mutex_unlock(&clock->mutex);
    return time;
}

// 5. 切换计时方
void switch_clock(chess_clock_t *clock) {
    if (clock == NULL) {
        fprintf(stderr, "switch_clock: clock is NULL\n");
        return;
    }

    pthread_mutex_lock(&clock->mutex);
    if (clock->running == 0) {
        printf("switch_clock: 棋钟未启动，无法切换！\n");
        pthread_mutex_unlock(&clock->mutex);
        return;
    }

    // 切换回合
    clock->is_white_turn = !clock->is_white_turn;
    if (clock->is_white_turn == 1) {
        printf("\n黑方停钟, 白方开始计时!\n");
    } else {
        printf("\n白方停钟, 黑方开始计时!\n");
    }
    pthread_cond_broadcast(&clock->cond);  // 唤醒等待的线程
    pthread_mutex_unlock(&clock->mutex);
}

// 启动棋钟
void start_clock(chess_clock_t *clock) {
    if (clock == NULL || clock->running == 1) {
        return;
    }

    pthread_mutex_lock(&clock->mutex);
    clock->running = 1;
    pthread_mutex_unlock(&clock->mutex);

    // 创建计时线程
    pthread_create(&clock->tid_white, NULL, timer_white, clock);
    pthread_create(&clock->tid_black, NULL, timer_black, clock);
    printf("=== 棋钟启动 ===\n初始回合: 白方\n");
}

// 6. 销毁时钟实例
void clock_free(chess_clock_t *clock) {
    if (clock == NULL) {
        return;
    }

    // 停止运行并唤醒所有线程
    pthread_mutex_lock(&clock->mutex);
    clock->running = 0;
    pthread_cond_broadcast(&clock->cond);
    pthread_mutex_unlock(&clock->mutex);

    // 等待线程结束
    pthread_join(clock->tid_white, NULL);
    pthread_join(clock->tid_black, NULL);

    // 销毁同步对象
    pthread_mutex_destroy(&clock->mutex);
    pthread_cond_destroy(&clock->cond);

    // 释放内存
    free(clock);
    printf("\n=== 棋钟已销毁 ===\n");
}
