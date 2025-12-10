#include "unity.h"
#include "../src/pieces/piece.h"
#include "../src/pieces/king.h"
#include "../src/pieces/rook.h"
#include "../src/boards/stdboard.h"
#include "../src/position.h"
#include "../src/move.h"

void setUp(void) {}
void tearDown(void) {}

// king
void test_king_create(void)
{
    piece_ptr_t k = king_create(SIDE_WHITE);
    TEST_ASSERT_EQUAL_UINT(SIDE_WHITE, k.i->get_side(k)); // 颜色
    TEST_ASSERT_TRUE(k.i->is(k, "king"));                 // 类别
    k.i->free(k);
}

void test_king_copy(void)
{
    piece_ptr_t k = king_create(SIDE_BLACK);
    TEST_ASSERT_FALSE(k.i->is_moved(k));
    piece_ptr_t c = k.i->copy(k);
    TEST_ASSERT_EQUAL_UINT(k.i->get_side(k), c.i->get_side(c));
    TEST_ASSERT_FALSE(c.i->is_moved(c));
    k.i->free(k);
    c.i->free(c);
}

void test_king_on_move(void)
{
    piece_ptr_t k = king_create(SIDE_WHITE);
    board_ptr_t b = stdboard_create();

    TEST_ASSERT_FALSE(k.i->is_moved(k));

    k.i->on_move(k, b);
    TEST_ASSERT_TRUE(k.i->is_moved(k));

    k.i->free(k);
    b.i->free(b);
}

// knight
void test_knight_create(void)
{
    piece_ptr_t k = knight_create(SIDE_WHITE);
    TEST_ASSERT_EQUAL_UINT(SIDE_WHITE, k.i->get_side(k));
    TEST_ASSERT_TRUE(k.i->is(k, "knight"));
    k.i->free(k);
}
void test_knight_copy(void)
{
    piece_ptr_t k = knight_create(SIDE_BLACK);
    piece_ptr_t c = k.i->copy(k);
    TEST_ASSERT_EQUAL_UINT(k.i->get_side(k), c.i->get_side(c));
    k.i->free(k);
    c.i->free(c);
}

void test_knight_can_control(void)
{
    board_ptr_t b = stdboard_create();
    piece_ptr_t k = knight_create(SIDE_WHITE);

    // knight at d4 -> (4,3)
    b.i->put(b, k, pos_create(4, 3));

    // valid
    TEST_ASSERT_TRUE(k.i->can_control(k, b, pos_create(3, 1)));
    TEST_ASSERT_TRUE(k.i->can_control(k, b, pos_create(2, 4)));

    // invalid
    TEST_ASSERT_FALSE(k.i->can_control(k, b, pos_create(4, 4)));
    TEST_ASSERT_FALSE(k.i->can_control(k, b, pos_create(4, 5)));

    b.i->free(b);
}

// queen
void test_queen_create(void)
{
    piece_ptr_t k = queen_create(SIDE_WHITE);
    TEST_ASSERT_EQUAL_UINT(SIDE_WHITE, k.i->get_side(k));
    TEST_ASSERT_TRUE(k.i->is(k, "queen"));
    k.i->free(k);
}

void test_queen_copy(void)
{
    piece_ptr_t k = queen_create(SIDE_BLACK);
    piece_ptr_t c = k.i->copy(k);
    TEST_ASSERT_EQUAL_UINT(k.i->get_side(k), c.i->get_side(c));
    k.i->free(k);
    c.i->free(c);
}

void test_queen_can_control(void)
{
    board_ptr_t b = stdboard_create();
    piece_ptr_t q = queen_create(SIDE_WHITE);

    // queen at d4 -> (4,3)
    b.i->put(b, q, pos_create(4, 3));

    // 直线
    TEST_ASSERT_TRUE(q.i->can_control(q, b, pos_create(4, 0))); // 左
    TEST_ASSERT_TRUE(q.i->can_control(q, b, pos_create(4, 7))); // 右
    TEST_ASSERT_TRUE(q.i->can_control(q, b, pos_create(0, 3))); // 上
    TEST_ASSERT_TRUE(q.i->can_control(q, b, pos_create(7, 3))); // 下
    // 对角线
    TEST_ASSERT_TRUE(q.i->can_control(q, b, pos_create(1, 0)));
    TEST_ASSERT_TRUE(q.i->can_control(q, b, pos_create(7, 6)));

    // 在中间放一个阻挡，返回 false
    piece_ptr_t blocker = pawn_create(SIDE_WHITE);
    b.i->put(b, blocker, pos_create(4, 5));
    TEST_ASSERT_FALSE(q.i->can_control(q, b, pos_create(4, 7)));

    b.i->free(b);
}

