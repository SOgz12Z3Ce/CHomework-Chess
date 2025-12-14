#ifndef CHESS_CLOCK_H
#define CHESS_CLOCK_H

#include <stdlib.h>
#include <pthread.h>

typedef struct chess_clock chess_clock_t;

struct chess_clock {
    size_t white_time;          // 白方剩余时间（秒）
    size_t black_time;          // 黑方剩余时间（秒）
    int is_white_turn;          // 1:白方计时，0:黑方计时
    int running;                // 1:运行中，0:停止
    pthread_mutex_t mutex;      // 互斥锁
    pthread_cond_t cond;        // 条件变量
    pthread_t tid_white;        // 白方计时线程ID
    pthread_t tid_black;        // 黑方计时线程ID
};

// 1. 创建时钟实例，默认白方计时
chess_clock_t *create_clock();

// 2. 设置双方时间
void set_clock(chess_clock_t *clock, size_t white_t, size_t black_t);

// 3. 获取白方时间
size_t get_white_time(chess_clock_t *clock);

// 4. 获取黑方时间
size_t get_black_time(chess_clock_t *clock);

// 5. 切换计时方
void switch_clock(chess_clock_t *clock);

// 6. 启动棋钟
void start_clock(chess_clock_t *clock);

// 7. 销毁时钟实例
void clock_free(chess_clock_t *clock);

#endif
