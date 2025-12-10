#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>  // 添加 SDL_image 以支持 PNG 加载
#include <stdbool.h>
#include <stdio.h>

// 窗口尺寸
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// 回调类型定义
typedef void (*ButtonCallback)();

// 按钮结构体（添加纹理）
typedef struct {
    SDL_FRect rect;
    ButtonCallback on_click;
    SDL_Texture* texture;
} Button;

// 应用状态结构体（添加背景纹理）
typedef struct {
    bool running;
    Button buttons[3];
    int num_buttons;
    SDL_Texture* background_texture;
} appstate_t;

// 全局变量
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

// 示例回调函数
void start_game() {
    printf("开始游戏 clicked!\n");
    // 这里添加实际开始游戏逻辑，例如切换到游戏场景
}

void save_game() {
    printf("存档 clicked!\n");
    // 这里添加存档逻辑
}

void exit_game(appstate_t* as) {  // 传入状态以修改 running
    printf("退出游戏 clicked!\n");
    as->running = false;  // 退出循环
}

// 初始化回调
SDL_AppResult SDL_AppInit(void** appstate_p, int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // 初始化 SDL_image 以支持 PNG
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("IMG_Init Error: %s\n", IMG_GetError());
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow("Chess Menu", 
        WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    // 分配应用状态
    appstate_t* as = (appstate_t*)SDL_malloc(sizeof(appstate_t));
    if (!as) {
        return SDL_APP_FAILURE;
    }
    as->running = true;
    as->num_buttons = 3;

    // 加载背景图片 (p4.png)
    as->background_texture = IMG_LoadTexture(renderer, "p4.png");
    if (!as->background_texture) {
        printf("Failed to load background texture: %s\n", IMG_GetError());
        SDL_free(as);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    // 初始化按钮并绑定回调和纹理
    // p1.png 为开始游戏, p2.png 为存档, p3.png 为退出游戏
    as->buttons[0] = (Button){{200.0f, 150.0f, 400.0f, 100.0f}, start_game, IMG_LoadTexture(renderer, "p1.png")};
    as->buttons[1] = (Button){{200.0f, 300.0f, 400.0f, 100.0f}, save_game, IMG_LoadTexture(renderer, "p2.png")};
    as->buttons[2] = (Button){{200.0f, 450.0f, 400.0f, 100.0f}, (ButtonCallback)exit_game, IMG_LoadTexture(renderer, "p3.png")};

    // 检查按钮纹理加载
    for (int i = 0; i < 3; i++) {
        if (!as->buttons[i].texture) {
            printf("Failed to load button texture %d: %s\n", i+1, IMG_GetError());
            // 清理已加载的纹理
            for (int j = 0; j < i; j++) {
                SDL_DestroyTexture(as->buttons[j].texture);
            }
            SDL_DestroyTexture(as->background_texture);
            SDL_free(as);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
            return SDL_APP_FAILURE;
        }
    }

    *appstate_p = as;
    return SDL_APP_CONTINUE;
}

// 事件处理回调
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    appstate_t* as = (appstate_t*)appstate;

    if (event->type == SDL_EVENT_QUIT) {
        as->running = false;
        return SDL_APP_SUCCESS;
    } else if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float x = (float)event->button.x;
        float y = (float)event->button.y;
        SDL_FPoint point = {x, y};

        for (int i = 0; i < as->num_buttons; i++) {
            if (SDL_PointInRectFloat(&point, &as->buttons[i].rect)) {
                if (as->buttons[i].on_click) {
                    // 对于 exit_game，需要传入 as
                    if (i == 2) {  // 假设第三个是 exit
                        ((void (*)(appstate_t*))as->buttons[i].on_click)(as);
                    } else {
                        as->buttons[i].on_click();
                    }
                }
                break;
            }
        }
    }

    if (!as->running) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// 迭代（渲染）回调
SDL_AppResult SDL_AppIterate(void* appstate) {
    appstate_t* as = (appstate_t*)appstate;

    // 清屏（可选，如果背景覆盖整个窗口）
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // 黑色背景
    SDL_RenderClear(renderer);

    // 渲染背景
    SDL_FRect bg_rect = {0.0f, 0.0f, (float)WINDOW_WIDTH, (float)WINDOW_HEIGHT};
    SDL_RenderCopyF(renderer, as->background_texture, NULL, &bg_rect);

    // 渲染按钮纹理
    for (int i = 0; i < as->num_buttons; i++) {
        SDL_RenderCopyF(renderer, as->buttons[i].texture, NULL, &as->buttons[i].rect);
    }

    SDL_RenderPresent(renderer);

    if (!as->running) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

// 退出回调
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    appstate_t* as = (appstate_t*)appstate;
    if (as) {
        // 清理按钮纹理
        for (int i = 0; i < 3; i++) {
            if (as->buttons[i].texture) {
                SDL_DestroyTexture(as->buttons[i].texture);
            }
        }
        // 清理背景纹理
        if (as->background_texture) {
            SDL_DestroyTexture(as->background_texture);
        }
        SDL_free(as);
    }
    if (renderer) {
        SDL_DestroyRenderer(renderer);
    }
    if (window) {
        SDL_DestroyWindow(window);
    }
    IMG_Quit();  // 清理 SDL_image
    SDL_Quit();
}
