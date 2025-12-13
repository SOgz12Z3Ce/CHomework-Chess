#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>
#include <conio.h>

// 全局共享状态
typedef struct {
    int turn;          // 0: A方计时  1: B方计时
    int running;       // 1: 程序运行中  0: 退出
    int time_a;        // A方剩余时间（秒）
    int time_b;        // B方剩余时间（秒）
} ChessClock;

ChessClock clock_info = {0, 1, 600, 600}; // 初始各10分钟(600秒)

pthread_mutex_t mutex;
pthread_cond_t cond;

// 切换玩家
//按空格切换回合，按ESC退出
void *key_monitor(void *arg) {
    char ch;
    while (clock_info.running) {
        if (_kbhit()) { // 检测是否有按键按下
            ch = _getch();
            pthread_mutex_lock(&mutex);
            switch (ch) {
                case ' ': // 空格键切换计时方
                    if (clock_info.turn == 0) {
                        clock_info.turn = 1;
                        printf("\nA方停钟,B方开始计时!\n");
                    } else {
                        clock_info.turn = 0;
                        printf("\nB方停钟,A方开始计时!\n");
                    }
                    pthread_cond_signal(&cond); // 唤醒等待的计时线程
                    break;
                case 27: // ESC键退出程序
                    clock_info.running = 0;
                    pthread_cond_broadcast(&cond); // 唤醒所有线程
                    printf("\n程序退出中...\n");
                    break;
            }
            pthread_mutex_unlock(&mutex);
        }
        Sleep(50); // 降低CPU占用
    }
    return NULL;
}

// A方计时线程，可返回时间
void *timer_a(void *arg) {
    while (clock_info.running) {
        pthread_mutex_lock(&mutex);
        // 不是A方回合 或 时间耗尽，等待
        while (clock_info.running && clock_info.turn != 0 && clock_info.time_a > 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        if (!clock_info.running || clock_info.time_a <= 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 计时逻辑：每秒减1
        Sleep(1000);
        clock_info.time_a--;
        printf("\rA方剩余时间: %02d:%02d | B方剩余时间: %02d:%02d",
               clock_info.time_a / 60, clock_info.time_a % 60,
               clock_info.time_b / 60, clock_info.time_b % 60);
        fflush(stdout); // 强制刷新输出缓冲区

        pthread_mutex_unlock(&mutex);
    }
    if (clock_info.time_a <= 0) {
        printf("\n\nA方时间耗尽!B方获胜!\n");
        clock_info.running = 0;
        pthread_cond_broadcast(&cond);
    }
    return NULL;
}

// B方计时线程，可返回时间
void *timer_b(void *arg) {
    while (clock_info.running) {
        pthread_mutex_lock(&mutex);
        // 不是B方回合 或 时间耗尽，等待
        while (clock_info.running && clock_info.turn != 1 && clock_info.time_b > 0) {
            pthread_cond_wait(&cond, &mutex);
        }

        if (!clock_info.running || clock_info.time_b <= 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        // 计时逻辑：每秒减1
        Sleep(1000);
        clock_info.time_b--;
        printf("\rA方剩余时间: %02d:%02d | B方剩余时间: %02d:%02d",
               clock_info.time_a / 60, clock_info.time_a % 60,
               clock_info.time_b / 60, clock_info.time_b % 60);
        fflush(stdout);

        pthread_mutex_unlock(&mutex);
    }
    if (clock_info.time_b <= 0) {
        printf("\n\nB方时间耗尽!A方获胜!\n");
        clock_info.running = 0;
        pthread_cond_broadcast(&cond);
    }
    return NULL;
}

int main()
{
    pthread_t tid_a, tid_b, tid_key;

    // 初始化互斥锁和条件变量
    if (pthread_mutex_init(&mutex, NULL) != 0 || pthread_cond_init(&cond, NULL) != 0) {
        printf("同步对象初始化失败！\n");
        return -1;
    }

    printf("=== 棋钟程序 ===\n");
    printf("操作说明：按【空格】键切换计时方 | 按【ESC】键退出\n");
    printf("初始时间:双方各10分钟\n\n");

    // 创建线程
    pthread_create(&tid_a, NULL, timer_a, NULL);
    pthread_create(&tid_b, NULL, timer_b, NULL);
    pthread_create(&tid_key, NULL, key_monitor, NULL);

    // 等待线程结束
    pthread_join(tid_a, NULL);
    pthread_join(tid_b, NULL);
    pthread_join(tid_key, NULL);

    // 销毁同步对象
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("程序已退出\n");

    return 0;
}
