#ifndef CLOCK_H_
#define CLOCK_H_

#include <stddef.h>
#include <time.h>

typedef struct chess_clock_t chess_clock_t;

struct chess_clock_t {
	size_t white_t;
	size_t black_t;
	time_t white_last_check;
	time_t black_last_check;
	bool white_ticking;
};

chess_clock_t *create_clock();
void set_clock(chess_clock_t *clock, size_t white_t, size_t black_t);
size_t get_white_time(chess_clock_t *clock);
size_t get_black_time(chess_clock_t *clock);
void switch_clock(chess_clock_t *clock);
void clock_free(chess_clock_t *clock);

#endif /* CLOCK_H_ */
