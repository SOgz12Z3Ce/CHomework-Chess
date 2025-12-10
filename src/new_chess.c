#include <stdio.h>
#include <string.h>
#include <time.h>

// 棋子类型枚举（定义所有可能的棋子类型）
enum PieceType {
    EMPTY = 0,   // 空格子
    PAWN,        // 兵
    KNIGHT,      // 马
    BISHOP,      // 象
    ROOK,        // 车
    QUEEN,       // 后
    KING         // 王
};

// 颜色枚举
enum Color {
    WHITE = 0,   // 白方
    BLACK        // 黑方
};

// 当前回合枚举
enum Turn {
    WHITE_TURN = 0,  // 白方回合
    BLACK_TURN       // 黑方回合
};

// 游戏结果枚举
enum Result {
    NOT_FINISHED = 0,  // 未结束
    WHITE_WIN,         // 白胜
    BLACK_WIN,         // 黑胜
    DRAW               // 和棋
};

// 棋子结构体（存储单个棋子的完整信息）
struct PIECE {
    enum PieceType type;  // 棋子类型
    enum Color color;     // 棋子颜色
    int has_moved;        // 是否移动过（用于王车易位、吃过路兵规则）
};

// 王车易位权限结构体（存储特殊规则状态）
struct CastlingRights {
    int white_king;     // 白王是否移动过
    int white_rook_k;   // 白王翼车是否移动过（h1）
    int white_rook_q;   // 白后翼车是否移动过（a1）
    int black_king;     // 黑王是否移动过
    int black_rook_k;   // 黑王翼车是否移动过（h8）
    int black_rook_q;   // 黑后翼车是否移动过（a8）
};

// 游戏状态结构体（存储完整游戏状态，包括棋盘）
struct GAMESTATE {
    struct PIECE board[8][8];       // 对结构体变量名的声明：8x8棋盘，存储每个格子的棋子信息
    char player_white[50];          // 白方玩家名称
    char player_black[50];          // 黑方玩家名称
    enum Turn current_turn;         // 当前回合
    enum Result result;             // 游戏结果
    int move_count;                 // 总回合数（双方各走一步为一回合）
    int half_move_count;            // 半回合数（用于五十步规则）
    struct CastlingRights castling;  // 王车易位权限
    char en_passant_target[3];      // 吃过路兵目标位置（如"e3"，空字符串表示无）
};

// 存档结构体（用于将游戏状态保存到文件）
struct GAME_SAVE {
    char save_name[100];            // 存档名称
    char save_date[20];             // 存档日期（格式：YYYY-MM-DD HH:MM）
    int save_version;               // 存档版本（用于后续兼容）。大概率用不到
    struct GAMESTATE game_state;    // 完整游戏状态
};

// 获取当前时间字符串（用于存档日期）
void get_current_time(char *time_str, int max_len) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(time_str, max_len, "%Y-%m-%d %H:%M", tm_info);
}

// 初始化棋盘（设置初始棋子位置）
void init_board(struct PIECE board[8][8]) {
    //在函数（）括号内结构体PIECE的结构变量作为参数
    
    //调用结构体里的成员语法为：
    //先声明结构体变量名: struct 结构体名 结构体变量名【可以是各种形式（数组、指针等）】
    //在进行对成员的调用：结构体变量名（结构变量）.成员名
   
    //因为是初始化棋盘就相当于把棋子结构体的每一个成员都初始化，所以调用棋子结构体的所有成员
    //对于二维数组的遍历使用嵌套for循环，外层循环行，内层循环列
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j].type = EMPTY;//在枚举里声明过EMPTY为0，方便阅读
            board[i][j].color = WHITE;//没实际含义
            board[i][j].has_moved = 0;//为王车易位做准备
        }
    }

    // 设置白方棋子（第0行和第1行）
    board[0][0] = (struct PIECE){ROOK, WHITE, 0};    // a1 白车
    board[0][1] = (struct PIECE){KNIGHT, WHITE, 0};  // b1 白马
    board[0][2] = (struct PIECE){BISHOP, WHITE, 0};  // c1 白象
    board[0][3] = (struct PIECE){QUEEN, WHITE, 0};   // d1 白后
    board[0][4] = (struct PIECE){KING, WHITE, 0};    // e1 白王
    board[0][5] = (struct PIECE){BISHOP, WHITE, 0};  // f1 白象
    board[0][6] = (struct PIECE){KNIGHT, WHITE, 0};  // g1 白马
    board[0][7] = (struct PIECE){ROOK, WHITE, 0};    // h1 白车

    // 设置白方兵（第1行）
    for (int j = 0; j < 8; j++) {
        board[1][j] = (struct PIECE){PAWN, WHITE, 0};  // a2-h2 白兵
    }
    //对于像结构体这样复合字面量的初始化称作结构体初始化，又分为声明和赋值
    //赋值的语法格式为：
    // 结构体变量名 = (结构体类型){成员1值, 成员2值, ...};
    // 设置黑方棋子（第7行和第6行）
    board[7][0] = (struct PIECE){ROOK, BLACK, 0};    // a8 黑车
    board[7][1] = (struct PIECE){KNIGHT, BLACK, 0};  // b8 黑马
    board[7][2] = (struct PIECE){BISHOP, BLACK, 0};  // c8 黑象
    board[7][3] = (struct PIECE){QUEEN, BLACK, 0};   // d8 黑后
    board[7][4] = (struct PIECE){KING, BLACK, 0};    // e8 黑王
    board[7][5] = (struct PIECE){BISHOP, BLACK, 0};  // f8 黑象
    board[7][6] = (struct PIECE){KNIGHT, BLACK, 0};  // g8 黑马
    board[7][7] = (struct PIECE){ROOK, BLACK, 0};    // h8 黑车

    // 设置黑方兵（第6行）
    for (int j = 0; j < 8; j++) {
        board[6][j] = (struct PIECE){PAWN, BLACK, 0};  // a7-h7 黑兵
    }
}

