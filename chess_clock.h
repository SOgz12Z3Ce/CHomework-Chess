#ifndef __CHESS_CLOCK_H
#define __CHESS_CLOCK_H

extern struct ChessClock clock_info;
extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

void *key_monitor(void *arg);
void *timer_a(void *arg);
void *timer_b(void *arg);

#endif
