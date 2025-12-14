#include "gui/common.h"
#include "core/pieces/piece.h"
#include "core/boards/borad.h"
#include "core/pieces/rook.h"
#include "core/pieces/king.h"
#include "core/pieces/position.h"
#include "core/pieces/state.h"
#include<stdlib.h>
#include<stdbool.h>
//？如何调用get_state获得数组，name获得种类，get_side获得阵营，王车易位王和车移动次数，把现有的信息存进存档文件
void save(appstate_t *as)//存入函数
{
	board_ptr_t board = as->game.board;//访问game的board
	piece_ptr_t **state = board.i->get_state(borad);//获得一个二维数组
	
	FILE *fp = fopen("save.txt", "w");

	if (fp == NULL) {
		printf("无法打开文件 save.txt\n");
		return;
	}
	else fprintf(fp,"position(row_col),piece_type,side,whether_moved\n");

	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			if (state[i][j].ptr == NULL) {
				continue;//跳过空格子
			}
			piece_ptr_t p = state[i][j];//访问			
			char buffer_1[256];
			char buffer_2[256];
			p.i->get_name(p, buffer_1);
			p.i->get_side(p,buffer_2);//这里函数返回的类型是什么：side_white,所以得用字符串存
			if(buffer_1 == king){
				return piece_is_moved(piece_ptr_t p)
			    }
			else if(buffer_1 == rook){
				return piece_is_moved(piece_ptr_t p)//函数里自带返回false和打印
				}
			else return true;
	fclose(fp);
	printf("save success in save.txt\n");
		}
    }
}

piece_ptr_t **load()
{
	size_t row_size =8,col_size =8;
	piece_ptr_t **state=(piece_ptr_t**)malloc(row_size*sizeof(piece_ptr_t*));//数组
    else fprintf(fp,"position(row_col),piece_type,side,whether_moved\n");
    if (ret == NULL) {
        fprintf(stderr, "内存分配失败！\n");
        exit(EXIT_FAILURE);                
    }
	//初始化
	for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			state[i][j].ptr = NULL;
		}
	}
    //逐行读取文件
    FILE *fp = fopen("save.txt", "r");
    if (fp== NULL) {
		printf("无法打开文件 save.txt\n");
		return ;
	}
	//跳过表头？
    char line_buf[512] = {0};
    if (fgets(line_buf, sizeof(line_buf), fp) == NULL) {
        fprintf(stderr, "错误：文件为空\n");
        fclose(fp);
        for (size_t i = 0; i < row_size; i++) free(state[i]);
        free(state);
        return NULL;
    }

    size_t row,col;
	piece_ptr_t piece;
    char buffer_1[256];//存棋子种类
	char buffer_2[256];//为存储side为char型
	bool is_moved;

    if (!parse_piece_from_line(line_buf, &row, &col, &piece, &is_moved)) {
            printf("无效坐标 (%d,%d)，跳过该行\\n", row, col);
            continue;
        }
    //如何从读到的数据往state数组里填充棋子
    for (size_t i = 0; i < 8; i++) {
		for (size_t j = 0; j < 8; j++) {
			if (state[i][j].ptr == NULL) {
				continue;//跳过空格子
			}
			piece_ptr_t p = state[i][j];//访问			
		}
	if (!check_castling_valid(state)) {
    printf("警告：王/车已移动，王车易位不合法\n");
}
return state;
    fclose(fp);
	printf("load success in save.txt\n");
// 释放内存
    free(ret);
    ret = NULL;
	return state;
}