// rook
void test_rook_create(void)
{
    piece_ptr_t k = rook_create(SIDE_WHITE);
    TEST_ASSERT_EQUAL_UINT(SIDE_WHITE, k.i->get_side(k));
    TEST_ASSERT_TRUE(k.i->is(k, "rook"));
    k.i->free(k);
}
void test_rook_copy(void)
{
    piece_ptr_t k = rook_create(SIDE_BLACK);
    TEST_ASSERT_FALSE(k.i->is_moved(k));
    piece_ptr_t c = k.i->copy(k);
    TEST_ASSERT_EQUAL_UINT(k.i->get_side(k), c.i->get_side(c));
    TEST_ASSERT_FALSE(c.i->is_moved(c));
    k.i->free(k);
    c.i->free(c);
}
void test_rook_on_move(void)
{
    piece_ptr_t k = rook_create(SIDE_WHITE);
    board_ptr_t b = stdboard_create();

    TEST_ASSERT_FALSE(k.i->is_moved(k));

    k.i->on_move(k, b);
    TEST_ASSERT_TRUE(k.i->is_moved(k));

    k.i->free(k);
    b.i->free(b);
}

void test_rook_can_control(void)
{
    board_ptr_t b = stdboard_create();
    piece_ptr_t r = rook_create(SIDE_WHITE);

    // rook at d4 -> (4,3)
    b.i->put(b, r, pos_create(4, 3));

    // 直线
    TEST_ASSERT_TRUE(r.i->can_control(r, b, pos_create(4, 0))); // 左
    TEST_ASSERT_TRUE(r.i->can_control(r, b, pos_create(4, 7))); // 右
    TEST_ASSERT_TRUE(r.i->can_control(r, b, pos_create(0, 3))); // 上
    TEST_ASSERT_TRUE(r.i->can_control(r, b, pos_create(7, 3))); // 下

    // 阻挡
    piece_ptr_t blocker = pawn_create(SIDE_WHITE);
    b.i->put(b, blocker, pos_create(4, 5));
    TEST_ASSERT_FALSE(r.i->can_control(r, b, pos_create(4, 7)));

    b.i->free(b);
}

// pawn
void test_pawn_create(void)
{
    piece_ptr_t k = pawn_create(SIDE_WHITE);
    TEST_ASSERT_EQUAL_UINT(SIDE_WHITE, k.i->get_side(k));
    TEST_ASSERT_TRUE(k.i->is(k, "pawn"));
    k.i->free(k);
}
void test_pawn_copy(void)
{
    piece_ptr_t k = pawn_create(SIDE_BLACK);
    piece_ptr_t c = k.i->copy(k);
    TEST_ASSERT_EQUAL_UINT(k.i->get_side(k), c.i->get_side(c));

    k.i->free(k);
    c.i->free(c);
}

void test_pawn_can_walk_and_control(void)
{
    board_ptr_t b = stdboard_create();
    piece_ptr_t p = pawn_create(SIDE_WHITE);

    b.i->put(b, p, pos_create(6, 3));

    // 向前移动一格和两格
    TEST_ASSERT_TRUE(p.i->can_walk(p, b, pos_create(5, 3)));
    TEST_ASSERT_TRUE(p.i->can_walk(p, b, pos_create(4, 3)));

    // 对角线
    TEST_ASSERT_TRUE(p.i->can_control(p, b, pos_create(5, 2)));
    TEST_ASSERT_TRUE(p.i->can_control(p, b, pos_create(5, 4)));

    // 阻挡
    piece_ptr_t blocker = pawn_create(SIDE_WHITE);
    b.i->put(b, blocker, pos_create(5, 3));
    TEST_ASSERT_FALSE(p.i->can_walk(p, b, pos_create(5, 3)));
    TEST_ASSERT_TRUE(p.i->can_walk(p, b, pos_create(4, 3)));

    b.i->free(b);
}

