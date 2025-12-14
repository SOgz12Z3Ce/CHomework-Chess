#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include <string.h>

// 全局共享状态
typedef struct {
    int turn;          // 0:白方计时  1:黑方计时
    int running;       // 1:运行中  0:退出
    int time_a;        // 白方剩余时间（秒）
    int time_b;        // 黑方剩余时间（秒）
} ChessClock;

ChessClock clock_info = {0, 0, 600, 600}; // 默认10分钟，初始未运行

typedef struct {
    LARGE_INTEGER frequency;
    LARGE_INTEGER lastTime;
    double interval;  // 间隔（秒）
} Timer;

pthread_mutex_t mutex;
pthread_cond_t cond;
pthread_t tid_a, tid_b; // 仅保留计时线程ID

// 定时器初始化
void timer_init(Timer* timer, double interval_seconds) {
    QueryPerformanceFrequency(&timer->frequency);
    QueryPerformanceCounter(&timer->lastTime);
    timer->interval = interval_seconds;
}

// 检查定时器是否触发
int timer_should_trigger(Timer* timer) {
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    double elapsed = (double)(currentTime.QuadPart - timer->lastTime.QuadPart) / timer->frequency.QuadPart;
    if (elapsed >= timer->interval) {
        timer->lastTime = currentTime;
        return 1;
    }
    return 0;
}

// 阻塞指定秒数（无Sleep，用定时器实现）
void precise_delay(double seconds) {
    Timer timer;
    timer_init(&timer, seconds);
    while (!timer_should_trigger(&timer));
}

// 1. 设置双方总时间
void chess_clock_set_time(int white_sec, int black_sec) {
    pthread_mutex_lock(&mutex);
    clock_info.time_a = white_sec;
    clock_info.time_b = black_sec;
    pthread_mutex_unlock(&mutex);
}

// 2. 获取白方当前时间
int chess_clock_get_white() {
    int time;
    pthread_mutex_lock(&mutex);
    time = clock_info.time_a;
    pthread_mutex_unlock(&mutex);
    return time;
}

// 3. 获取黑方当前时间
int chess_clock_get_black() {
    int time;
    pthread_mutex_lock(&mutex);
    time = clock_info.time_b;
    pthread_mutex_unlock(&mutex);
    return time;
}

// 4. 切换计时方（主动调用，无键盘依赖）
void chess_clock_switch() {
    pthread_mutex_lock(&mutex);
    if (clock_info.running == 0) {
        printf("棋钟未启动，无法切换！\n");
        pthread_mutex_unlock(&mutex);
        return;
    }
    clock_info.turn = !clock_info.turn;
    if (clock_info.turn == 0) {
        printf("\n黑方停钟, 白方开始计时!\n");
    } else {
        printf("\n白方停钟, 黑方开始计时!\n");
    }
    pthread_cond_broadcast(&cond); // 唤醒等待的计时线程
    pthread_mutex_unlock(&mutex);
}

// 白方计时线程
void* timer_white(void* arg) {
    Timer timer;
    timer_init(&timer, 1.0); // 1秒间隔
    while (1) {
        pthread_mutex_lock(&mutex);
        // 等待条件：运行中 + 非白方回合 + 时间未耗尽
        while (clock_info.running && clock_info.turn != 0 && clock_info.time_a > 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        // 退出条件
        if (!clock_info.running || clock_info.time_a <= 0) {
            if (clock_info.time_a <= 0) {
                printf("\n=== 白方时间耗尽! ===\n");
                clock_info.running = 0;
                pthread_cond_broadcast(&cond);
            }
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        // 计时逻辑（无Sleep，用定时器触发）
        if (timer_should_trigger(&timer)) {
            pthread_mutex_lock(&mutex);
            if (clock_info.turn == 0 && clock_info.running) {
                clock_info.time_a--;
                printf("白方剩余: %02d:%02d\r", clock_info.time_a / 60, clock_info.time_a % 60);
                fflush(stdout); // 强制刷新输出缓冲区
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

// 黑方计时线程
void* timer_black(void* arg) {
    Timer timer;
    timer_init(&timer, 1.0);
    while (1) {
        pthread_mutex_lock(&mutex);
        while (clock_info.running && clock_info.turn != 1 && clock_info.time_b > 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        if (!clock_info.running || clock_info.time_b <= 0) {
            if (clock_info.time_b <= 0) {
                printf("\n=== 黑方时间耗尽! ===\n");
                clock_info.running = 0;
                pthread_cond_broadcast(&cond);
            }
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        if (timer_should_trigger(&timer)) {
            pthread_mutex_lock(&mutex);
            if (clock_info.turn == 1 && clock_info.running) {
                clock_info.time_b--;
                printf("黑方剩余: %02d:%02d\r", clock_info.time_b / 60, clock_info.time_b % 60);
                fflush(stdout);
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    return NULL;
}

// 5. 创建时钟（初始化+启动线程）
void chess_clock_create() {
    // 初始化同步对象
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
    // 启动运行状态
    pthread_mutex_lock(&mutex);
    clock_info.running = 1;
    clock_info.turn = 0; // 默认白方先计时
    pthread_mutex_unlock(&mutex);
    // 创建线程
    pthread_create(&tid_a, NULL, timer_white, NULL);
    pthread_create(&tid_b, NULL, timer_black, NULL);
    printf("=== 棋钟启动 ===\n");
    printf("初始回合: 白方\n");
}

// 6. 销毁时钟（回收线程+释放资源）
void chess_clock_destroy() {
    // 先停止运行
    pthread_mutex_lock(&mutex);
    clock_info.running = 0;
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    // 等待线程结束
    pthread_join(tid_a, NULL);
    pthread_join(tid_b, NULL);
    // 销毁同步对象
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    printf("\n=== 棋钟已销毁 ===\n");
}