// 初始化游戏状态
void init_game_state(struct GAMESTATE *game, const char *white_name, const char *black_name) {
   //括号内声明函数参数，用指针
   //!!game是指向GAMESTATE结构体的指针，white_name和black_name是玩家名称的常量字符串指针
    init_board(game->board);// 初始化棋盘
    // 设置玩家名称
    //strncpy()函数用于将一个字符串复制到另一个字符串，最多复制n个字符
    //strncpy(目标字符串, 源字符串, 最大复制字符数)如果原字符串长度小于n,则用'\0'填充
    strncpy(game->player_white, white_name, sizeof(game->player_white) - 1);
    strncpy(game->player_black, black_name, sizeof(game->player_black) - 1);
    game->player_white[sizeof(game->player_white) - 1] = '\0';  // 确保字符串结束符
    game->player_black[sizeof(game->player_black) - 1] = '\0';  // 确保字符串结束符

    // 设置初始游戏状态
    game->current_turn = WHITE_TURN;  // 白方先行
    game->result = NOT_FINISHED;      // 游戏未结束
    game->move_count = 0;             // 回合数初始为0
    game->half_move_count = 0;        // 半回合数初始为0

    // 初始化王车易位权限（所有棋子都未移动过）
    game->castling.white_king = 0;
    game->castling.white_rook_k = 0;
    game->castling.white_rook_q = 0;
    game->castling.black_king = 0;
    game->castling.black_rook_k = 0;
    game->castling.black_rook_q = 0;

    // 初始化吃过路兵目标（无）
    game->en_passant_target[0] = '\0';
}

// 保存游戏状态到二进制文件
int save_game(const struct GAMESTATE *game, const char *save_name, const char *filename) {
    FILE *fp = NULL;
    struct GAME_SAVE save;

    // 1. 填充存档信息
    strncpy(save.save_name, save_name, sizeof(save.save_name) - 1);
    save.save_name[sizeof(save.save_name) - 1] = '\0';
    
    // 获取当前时间作为存档日期
    get_current_time(save.save_date, sizeof(save.save_date));
    
    // 设置存档版本（用于后续兼容）
    save.save_version = 1;
    
    // 复制游戏状态到存档结构体
    save.game_state = *game;

    // 2. 打开二进制文件（wb模式：重写文件）
    fp = fopen(filename, "wb");
    if (fp == NULL) {
        printf("错误：无法打开文件 %s 进行写入！\n", filename);
        return 1;  // 返回1表示失败
    }

    // 3. 写入完整的存档结构体到文件
    size_t written = fwrite(&save, sizeof(struct GAME_SAVE), 1, fp);
    
    // 4. 关闭文件
    fclose(fp);

    // 5. 检查写入是否成功
    if (written != 1) {
        printf("错误：写入文件 %s 不完整！\n", filename);
        return 1;  // 返回1表示失败
    }

    printf("游戏已成功保存到 %s！\n", filename);
    return 0;  // 返回0表示成功
}

// 从二进制文件加载游戏状态
int load_game(struct GAMESTATE *game, char *save_name, const char *filename) {
    FILE *fp = NULL;
    struct GAME_SAVE save;

    // 1. 打开二进制文件（rb模式：只读）
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("错误：无法打开文件 %s 进行读取！\n", filename);
        return 1;  // 返回1表示失败
    }

    // 2. 从文件读取完整的存档结构体
    size_t read = fread(&save, sizeof(struct GAME_SAVE), 1, fp);
    
    // 3. 关闭文件
    fclose(fp);

    // 4. 检查读取是否成功
    if (read != 1) {
        printf("错误：读取文件 %s 不完整或格式错误！\n", filename);
        return 1;  // 返回1表示失败
    }

    // 5. 复制存档中的游戏状态到输出参数
    *game = save.game_state;
    
    // 6. 如果需要，返回存档名称
    if (save_name != NULL) {
        strncpy(save_name, save.save_name, 100);
        save_name[99] = '\0';
    }

    printf("游戏已成功从 %s 加载！\n", filename);
    printf("存档名称：%s\n", save.save_name);
    printf("存档日期：%s\n", save.save_date);
    printf("存档版本：%d\n", save.save_version);
    return 0;  // 返回0表示成功
}