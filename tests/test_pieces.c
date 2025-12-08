#include "unity.h"
#include "../src/pieces/piece.h"
#include "../src/pieces/king.h"
#include "../src/pieces/rook.h"

void setUp(void) {}
void tearDown(void) {}

void test_same_side(void)
{

    piece_ptr_t white_king = king_create(SIDE_WHITE);
    piece_ptr_t white_rook = rook_create(SIDE_WHITE);
    piece_ptr_t black_rook = rook_create(SIDE_BLACK);

    TEST_ASSERT_TRUE(is_same_side(white_king, white_rook));
    TEST_ASSERT_FALSE(is_same_side(white_king, black_rook));

    white_king.i->free(white_king);
    white_rook.i->free(white_rook);
    black_rook.i->free(black_rook);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_same_side);
    return UNITY_END();
}