// bishop
void test_bishop_create(void)
{
    piece_ptr_t k = bishop_create(SIDE_WHITE);
    TEST_ASSERT_EQUAL_UINT(SIDE_WHITE, k.i->get_side(k));
    TEST_ASSERT_TRUE(k.i->is(k, "bishop"));
    k.i->free(k);
}
void test_bishop_copy(void)
{
    piece_ptr_t k = bishop_create(SIDE_BLACK);
    piece_ptr_t c = k.i->copy(k);
    TEST_ASSERT_EQUAL_UINT(k.i->get_side(k), c.i->get_side(c));
    k.i->free(k);
    c.i->free(c);
}
void test_bishop_on_move(void)
{
    piece_ptr_t k = bishop_create(SIDE_WHITE);
    board_ptr_t b = stdboard_create();
    k.i->on_move(k, b);

    k.i->free(k);
    b.i->free(b);
}

void test_bishop_can_control(void)
{
    board_ptr_t b = stdboard_create();
    piece_ptr_t bs = bishop_create(SIDE_WHITE);

    b.i->put(b, bs, pos_create(4, 3));

    // 对角线
    TEST_ASSERT_TRUE(bs.i->can_control(bs, b, pos_create(1, 0)));
    TEST_ASSERT_TRUE(bs.i->can_control(bs, b, pos_create(7, 6)));

    // 直线
    TEST_ASSERT_FALSE(bs.i->can_control(bs, b, pos_create(4, 0)));
    TEST_ASSERT_FALSE(bs.i->can_control(bs, b, pos_create(0, 3)));

    // 阻挡
    piece_ptr_t blocker = pawn_create(SIDE_WHITE);
    b.i->put(b, blocker, pos_create(5, 4));
    TEST_ASSERT_FALSE(bs.i->can_control(bs, b, pos_create(7, 6)));

    b.i->free(b);
}

// 易位测试
void test_kingside(void)
{
    board_ptr_t b = stdboard_create();
    piece_ptr_t k = king_create(SIDE_WHITE);
    piece_ptr_t r_l = rook_create(SIDE_WHITE);
    piece_ptr_t r_r = rook_create(SIDE_WHITE);

    b.i->put(b, k, pos_create(7, 4));
    b.i->put(b, r_l, pos_create(7, 0));
    b.i->put(b, r_r, pos_create(7, 7));

    /* king e1 (7,4) -> g1 (7,6) */
    b.i->move(b, move_create(pos_create(7, 4), pos_create(7, 6)));

    TEST_ASSERT_TRUE(b.i->at(b, pos_create(7, 6)).ptr);
    TEST_ASSERT_TRUE(b.i->at(b, pos_create(7, 5)).ptr); /* rook moved to f1 */

    b.i->free(b);
}

void test_queenside(void)
{
    board_ptr_t b = stdboard_create();
    piece_ptr_t k = king_create(SIDE_WHITE);
    piece_ptr_t r_l = rook_create(SIDE_WHITE);
    piece_ptr_t r_r = rook_create(SIDE_WHITE);

    b.i->put(b, k, pos_create(7, 4));
    b.i->put(b, r_l, pos_create(7, 0));
    b.i->put(b, r_r, pos_create(7, 7));

    /* king e1 (7,4) -> c1 (7,2) */
    b.i->move(b, move_create(pos_create(7, 4), pos_create(7, 2)));

    TEST_ASSERT_TRUE(b.i->at(b, pos_create(7, 2)).ptr);
    TEST_ASSERT_TRUE(b.i->at(b, pos_create(7, 3)).ptr); /* rook moved to d1 */

    b.i->free(b);
}

int main(void)
{
    UNITY_BEGIN();
    // king
    RUN_TEST(test_king_create);
    RUN_TEST(test_king_copy);
    RUN_TEST(test_king_on_move);
    // knight
    RUN_TEST(test_knight_create);
    RUN_TEST(test_knight_copy);
    RUN_TEST(test_knight_can_control);
    // queen
    RUN_TEST(test_queen_create);
    RUN_TEST(test_queen_copy);
    RUN_TEST(test_queen_can_control);
    // rook
    RUN_TEST(test_rook_create);
    RUN_TEST(test_rook_copy);
    RUN_TEST(test_rook_on_move);
    RUN_TEST(test_rook_can_control);
    // pawn
    RUN_TEST(test_pawn_create);
    RUN_TEST(test_pawn_copy);
    RUN_TEST(test_pawn_can_walk_and_control);
    // bishop
    RUN_TEST(test_bishop_create);
    RUN_TEST(test_bishop_copy);
    RUN_TEST(test_bishop_on_move);
    RUN_TEST(test_bishop_can_control);
    // castle
    RUN_TEST(test_kingside);
    RUN_TEST(test_queenside);
    return UNITY_END();
}
