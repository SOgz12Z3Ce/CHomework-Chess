#include "gui/common.h"
#include "core/pieces/piece.h"
#include "core/boards/borad.h"
#include "core/pieces/rook.h"
#include "core/pieces/king.h"
#include "core/pieces/position.h"
#include "core/pieces/state.h"
#include<stdlib.h>

//调用get_state获得数组，name获得种类，get_side获得阵营，王车易位王和车移动次数，把现有的信息存进存档文件
void save(appstate_t *as)//存入函数
{
	board_ptr_t borad = as->game.board;//访问game的board
	piece_ptr_t **state = board.i->get_state(borad);//获得一个二维数组
	
	FILE *fp = fopen("save.txt", "w");

	if (fp == NULL) {
		printf("无法打开文件 save.txt\n");
		return;
	}
	else fprintf(fp,"position(row_col),piece_type,player_name,side,moved_count\n");

	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			if (state[i][j].ptr == NULL) {
				continue;//跳过空格子
			}
			piece_ptr_t p = state[i][j];//访问			
			char buffer_1[256];
			p.i->get_name(p, buffer_1);
			p.i->get_side(p);
	
	//ypedef struct pos {
//size_t row;
//size_t col;
//} pos_t;
			
			//position存位，直接用i,j表示
			// if先判定name是否为王或马
           // 这里命名规则是啥，会有输入规则吗
			if(buffer_1 == king){
				if(p.i->is_moved(p)){
					count++;
				}else{
					count =0;
				}//返回次数还是真假更好
			}
			elseif(buffer_1 == rook){
				if(p.i->is_moved(p)){
					count++;
				}else{
					count =0;
				}
			}
			else break;
			//王车移动次数怎么存，和其他棋子的存储格式怎么表示一致
		fprintf(fp,"%d_%d,%s,%s,%d\\n",i,j,buffer_1,p.i->player_name,p.i->side);
	fclose(fp);
	printf("save success in save.txt\n");
		}

piece_ptr_t **load()
{
	piece_ptr_t **ret;
	FILE *fp = fopen("save.txt", "r");
	if (fp == NULL) {
		printf("无法打开文件 save.txt\n");
		return;
	}
    else fprintf(fp,"position(row_col),piece_type,player_name,side,moved_count\n");
	
	//创建棋盘
	piece_ptr_t**board = malloc(8 * sizeof(piece_ptr_t*));
	for (size_t i = 0; i < 8; i++) {
		board[i] = malloc(8 * sizeof(piece_ptr_t));
}
        for (size_t j =0;j<8;j++){
		board[j]=malloc(8 * sizeof(piece_ptr_t));
	}
	//初始化
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			ret[i][j].ptr = NULL;//防止野指针
			
		}
	}
	char header[100];//临时存储表头
    fgets(header, sizeof(header), fp);  // 读取并忽略第一行（表头）
//逐行读取文件
  	int row,col,type,color,has_moved;
    char buffer_1[256];
    piece_ptr_t piece;
	while(fscanf(fp, "%d,%d,%d,%s,%d\n", &row, &col, &type,buffer_1, &has_moved) == 6 {
        if (row < 0 || row >= 8 || col < 0 || col >= 8) {
            printf("无效坐标 (%d,%d)，跳过该行\\n", row, col);
            continue;
        }
	}
	//创建棋子？为什么在初始化？但应该包含棋子的所有信息。这里不知道应该干嘛
	state[0][0] = rook_create(SIDE_BLACK);
	state[0][1] = knight_create(SIDE_BLACK);
	state[0][2] = bishop_create(SIDE_BLACK);
	state[0][3] = queen_create(SIDE_BLACK);
	state[0][4] = king_create(SIDE_BLACK);
	state[0][5] = bishop_create(SIDE_BLACK);
	state[0][6] = knight_create(SIDE_BLACK);
	state[0][7] = rook_create(SIDE_BLACK);
	state[1][0] = pawn_create(SIDE_BLACK);
	state[1][1] = pawn_create(SIDE_BLACK);
	state[1][2] = pawn_create(SIDE_BLACK);
	state[1][3] = pawn_create(SIDE_BLACK);
	state[1][4] = pawn_create(SIDE_BLACK);
	state[1][5] = pawn_create(SIDE_BLACK);
	state[1][6] = pawn_create(SIDE_BLACK);
	state[1][7] = pawn_create(SIDE_BLACK);
fclose(fp);
	printf("load success in save.txt\n");
	return state;
}
