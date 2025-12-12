#include "unity.h"
#include "unity_internals.h"
#include "../src/state.h"
#include "../src/position.h"
#include "../src/mallocer.h"
#include "../src/move.h"

void setUp(void) {}
void tearDown(void) {}

void test_pos_create(void)
{
    pos_t p = pos_create(2, 3);
    TEST_ASSERT_EQUAL_UINT(2, p.row);
    TEST_ASSERT_EQUAL_UINT(3, p.col);
}

void test_move_create(void)
{
    pos_t s = pos_create(0, 1);
    pos_t d = pos_create(4, 7);
    move_t m = move_create(s, d);
    TEST_ASSERT_EQUAL_UINT(0, m.src.row);
    TEST_ASSERT_EQUAL_UINT(1, m.src.col);
    TEST_ASSERT_EQUAL_UINT(4, m.dest.row);
    TEST_ASSERT_EQUAL_UINT(7, m.dest.col);
}

void test_state_create(void)
{
    size_t R = 2, C = 3;
    piece_ptr_t **st = state_create(R, C);
    TEST_ASSERT_NOT_NULL(st);
    TEST_ASSERT_NULL(st[0][0].ptr);
    TEST_ASSERT_NULL(st[1][2].ptr);
    state_free(st, R);
}

void test_state_copy(void)
{
    size_t R = 2, C = 2;
    piece_ptr_t **a = state_create(R, C);
    piece_ptr_t **b = state_copy(a, R, C);
    TEST_ASSERT_NOT_NULL(b);
    TEST_ASSERT_NULL(b[0][0].ptr);
    TEST_ASSERT_NULL(b[1][1].ptr);
    state_free(a, R);
    state_free(b, R);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_pos_create);
    RUN_TEST(test_move_create);
    RUN_TEST(test_state_create);
    RUN_TEST(test_state_copy);
    return UNITY_END();
}
