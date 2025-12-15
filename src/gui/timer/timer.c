#include "gui/timer/timer.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

chess_clock_t *create_clock()
{
	chess_clock_t *ret = (chess_clock_t *)malloc(sizeof(chess_clock_t));
	time_t cur = time(NULL);

	*ret = (chess_clock_t){
		.white_t = 0,
		.black_t = 0,
		.white_last_check = cur,
		.black_last_check = cur,
		.white_ticking = true
	};
	return ret;
}

void set_clock(chess_clock_t *clock, size_t white_t, size_t black_t)
{
	time_t cur = time(NULL);
	clock->white_t = white_t;
	clock->black_t = black_t;
	clock->white_last_check = cur;
	clock->black_last_check = cur;
}

size_t get_white_time(chess_clock_t *clock)
{
	if (!clock->white_ticking)
		return clock->white_t;

	time_t cur = time(NULL);
	time_t passed = cur - clock->white_last_check;
	if (passed > clock->white_t)
		clock->white_t = 0;
	else
		clock->white_t = clock->white_t - passed;
	clock->white_last_check = cur;

	return clock->white_t;
}

size_t get_black_time(chess_clock_t *clock)
{
	if (clock->white_ticking)
		return clock->black_t;

	time_t cur = time(NULL);
	time_t passed = cur - clock->black_last_check;
	if (passed > clock->black_t)
		clock->black_t = 0;
	else
		clock->black_t = clock->black_t - passed;
	clock->black_last_check = cur;

	return clock->black_t;
}

void switch_clock(chess_clock_t *clock)
{
	time_t cur = time(NULL);
	clock->white_t = get_white_time(clock);
	clock->black_t = get_black_time(clock);
	if (clock->white_ticking)
		clock->black_last_check = cur;
	else
		clock->white_last_check = cur;
	clock->white_ticking = !clock->white_ticking;
}

void clock_free(chess_clock_t *clock)
{
	free(clock);
}
