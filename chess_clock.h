#ifndef CHESS_CLOCK_H
#define CHESS_CLOCK_H

#include <pthread.h>

// 棋钟全局状态结构体
typedef struct {
    int turn;          // 0:白方计时  1:黑方计时
    int running;       // 1:运行中  0:退出
    int time_a;        // 白方剩余时间（秒）
    int time_b;        // 黑方剩余时间（秒）
} ChessClock;

// 定时器结构体
typedef struct {
    LARGE_INTEGER frequency;
    LARGE_INTEGER lastTime;
    double interval;  // 间隔（秒）
} Timer;

// 全局共享棋钟实例（外部可访问）
extern ChessClock clock_info;
// 同步对象（外部可访问）
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;
// 计时线程ID（外部可访问）
extern pthread_t tid_a, tid_b;

// 定时器相关函数
void timer_init(Timer* timer, double interval_seconds);
int timer_should_trigger(Timer* timer);
void precise_delay(double seconds);

// 棋钟核心功能函数

//设置时间函数
void chess_clock_set_time(int white_sec, int black_sec);
//返回白方当前时间
int chess_clock_get_white();
//返回黑方当前时间
int chess_clock_get_black();
//切换计时器
void chess_clock_switch();
//创建时钟
void chess_clock_create();
//销毁时钟
void chess_clock_destroy();

#endif // CHESS_CLOCK_H
