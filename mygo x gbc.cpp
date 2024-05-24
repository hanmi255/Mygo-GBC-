#include <graphics.h>
#include <conio.h>
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <list>
#include <queue>
#include <set>
#include <easyx.h>
#include <chrono>
#include <thread>
#include <random>
#include <sstream>
#include <iomanip>
#include <tchar.h>
using namespace std;

// 字幕内容
std::vector<LPCTSTR> credits = {
    _T("欣然    组长"),
    _T("init() playerMove() callchess(int n)"),
    _T("hanmi	 技术官"),
    _T("Lead Actress: Mary Brown"),
    _T("小鸣	 技术官"),
    _T("gameView() ishasdown() isWin() gameLoop()"),
    _T("小灵通	 信息官"),
    _T("menuView() setView() UI"),
    _T("白	 信息官"),
    _T("pauseView()  int Pop()"),
    _T("徐辉杭	 技术官 项目经理"),
    _T("init() playerMove() callchess(int n)"),
    _T("量子二踢脚	 技术官"),
    _T("winView()"),
    _T("吃啥呢我也想吃	 副组长"),
    _T("gameView_ShowMap()"),
    _T("阿灿	 监督官"),
    _T("playerMove() callchess(int n)"),
    _T("徐昭	 技术官 项目经理"),
    _T("insertcemetery(int chess) drawCemetery()"),
    
};


// 棋盘全局变量
int flag = 0;
ExMessage m = { 0 };
IMAGE cqp; // 棋盘图像
IMAGE cqp_n; // 暂停后棋盘图像
IMAGE testchess[10]; // 棋子图像
IMAGE testchess_mask[10]; // 棋子遮罩图像
IMAGE testchess_n[10]; // 棋子图像
IMAGE testchess_n_mask[10]; // 棋子遮罩图像
TCHAR chesssign[10][30] = { _T("res/Ma1.jpg"), _T("res/Ma2.jpg"), _T("res/Ma3.jpg"), _T("res/Ma4.jpg"), _T("res/Ma5.jpg"), _T("res/Ga1.jpg"), _T("res/Ga2.jpg"), _T("res/Ga3.jpg"), _T("res/Ga4.jpg"), _T("res/Ga5.jpg") };
TCHAR chesssign_mask[10][30] = { _T("res/Mb1.jpg"), _T("res/Mb2.jpg"), _T("res/Mb3.jpg"), _T("res/Mb4.jpg"), _T("res/Mb5.jpg"), _T("res/Gb1.jpg"), _T("res/Gb2.jpg"), _T("res/Gb3.jpg"), _T("res/Gb4.jpg"), _T("res/Gb5.jpg") };


/*
棋子类型
    101,102,103,104,105表示Mygo方可以存入墓地的棋子
    001,002,003,004,005表示Mygo方不可以存入墓地的棋子
    106,107,108,109,110表示GBC方可以存入墓地的棋子
    006,007,008,009,010表示GBC方不可以存入墓地的棋子

    百位数表示能否存入墓地，个位十位表示棋子编号
*/
int Chess;
int map[19][19]; // 棋盘数组

// 棋盘和棋子参数(后续可扩展自定义棋盘)
const int board_start_x = 352; // 棋盘起始X坐标
const int board_start_y = 48; // 棋盘起始Y坐标
const int cell_size = 32; // 棋盘单元格大小
const int piece_size = 30; // 棋子大小
const int piece_half_size = piece_size / 2; // 棋子大小的一半

//墓地棋子
int mygo_cemetery[10];
int GBC_cemetery[10];
int mygo_chess = 0;//mygo当前墓地棋子数量
int GBC_chess = 0;//GBC当前墓地棋子数量

IMAGE baiban_n;
bool mluo = true;
int startX1 = 50; // Player A 的墓地起始X坐标
int startY1 = 700; // Player A 的墓地起始Y坐标
int startX2 = 690; // Player B 的墓地起始X坐标
int startY2 = 700; // Player B 的墓地起始Y坐标
int gap = 40; // 棋子之间的间隔
int* chess1 = new int;

// 其他全局变量
std::queue<int> chessqueue; // 落子队列
int gradeSize = 19; // 棋盘大小
int margin_x; // 水平边距
int margin_y; // 竖直边距
float chessSize = 60; // 棋子大小
bool playerFlag; // true: Mygo走，false：GBC走
IMAGE menuimg;
IMAGE gameimg;
IMAGE setimg;
IMAGE winimg;
IMAGE pauseimg;
IMAGE chessimg[10];
IMAGE baiban;//用于在落子处和墓地遮盖棋子
int round1 = 1;
ExMessage msg;
int PieceType;
MOUSEMSG msg2; // 全局变量，用于存储鼠标消息


// 主界面使用的枚举
enum ViewState {
    MENU,
    GAME,
    SETTINGS,
    EXIT
};

// 棋盘数组
int chessMap[19][19]; 

// 函数声明
void gameView();
void setView();
void menuView();
void gameView_ShowMap();
void winView(int i);
void pauseView(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound);
int isWin(int Chess, int x, int y);
int ishasdown(int x, int y);
void insertcemetery(int chess);
int iscontinuous(int x, int y);
int GetRandomNum(int min, int max);// 获取棋子坐标
int get_center_position(int start, int index, int cell_size);// 计算棋子落点的中心位置
void drawCemetery();
void initGameView();
void gameLoop();
void finalizeGameView();
void gbc_win();
void mygo_win();
void gameSet(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound);
void GameRules();
void displayScrollingCredits();
int getchess(int i);

// 音效全局变量
std::vector<Mix_Chunk*> clickSounds; // 存储点击音效
std::vector<Mix_Music*> bgMusics; // 存储背景音乐
int clickVolume = 50; // 点击音效音量
int bgVolume = 50; // 背景音乐音量
int clickx2 = clickVolume + 414;
int bgmx1 = bgVolume + 414;
Mix_Chunk* clickSound = nullptr;
Mix_Chunk* click5Sound = nullptr;
Mix_Chunk* click2Sound = nullptr;
Mix_Chunk* click6Sound = nullptr;

//初始化-----------------------------------------------------------------------------

// 初始化SDL2_mixer
bool initSDL2Mixer() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL2初始化失败: " << SDL_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL2_mixer初始化失败: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

// 加载音效
Mix_Chunk* loadSoundEffect(const char* path) {
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (sound == nullptr) {
        std::cerr << "音效文件加载失败: " << Mix_GetError() << std::endl;
    }
    return sound;
}

// 加载背景音乐
Mix_Music* loadMusic(const char* path) {
    Mix_Music* music = Mix_LoadMUS(path);
    if (music == nullptr) {
        std::cerr << "背景音乐加载失败: " << Mix_GetError() << std::endl;
    }
    return music;
}

// 释放音效
void freeSoundEffect(Mix_Chunk* sound) {
    if (sound != nullptr) {
        Mix_FreeChunk(sound);
    }
}

// 释放背景音乐
void freeMusic(Mix_Music* music) {
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
}

// 初始化棋盘
void chushihua() {
    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            map[i][j] = 0; // 将棋盘上所有位置置为0
        }
    }
    for (int i = 0; i < 10; i++) {
        mygo_cemetery[i] = 0;
        GBC_cemetery[i] = 0;
    }
    mygo_chess = 0;//mygo当前墓地棋子数量
    GBC_chess = 0;//GBC当前墓地棋子数量

}

// 设置窗口位置函数
void centerWindow() {
    HWND hwnd = GetHWnd();
    RECT rect;
    GetWindowRect(hwnd, &rect);

    int windowWidth = rect.right - rect.left;
    int windowHeight = rect.bottom - rect.top;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int posX = (screenWidth - windowWidth) / 2;
    int posY = (screenHeight - windowHeight) / 2;

    SetWindowPos(hwnd, HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

//游戏界面函数-----------------------------------------------------------------------------

/*
    menuView()
    负责人: 小灵通
    功能: easyx
        1. 展示选项: 进入游戏 进入设置 进入排行榜 退出游戏
        2. 接受玩家输入
        3. 执行对应操作
            进入游戏: 调用游戏界面
            进入设置: 调用设置界面
            进入排行榜: 调用排行榜界面(敬请期待)
            退出游戏: 结束程序
    参数: void
    返回值: void
*/
// 主菜单视图函数
void menuView() {
    initgraph(1280, 800); // 初始化图形窗口
    centerWindow(); // 居中窗口

    IMAGE background;
    loadimage(&background, _T("res/menu3.jpg")); // 加载背景图片

    ViewState viewState = MENU; // 初始视图状态为菜单

    if (!initSDL2Mixer()) {
        return;
    }

    // 加载点击音效和背景音乐
    clickSounds.push_back(loadSoundEffect("res/click1.mp3"));
    clickSounds.push_back(loadSoundEffect("res/click2.mp3"));
    bgMusics.push_back(loadMusic("res/bg1.mp3"));
    bgMusics.push_back(loadMusic("res/bg2.mp3"));

    Mix_Volume(-1, clickVolume); // 设置点击音效音量
    Mix_VolumeMusic(bgVolume); // 设置背景音乐音量

    if (!bgMusics.empty() && bgMusics[0] != nullptr) {
        Mix_PlayMusic(bgMusics[0], -1); // 循环播放背景音乐
    }

    putimage(0, 0, &background); // 显示背景图片
    FlushBatchDraw(); // 刷新绘图

    while (viewState != EXIT) {
        // 检查键盘输入
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) { // 按下ESC键退出
                viewState = EXIT;
            }
        }

        // 检查鼠标点击
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (msg.x >= 160 && msg.x <= 432) {
                    if (msg.y >= 472 && msg.y <= 508) {
                        // 播放点击音效
                        if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                            Mix_PlayChannel(-1, clickSounds[0], 0);
                        }
                        viewState = GAME; // 切换到游戏视图
                    }
                    else if (msg.y >= 565 && msg.y <= 603) {
                        // 播放点击音效
                        if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                            Mix_PlayChannel(-1, clickSounds[0], 0);
                        }
                        viewState = SETTINGS; // 切换到设置视图
                    }
                    else if (msg.y >= 660 && msg.y <= 696) {
                        // 播放点击音效
                        if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                            Mix_PlayChannel(-1, clickSounds[0], 0);
                        }
                        viewState = EXIT; // 退出
                    }
                }
            }
        }

        // 检查鼠标悬停
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(GetHWnd(), &cursor);

        static bool hover_played = false;
        if (cursor.x >= 160 && cursor.x <= 432) {
            if ((cursor.y >= 472 && cursor.y <= 508) ||
                (cursor.y >= 565 && cursor.y <= 603) ||
                (cursor.y >= 660 && cursor.y <= 696)) {
                if (!hover_played) {
                    if (!clickSounds.empty() && clickSounds[1] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[1], 0); // 播放悬停音效
                    }
                    hover_played = true;
                }
            }
            else {
                hover_played = false;
            }
        }
        else {
            hover_played = false;
        }

        // 根据视图状态切换视图
        switch (viewState) {
        case GAME:
            gameView();
            putimage(0, 0, &background);
            FlushBatchDraw();
            viewState = MENU;
            break;

        case SETTINGS:
            setView();
            putimage(0, 0, &background);
            FlushBatchDraw();
            viewState = MENU;
            break;

        case EXIT:
            break;

        default:
            break;
        }
    }

    // 释放音效和背景音乐
    for (auto sound : clickSounds) {
        freeSoundEffect(sound);
    }

    for (auto music : bgMusics) {
        freeMusic(music);
    }

    Mix_CloseAudio();
    SDL_Quit();
    closegraph();
}


/*
    setView()
    负责人:小灵通
    功能:
        1. 展示可以改动的游戏中变量
        2. 接收玩家输入
        3. 执行对应操作
            音效（开启/关闭）
            调节音量大小
            选择棋盘大小
    参数: void
    返回值: void
*/
// 设置界面
void setView() {
    initgraph(1280, 800); // 初始化图形窗口
    centerWindow(); // 居中窗口

    IMAGE bg;
    loadimage(&bg, _T("res/setview.jpg")); // 加载背景图片

    IMAGE buffer(1280, 800);
    setbkmode(TRANSPARENT); // 设置背景模式为透明
    setlinestyle(PS_SOLID, 5, NULL); // 设置线条样式
    settextcolor(YELLOW); // 设置文字颜色
    LOGFONT font;
    gettextstyle(&font);

    int width = 1280, height = 800;
    int posX = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
    int posY = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;
    SetWindowPos(GetHWnd(), HWND_TOP, posX, posY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

    putimage(0, 0, &bg); // 显示背景图片

    int a2change = clickVolume;
    int a1change = bgVolume;
    int y1 = 228;
    int y2 = 368;

    bool dragging1 = false, dragging2 = false;
    int dragStartX1 = 0, dragStartX2 = 0;

    IMAGE imga1, imga2;
    loadimage(&imga1, _T("res/bott1.png"));
    loadimage(&imga2, _T("res/bott1.png"));

    while (true) {
        BeginBatchDraw();
        putimage(0, 0, &bg); // 显示背景图片

        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();

            if (msg.uMsg == WM_LBUTTONDOWN) {
                if (msg.x >= bgmx1 && msg.x <= bgmx1 + imga1.getwidth() && msg.y >= y1 && msg.y <= y1 + imga1.getheight()) {
                    dragging1 = true;
                    dragStartX1 = msg.x - bgmx1;
                }
                if (msg.x >= clickx2 && msg.x <= clickx2 + imga2.getwidth() && msg.y >= y2 && msg.y <= y2 + imga2.getheight()) {
                    dragging2 = true;
                    dragStartX2 = msg.x - clickx2;
                }
                if (msg.x >= 419 && msg.x <= 622 && msg.y >= 566 && msg.y <= 696) {
                    if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[0], 0);
                    }
                    menuView(); // 返回主菜单               
                }
                // 检查点击是否在指定的矩形区域内
                if (msg.x >= 142 && msg.x <= 345 && msg.y >= 566 && msg.y <= 669) {
                    if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[0], 0);
                    }
                    GameRules(); // 操作说明
                    break; // 退出循环
                }
                //进入滚动报幕
                if (msg.x >= 1172 && msg.x <= 1255 && msg.y >= 691 && msg.y <= 781) {
                    if (!clickSounds.empty() && clickSounds[1] != nullptr) {
                        Mix_PlayChannel(-1, clickSounds[1], 0);
                    }
                    displayScrollingCredits(); // 操作说明
                    break; // 退出循环
                }



            }


            if (msg.uMsg == WM_MOUSEMOVE) {
                if (dragging1) {
                    int newX1 = msg.x - dragStartX1;
                    int deltaX1 = newX1 - 464;
                    if (deltaX1 >= -129 && deltaX1 <= 129) {
                        bgmx1 = newX1;
                        a1change = deltaX1 / 2 + 64;
                    }
                }
                if (dragging2) {
                    int newX2 = msg.x - dragStartX2;
                    int deltaX2 = newX2 - 464;
                    if (deltaX2 >= -129 && deltaX2 <= 129) {
                        clickx2 = newX2;
                        a2change = deltaX2 / 2 + 64;
                    }
                }
            }

            if (msg.uMsg == WM_LBUTTONUP) {
                dragging1 = false;
                dragging2 = false;
            }
        }

        clickVolume = a2change;
        bgVolume = a1change;

        putimage(bgmx1, y1, &imga1);
        putimage(clickx2, y2, &imga2);

        TCHAR str1[50], str2[50];
        _stprintf_s(str1, _T("%d"), a1change);
        _stprintf_s(str2, _T("%d"), a2change);
        outtextxy(616, 233, str1);
        outtextxy(616, 374, str2);

        EndBatchDraw();

        if (_kbhit()) {
            if (_getch() == 27) {
                break;
            }
        }
    }

    closegraph();
}

//暂停界面的设置
void gameSet(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound) {
    // 加载背景图片
    IMAGE bg;
    loadimage(&bg, _T("res/gameSet.jpg"));
    centerWindow();
    IMAGE buffer(1280, 800);
    setbkmode(TRANSPARENT); // 设置背景模式为透明
    setlinestyle(PS_SOLID, 5, NULL); // 设置线条样式
    settextcolor(YELLOW); // 设置文字颜色
    LOGFONT font;
    gettextstyle(&font);

    putimage(0, 0, &bg); // 显示背景图片

    int a2change = clickVolume;
    int a1change = bgVolume;
    int y1 = 228;
    int y2 = 368;

    bool dragging1 = false, dragging2 = false;
    int dragStartX1 = 0, dragStartX2 = 0;

    IMAGE imga1, imga2;
    loadimage(&imga1, _T("res/bott1.png"));
    loadimage(&imga2, _T("res/bott1.png"));

    while (true) { // 无限循环等待用户操作
        BeginBatchDraw();
        putimage(0, 0, &bg); // 显示背景图片

        MOUSEMSG msg = GetMouseMsg();

        if (msg.uMsg == WM_LBUTTONDOWN) {
            if (msg.x >= bgmx1 && msg.x <= bgmx1 + imga1.getwidth() && msg.y >= y1 && msg.y <= y1 + imga1.getheight()) {
                dragging1 = true;
                dragStartX1 = msg.x - bgmx1;
            }
            if (msg.x >= clickx2 && msg.x <= clickx2 + imga2.getwidth() && msg.y >= y2 && msg.y <= y2 + imga2.getheight()) {
                dragging2 = true;
                dragStartX2 = msg.x - clickx2;
            }
            if (msg.x >= 419 && msg.x <= 622 && msg.y >= 566 && msg.y <= 696) {
                if (!clickSounds.empty() && clickSounds[0] != nullptr) {
                    Mix_PlayChannel(-1, clickSounds[0], 0);
                }
                // 将改变的音量值赋回全局变量
                clickVolume = a2change;
                bgVolume = a1change;
                Mix_Volume(-1, clickVolume);
                Mix_VolumeMusic(bgVolume);
                return; // 返回暂停界面               
            }
        }

        if (msg.uMsg == WM_MOUSEMOVE) {
            if (dragging1) {
                int newX1 = msg.x - dragStartX1;
                int deltaX1 = newX1 - 464;
                if (deltaX1 >= -129 && deltaX1 <= 129) {
                    bgmx1 = newX1;
                    a1change = deltaX1 / 2 + 64;
                }
            }
            if (dragging2) {
                int newX2 = msg.x - dragStartX2;
                int deltaX2 = newX2 - 464;
                if (deltaX2 >= -129 && deltaX2 <= 129) {
                    clickx2 = newX2;
                    a2change = deltaX2 / 2 + 64;
                }
            }
        }

        if (msg.uMsg == WM_LBUTTONUP) {
            dragging1 = false;
            dragging2 = false;
        }

        putimage(bgmx1, y1, &imga1);
        putimage(clickx2, y2, &imga2);

        TCHAR str1[50], str2[50];
        _stprintf_s(str1, _T("%d"), a1change);
        _stprintf_s(str2, _T("%d"), a2change);
        outtextxy(616, 233, str1);
        outtextxy(616, 374, str2);

        EndBatchDraw();

        if (_kbhit()) {
            if (_getch() == 27) {
                break;
            }
        }
    }

    // 将改变的音量值赋回全局变量
    clickVolume = a2change;
    bgVolume = a1change;
    Mix_Volume(-1, clickVolume);
    Mix_VolumeMusic(bgVolume);

    closegraph();
}

//游戏说明界面
void GameRules()
{
    IMAGE rules;
    loadimage(&rules, _T("res/rules.jpg")); // 加载操作说明图片

    // 检查图片是否成功加载
    if (rules.getwidth() == 0 || rules.getheight() == 0) {
        printf("Failed to load rules.jpg\n");
        return;
    }

    // 确保图形窗口已初始化
    initgraph(1280, 800);
    centerWindow();  // 确保 centerWindow 函数已正确实现

    // 创建一个与屏幕大小相同的缓冲区
    IMAGE buffer(1280, 800);
    SetWorkingImage(&buffer); // 设置绘图操作的目标为缓冲区

    putimage(0, 0, &rules); // 在缓冲区中显示操作说明图片
    SetWorkingImage(); // 还原绘图操作的目标为屏幕

    // 启用批量绘图模式
    BeginBatchDraw();

    while (true)
    {
        // 将缓冲区内容复制到屏幕
        putimage(0, 0, &buffer);

        // 检查是否有鼠标点击事件
        if (MouseHit())
        {
            MOUSEMSG mouseMsg = GetMouseMsg();
            if (mouseMsg.uMsg == WM_LBUTTONDOWN)
            {
                // 检查点击是否在指定的矩形区域内
                if (mouseMsg.x >= 949 && mouseMsg.x <= 1054 && mouseMsg.y >= 643 && mouseMsg.y <= 698)
                {
                    setView(); // 调用 setView 函数
                    break; // 退出循环
                }
            }
        }

        // 刷新屏幕以显示缓冲区内容
        FlushBatchDraw();
    }

    // 结束批量绘图模式
    EndBatchDraw();

    closegraph();
}

/*
    gameView_ShowMap()
    负责人:吃啥呢
    功能:easyx
        打印棋盘、暂停、时间回合数、墓地、召唤、落子列表、本局随机获得棋子
        调用int iscontinuous()，如果满足消消乐的条件，将相连棋子透明化
    参数: void
    返回值: void
*/
// 显示游戏地图函数
void gameView_ShowMap() {
    loadimage(&gameimg, L"res/gameView.jpg", 1280, 800);
    putimage(0, 0, &gameimg);
}

/*
    pauseView()
    负责人: 白
    功能: easyx 暂停时间记录 打印暂停界面 用户可以点击继续游戏或退出游戏
    参数: void
    返回值: void
*/
// 暂停界面
void pauseView(IMAGE& bk, Mix_Chunk* click2Sound, Mix_Chunk* click6Sound) {
    loadimage(&pauseimg, _T("res/pauseView.jpg")); // 加载暂停界面图像

    // 确保界面刷新
    BeginBatchDraw();
    putimage(0, 0, &pauseimg); // 显示暂停界面
    EndBatchDraw();
    FlushBatchDraw();

    auto start = std::chrono::high_resolution_clock::now(); // 记录开始时间

    while (true) { // 无限循环等待用户操作
        if (MouseHit()) { // 检查是否有鼠标消息
            msg2 = GetMouseMsg(); // 获取鼠标消息
            if (msg2.uMsg == WM_LBUTTONDOWN && msg2.x >= 518 && msg2.x <= 763 && msg2.y >= 193 && msg2.y <= 640) {
                // 检查鼠标点击位置
                if (msg2.x >= 550 && msg2.x <= 730 && msg2.y >= 242 && msg2.y <= 308) {
                    // 继续游戏按钮区域
                    // 播放点击音效
                    if (click2Sound != nullptr) {
                        Mix_PlayChannel(-1, click2Sound, 0);
                    }
                    auto end = std::chrono::high_resolution_clock::now(); // 记录结束时间
                    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); // 计算暂停时间                    

                    // 恢复原界面
                    BeginBatchDraw();
                    putimage(0, 0, &bk);
                    EndBatchDraw();
                    FlushBatchDraw();
                    break; // 退出循环
                }
                else if (msg2.x >= 550 && msg2.x <= 730 && msg2.y >= 492 && msg2.y <= 557) {
                    // 播放点击音效
                    if (click6Sound != nullptr) {
                        Mix_PlayChannel(-1, click6Sound, 0);
                    }
                    menuView(); // 返回主页按钮区域                    
                    break; // 退出循环
                }
                else if (msg2.x >= 517 && msg2.x <= 762 && msg2.y >= 322 && msg2.y <= 427) {
                    // 设置按钮区域
                    // 播放点击音效
                    if (click6Sound != nullptr) {
                        Mix_PlayChannel(-1, click6Sound, 0);
                    }
                    // 进入设置界面
                    gameSet(bk, click2Sound, click6Sound);
                    // 确保界面刷新
                    BeginBatchDraw();
                    putimage(0, 0, &pauseimg); // 重新显示暂停界面
                    EndBatchDraw();
                    FlushBatchDraw();
                }
            }
        }
    }
}


//游戏核心-----------------------------------------------------------------------------
/*
    gameView()/init()/playerMove()/callchess(int n)
    负责人:徐辉杭、阿灿、组长、小鸣
    功能:
         easyx
        1. 初始化游戏数据,直接初始化
        while(1)
        {
            2. 展示背景
            3. 显示当前落子方
            3. 接受玩家输入:
                判断输入的类型
                    开局点击棋盘交点，落子
                    若棋子队列不空，则继续点击落子，下完交换
                    消消乐后点击召唤，再点击墓地中棋子，使之进入队列
            4. 落子后的结果 = 再棋盘上相应位置添加棋子
            5. 判断结果情况(调用函数hasdown())
                落子成功: 落子音效
                落子失败: 失败音效
            6. 胜利判断:(调用函数isWin())
                若游戏胜利: 展示胜利界面
        }
    参数: void
    返回值: void
*/

//游戏界面初始化
void initGameView() {
    // 初始化界面
    initgraph(1280, 800); // 不使用EX_SHOWCONSOLE标志
    centerWindow();
    cleardevice();
    chushihua(); // 初始化棋盘
    loadimage(&cqp, _T("res/gameView.jpg"), 1280, 800); // 加载背景图像
    loadimage(&cqp_n, _T("res/gameView.jpg"), 1280, 800); // 加载背景图像
    putimage(0, 0, &cqp); // 显示背景图像
    loadimage(&baiban, L"res/baiban.jpg", 70, 70);

    // 初始化SDL2_mixer
    if (!initSDL2Mixer()) {
        return;
    }

    // 加载点击音效
    clickSound = loadSoundEffect("res/click4.mp3");
    if (clickSound == nullptr) {
        std::cerr << "点击音效加载失败" << std::endl;
    }

    // 加载消除音效
    click5Sound = loadSoundEffect("res/click5.mp3");
    if (click5Sound == nullptr) {
        std::cerr << "消除音效加载失败" << std::endl;
    }

    // 加载暂停按钮音效
    click2Sound = loadSoundEffect("res/click2.mp3");
    if (click2Sound == nullptr) {
        std::cerr << "暂停按钮音效加载失败" << std::endl;
    }

    // 加载退出游戏音效
    click6Sound = loadSoundEffect("res/click6.mp3");
    if (click6Sound == nullptr) {
        std::cerr << "退出游戏音效加载失败" << std::endl;
    }

    // 设置点击音效音量
    Mix_VolumeChunk(clickSound, clickVolume);
    Mix_VolumeChunk(click5Sound, clickVolume);
    Mix_VolumeChunk(click2Sound, clickVolume);
    Mix_VolumeChunk(click6Sound, clickVolume);
}

//游戏主循环
void gameLoop() {
    int xuanzi = 0;
    bool zhong = true;
    int test = 1;
    int* chess = new int;
    *chess = 0;

    while (zhong) {
        BeginBatchDraw(); // 开始批量绘制

        if (flag % 2 == 0 && test == 1 && mluo == true) {
            *chess = GetRandomNum(101, 105);
            int c = *chess % 10 - 1;
            loadimage(&testchess[c], chesssign[c], 60, 60);
            loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
            putimage(1160, 70, &baiban);
            putimage(1160, 70, &testchess_mask[c], SRCAND);
            putimage(1160, 70, &testchess[c], SRCPAINT);
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            test++; // 获取随机棋子编号
            mluo = true;
        }
        else if (flag % 2 == 1 && test == 0 && mluo == true) {
            *chess = GetRandomNum(106, 110);
            int c = *chess % 10 - 1;
            if (c == -1) {
                c = 9;
            }
            loadimage(&testchess[c], chesssign[c], 60, 60);
            loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
            putimage(1160, 70, &baiban);
            putimage(1160, 70, &testchess_mask[c], SRCAND);
            putimage(1160, 70, &testchess[c], SRCPAINT);
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            test--; // 获取随机棋子编号
            mluo = true;
        }

        if (peekmessage(&m, EX_MOUSE | EX_KEY)) {
            if (m.message == WM_KEYDOWN) {
                if (m.vkcode == VK_ESCAPE) {
                    // 获取当前屏幕内容，用于恢复显示
                    getimage(&cqp, 0, 0, 1280, 800);
                    pauseView(cqp, click2Sound, click6Sound); // 调用暂停界面
                    EndBatchDraw(); // 结束批量绘制
                    continue; // 跳过本次循环，避免误处理棋子
                }
            }
            if (mygo_chess < 10 || GBC_chess < 10) {
                if (m.message == WM_RBUTTONDOWN) {
                    insertcemetery(*chess);
                    drawCemetery();

                    // 存入墓地后随机新的棋子
                    if (flag % 2 == 0 && mluo == true) {
                        *chess = GetRandomNum(101, 105);
                        int c = *chess % 10 - 1;
                        loadimage(&testchess[c], chesssign[c], 60, 60);
                        loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
                        putimage(1160, 70, &baiban);
                        putimage(1160, 70, &testchess_mask[c], SRCAND);
                        putimage(1160, 70, &testchess[c], SRCPAINT);
                        loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
                        loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size); // 获取随机棋子编号
                        mluo = true;
                    }
                    else if (flag % 2 == 1 && mluo == true) {
                        *chess = GetRandomNum(106, 110);
                        int c = *chess % 10 - 1;
                        if (c == -1) {
                            c = 9;
                        }
                        loadimage(&testchess[c], chesssign[c], 60, 60);
                        loadimage(&testchess_mask[c], chesssign_mask[c], 60, 60);
                        putimage(1160, 70, &baiban);
                        putimage(1160, 70, &testchess_mask[c], SRCAND);
                        putimage(1160, 70, &testchess[c], SRCPAINT);
                        loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
                        loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size); // 获取随机棋子编号
                        mluo = true;
                    }
                }
            }

            if (m.message == WM_LBUTTONDOWN) {
                // 检查是否点击暂停区域
                if (m.x >= 53 && m.x <= 256 && m.y >= 49 && m.y <= 152) {
                    // 播放点击音效
                    if (click2Sound != nullptr) {
                        Mix_PlayChannel(-1, click2Sound, 0);
                    }
                    // 获取当前屏幕内容，用于恢复显示
                    getimage(&cqp, 0, 0, 1280, 800);
                    pauseView(cqp, click2Sound, click6Sound); // 调用暂停界面
                    // 恢复游戏界面后重新绘制背景
                    putimage(0, 0, &cqp);
                    EndBatchDraw(); // 结束批量绘制
                    continue; // 跳过本次循环，避免误处理棋子
                }

                // 输出棋子编号
                for (int i = 0; i < 19; i++) {
                    for (int j = 0; j < 19; j++) {
                        int center_x = get_center_position(board_start_x, i, cell_size);
                        int center_y = get_center_position(board_start_y, j, cell_size);
                        if (m.x > center_x - piece_half_size && m.x < center_x + piece_half_size && m.y > center_y - piece_half_size - j && m.y < center_y + piece_half_size - j) {
                            if (ishasdown(i, j)) {
                                map[i][j] = *chess % 100;
                                int c = *chess % 10 - 1;
                                if (c == -1) {
                                    c = 9;
                                }
                                if (c >= 0 && c < 10) {
                                    putimage(center_x - piece_half_size, center_y - piece_half_size - j, &testchess_n_mask[c], SRCAND);
                                    putimage(center_x - piece_half_size, center_y - piece_half_size - j, &testchess_n[c], SRCPAINT);
                                    mluo = true;
                                    // 播放点击音效
                                    if (clickSound != nullptr) {
                                        Mix_PlayChannel(-1, clickSound, 0);
                                    }
                                    if (c < 5 && m.message != WM_RBUTTONDOWN) {
                                        test = 0;
                                    }
                                    if (c >= 5 && m.message != WM_RBUTTONDOWN) {
                                        test = 1;
                                    }
                                    if (isWin(*chess, i, j) == 1) {
                                        // 关闭图形窗口
                                        closegraph();
                                        // 释放背景音乐
                                        Mix_HaltMusic();
                                        zhong = false; // 游戏结束
                                        winView(1);
                                    }
                                    if (isWin(*chess, i, j) == 2) {
                                        // GBC获胜
                                        // 关闭图形窗口
                                        closegraph();
                                        // 释放背景音乐
                                        Mix_HaltMusic();
                                        zhong = false; // 游戏结束
                                        winView(2);
                                    }
                                    if (iscontinuous(i, j) > 0) {
                                        // 播放消除音效
                                        if (click5Sound != nullptr) {
                                            Mix_PlayChannel(-1, click5Sound, 0);
                                        }
                                        // 棋子消除后更新棋盘
                                        cleardevice();
                                        putimage(0, 0, &cqp_n); // 重新绘制背景
                                        drawCemetery(); // 重新墓地
                                        for (int k = 0; k < 19; k++) {
                                            for (int l = 0; l < 19; l++) {
                                                if (map[k][l] != 0) {
                                                    int center_x = get_center_position(board_start_x, k, cell_size);
                                                    int center_y = get_center_position(board_start_y, l, cell_size);
                                                    int c = map[k][l] % 10 - 1;
                                                    if (c == -1) {
                                                        c = 9;
                                                    }
                                                    putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n_mask[c], SRCAND);
                                                    putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n[c], SRCPAINT);
                                                }
                                            }
                                        }
                                        IMAGE zhaohuan;
                                        loadimage(&zhaohuan, L"res/zhaohuan.jpg", 151, 101);
                                        putimage(1068, 498, &zhaohuan);
                                        // 停止刷新棋盘，进入墓地召唤棋子操作
                                        EndBatchDraw();
                                        bool luoziw = true;
                                        while (luoziw) {
                                            // 如果mygo方墓地为空，则直接跳过回合
                                            if (mygo_chess == 0 && flag % 2 == 0) {
                                                BeginBatchDraw();
                                                flag++;
                                                luoziw = false;
                                            }
                                            // 如果GBC方墓地为空，则直接跳过回合
                                            else if (GBC_chess == 0 && flag % 2 == 1) {
                                                BeginBatchDraw();
                                                flag++;
                                                luoziw = false;
                                            }
                                            cout << "程序运行" << endl;
                                            if (peekmessage(&m, EX_MOUSE)) {
                                                if (m.message == WM_LBUTTONDOWN) {
                                                    for (int i = 0; i < 10; i++) {
                                                        // 对mygo方墓地操作
                                                        if (m.x > startX1 + i * gap - 12 && m.x < startX1 + i * gap + 20 && m.y > startY1 && m.y < startY1 + 24 && flag % 2 == 0) {
                                                            cout << "这是mygo墓地第" << i + 1 << "个位置" << endl;
                                                            getchess(i);
                                                            *chess = *chess1;
                                                            int n = *chess1 % 10 - 1;
                                                            if (n == -1) {
                                                                n = 9;
                                                            }
                                                            loadimage(&testchess[n], chesssign[n], 60, 60);
                                                            loadimage(&testchess_mask[n], chesssign_mask[n], 60, 60);
                                                            putimage(1160, 70, &baiban);
                                                            putimage(1160, 70, &testchess_mask[n], SRCAND);
                                                            putimage(1160, 70, &testchess[n], SRCPAINT);
                                                            loadimage(&testchess_n[n], chesssign[n], piece_size, piece_size);
                                                            loadimage(&testchess_n_mask[n], chesssign_mask[n], piece_size, piece_size);
                                                            luoziw = false;
                                                        }
                                                    }

                                                    for (int i = 0; i < 10; i++) {
                                                        // 对GBC方墓地操作
                                                        if (m.x > startX2 + i * gap - 12 && m.x < startX2 + i * gap + 20 && m.y > startY1 && m.y < startY1 + 24 && flag % 2 == 1) {
                                                            cout << "这是GBC墓地第" << i + 1 << "个位置" << endl;
                                                            getchess(i);
                                                            *chess = *chess1;
                                                            int n = *chess1 % 10 - 1;
                                                            if (n == -1) {
                                                                n = 9;
                                                            }
                                                            loadimage(&testchess[n], chesssign[n], 60, 60);
                                                            loadimage(&testchess_mask[n], chesssign_mask[n], 60, 60);
                                                            putimage(1160, 70, &baiban);
                                                            putimage(1160, 70, &testchess_mask[n], SRCAND);
                                                            putimage(1160, 70, &testchess[n], SRCPAINT);
                                                            loadimage(&testchess_n[n], chesssign[n], piece_size, piece_size);
                                                            loadimage(&testchess_n_mask[n], chesssign_mask[n], piece_size, piece_size);
                                                            luoziw = false;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        continue;
                                    }
                                    flag++;
                                }
                                else {
                                    cout << "棋子索引超出范围: " << c << endl;
                                }
                            }
                        }
                    }
                }
            }
        }
        EndBatchDraw(); // 结束批量绘制
    }

    delete chess; // 释放动态分配的内存
}

//游戏资源释放
void finalizeGameView() {
    // 释放点击音效和消除音效
    freeSoundEffect(clickSound);
    freeSoundEffect(click5Sound);
    freeSoundEffect(click2Sound);
    freeSoundEffect(click6Sound);

    // 关闭SDL2_mixer
    Mix_CloseAudio();
    SDL_Quit();
}

//原神启动
void gameView() {
    initGameView();
    gameLoop();
    finalizeGameView();
}

//函数为从墓地中取出棋子并刷新界面
int getchess(int i) {
    //定义一个新的指针空间
    int* chess = new int;
    //给指针指向的元素赋值
    *chess = 0;
    //下标
    int index;
    //用于打印棋子
    int c;
    //此处为mygo方墓地操作
    if (flag % 2 == 0) {
        //将选中棋子元素归0
        for (int j = 0; j < mygo_chess; j++) {
            if (i == j) {
                *chess = mygo_cemetery[j];
                index = i;
                mygo_cemetery[j] = 0;
            }
        }

        //将选中棋子后边的元素前移一位
        for (int z = 0; z < mygo_chess; z++) {
            if (z != 0 && z > i) {
                mygo_cemetery[z - 1] = mygo_cemetery[z];
            }
        }
        //将数组长度-1及最后一位归0
        mygo_cemetery[mygo_chess] = 0;
        mygo_chess--;
        for (int x = 0; x < mygo_chess; x++) {
            int c = mygo_cemetery[x] % 10 - 1;
            if (c == -1) {
                c = 9;
            }
            //重新绘制墓地中棋子
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            putimage(startX1 + i * gap, startY1, &testchess_mask[c], SRCAND);
            putimage(startX1 + i * gap, startY1, &testchess[c], SRCPAINT);
            cout << "绘制了墓地" << endl;
        }
    }
    //此处为GBC方墓地操作
    if (flag % 2 == 1) {
        //将选中棋子元素归0
        for (int j = 0; j < GBC_chess; j++) {
            if (i == j) {
                *chess = GBC_cemetery[j];
                index = i;
                GBC_cemetery[j] = 0;
            }
        }
        //将选中棋子后边的元素前移一位
        for (int z = 0; z < GBC_chess; z++) {
            if (z != 0 && z > i) {
                GBC_cemetery[z - 1] = GBC_cemetery[z];
            }
        }
        //将数组长度-1及最后一位归0
        GBC_cemetery[GBC_chess] = 0;
        GBC_chess--;
        for (int x = 0; x < GBC_chess; x++) {
            int c = GBC_cemetery[x] % 10 - 1;
            if (c == -1) {
                c = 9;
            }
            //重新绘制墓地中棋子
            loadimage(&testchess_n[c], chesssign[c], piece_size, piece_size);
            loadimage(&testchess_n_mask[c], chesssign_mask[c], piece_size, piece_size);
            putimage(startX2 + i * gap, startY2, &testchess_mask[c], SRCAND);
            putimage(startX2 + i * gap, startY2, &testchess[c], SRCPAINT);
            cout << "绘制了墓地" << endl;
        }
    }
    //刷新棋盘
    cleardevice();
    putimage(0, 0, &cqp_n); // 重新绘制背景
    drawCemetery();// 重新墓地
    for (int k = 0; k < 19; k++) {
        for (int l = 0; l < 19; l++) {
            if (map[k][l] != 0) {
                int center_x = get_center_position(board_start_x, k, cell_size);
                int center_y = get_center_position(board_start_y, l, cell_size);
                int c = map[k][l] % 10 - 1;
                if (c == -1) {
                    c = 9;
                }
                putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n_mask[c], SRCAND);
                putimage(center_x - piece_half_size, center_y - piece_half_size - l, &testchess_n[c], SRCPAINT);
            }
        }
    }
    //返回一个指针指向元素的值
    *chess1 = *chess;
    return *chess;
}



//其他功能-----------------------------------------------------------------------------
/*
    iscontinuous()
    负责人:hanmi 
    功能:  根据鼠标点击的坐标和棋子的表示值 判断落点后是否满足多字相连的消消乐
    参数:
        void
    返回值:
        0表示不可消消乐
        3表示由连续3个相同棋子
        4表示由连续4个相同棋子
        5表示由连续5个相同棋子
*/

// 消消乐函数
int iscontinuous(int x, int y) {
    int count = 0;
    int PieceType = map[x][y];
    int index_x[3];
    int index_y[3];

    // 检查水平方向是否可消除
    for (int i = x - 2, j = y; i <= x + 2; i++) {
        if (i >= 0 && i < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // 将其中一个消除的棋子存入墓地

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    count = 0;
    // 检查垂直方向是否可消除
    for (int i = x, j = y - 2; j <= y + 2; j++) {
        if (j >= 0 && j < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // 将其中一个消除的棋子存入墓地

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    count = 0;
    // 检查主对角线方向是否可消除
    for (int i = x - 2, j = y + 2; i <= x + 2 && j >= y - 2; i++, j--) {
        if (i >= 0 && i < 19 && j >= 0 && j < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // 将其中一个消除的棋子存入墓地

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    count = 0;
    // 检查副对角线方向是否可消除
    for (int i = x - 2, j = y - 2; i <= x + 2 && j <= y + 2; i++, j++) {
        if (i >= 0 && i < 19 && j >= 0 && j < 19 && map[i][j] == PieceType && map[i][j] != 0) {
            index_x[count] = i;
            index_y[count] = j;
            count++;

            if (count == 3) {
                // 将其中一个消除的棋子存入墓地

                for (int p = 0; p < count; p++) {
                    map[index_x[p]][index_y[p]] = 0;
                }
                return count;
            }
        }
        else {
            count = 0;
        }
    }

    return 0;
}

// 计算棋子落点的中心位置
int get_center_position(int start, int index, int cell_size) {
    return start + index * cell_size;
}

// 获取棋子坐标
int GetRandomNum(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

/*
    ishasdown()
    负责人:若凡
    功能: 判断点击处是否为合法落子点
    参数:
        void
    返回值:
        0表示不合法 (棋盘已经有子)
        1表示合法
*/
// 检查该位置是否已经有棋子
int ishasdown(int i, int j) {
    if (map[i][j] == 0) {
        return 1; // 该位置没有棋子
    }
    else {
        cout << "此处已有子" << endl << map[i][j];
        return 0; // 该位置已有棋子
    }
}

/*
    isWin()
    负责人: 徐昭
    功能: 根据鼠标点击的坐标和棋子的表示值 判断落点后是否获胜
    参数:
        void
    返回值:
        0表示没有获胜
        1表示Mygo胜利
        2表示GBC胜利
*/
// 判断是否获胜函数
int isWin(int chess, int x, int y) {
    if (flag % 2 == 0) {
        int count = 0;
        int s[5] = { 0, 0, 0, 0, 0 };
        // 判断横向是否有连续五个不同棋子
        for (int i = x - 4, j = y; i <= x + 4; i++) {
            if (i >= 0 && i < 19 && s[0] != map[i][j] && s[1] != map[i][j] && s[2] != map[i][j] && s[3] != map[i][j] && s[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo获胜
            }
        }
        count = 0;
        int s1[5] = { 0, 0, 0, 0, 0 };
        // 判断纵向是否有连续五个不同棋子
        for (int i = x, j = y - 4; j <= y + 4; j++) {
            if (j >= 0 && j < 19 && s1[0] != map[i][j] && s1[1] != map[i][j] && s1[2] != map[i][j] && s1[3] != map[i][j] && s1[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s1[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s1[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo获胜
            }
        }
        count = 0;
        int s2[5] = { 0, 0, 0, 0, 0 };
        // 判断主对角线是否有连续五个不同棋子
        for (int i = x - 4, j = y + 4; i <= x + 4 && j >= y - 4; i++, j--) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s2[0] != map[i][j] && s2[1] != map[i][j] && s2[2] != map[i][j] && s2[3] != map[i][j] && s2[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s2[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s2[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo获胜
            }
        }
        count = 0;
        int s3[5] = { 0, 0, 0, 0, 0 };
        // 判断副对角线是否有连续五个不同棋子
        for (int i = x - 4, j = y - 4; i <= x + 4 && j <= y + 4; i++, j++) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s3[0] != map[i][j] && s3[1] != map[i][j] && s3[2] != map[i][j] && s3[3] != map[i][j] && s3[4] != map[i][j] && map[i][j] <= 5 && map[i][j] != 0) {
                s3[count] = map[i][j];
                count++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s3[n] = 0;
                }
                count = 0;
            }
            if (count == 5) {
                return 1; // Mygo获胜
            }
        }
    }
    else if (flag % 2 == 1) {
        int count4 = 0;
        int s4[5] = { 0, 0, 0, 0, 0 };
        // 判断横向是否有连续五个不同棋子
        for (int i = x - 4, j = y; i <= x + 4; i++) {
            if (i >= 0 && i < 19 && s4[0] != map[i][j] && s4[1] != map[i][j] && s4[2] != map[i][j] && s4[3] != map[i][j] && s4[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s4[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s4[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC获胜
            }
        }
        count4 = 0;
        int s5[5] = { 0, 0, 0, 0, 0 };
        // 判断纵向是否有连续五个不同棋子
        for (int i = x, j = y - 4; j <= y + 4; j++) {
            if (j >= 0 && j < 19 && s5[0] != map[i][j] && s5[1] != map[i][j] && s5[2] != map[i][j] && s5[3] != map[i][j] && s5[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s5[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s5[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC获胜
            }
        }
        count4 = 0;
        int s6[5] = { 0, 0, 0, 0, 0 };
        // 判断主对角线是否有连续五个不同棋子
        for (int i = x - 4, j = y + 4; i <= x + 4 && j >= y - 4; i++, j--) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s6[0] != map[i][j] && s6[1] != map[i][j] && s6[2] != map[i][j] && s6[3] != map[i][j] && s6[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s6[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s6[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC获胜
            }
        }
        count4 = 0;
        int s7[5] = { 0, 0, 0, 0, 0 };
        // 判断副对角线是否有连续五个不同棋子
        for (int i = x - 4, j = y - 4; i <= x + 4 && j <= y + 4; i++, j++) {
            if (i >= 0 && i < 19 && j >= 0 && j < 19 && s7[0] != map[i][j] && s7[1] != map[i][j] && s7[2] != map[i][j] && s7[3] != map[i][j] && s7[4] != map[i][j] && map[i][j] > 5 && map[i][j] != 0) {
                s7[count4] = map[i][j];
                count4++;
            }
            else {
                for (int n = 0; n < 5; n++) {
                    s7[n] = 0;
                }
                count4 = 0;
            }
            if (count4 == 5) {
                return 2; // GBC获胜
            }
        }
    }
    return 0; // 没有获胜
}

/*
    负责人:徐昭
    功能:
        将得到的棋子存入墓地中
    参数:
        Chess
    返回值:
        void
*/
//存入墓地
void insertcemetery(int chess) {
    if (mygo_chess < 10) {
        if (chess >= 101 && chess <= 105) {
            // Player A
            mygo_cemetery[mygo_chess] = chess;
            mygo_chess++;
            flag++;
        }
    }

    if (GBC_chess < 10) {
        if (chess >= 106 && chess <= 110) {
            // Player B
            GBC_cemetery[GBC_chess] = chess;
            GBC_chess++;
            flag++;
        }
    }

}

//墓地棋子绘制
void drawCemetery() {
    int startX1 = 50; // Player A 的墓地起始X坐标
    int startY1 = 700; // Player A 的墓地起始Y坐标
    int startX2 = 690; // Player B 的墓地起始X坐标
    int startY2 = 700; // Player B 的墓地起始Y坐标
    int gap = 40; // 棋子之间的间隔

    // 绘制 Player A 的墓地
    for (int i = 0; i < mygo_chess; i++) {
        int c = mygo_cemetery[i] % 10 - 1;
        if (c == -1) c = 9;
        putimage(startX1 + i * gap, startY1, &testchess_n_mask[c], SRCAND);
        putimage(startX1 + i * gap, startY1, &testchess_n[c], SRCPAINT);
    }

    // 绘制 Player B 的墓地
    for (int i = 0; i < GBC_chess; i++) {
        int c = GBC_cemetery[i] % 10 - 1;
        if (c == -1) c = 9;
        putimage(startX2 + i * gap, startY2, &testchess_n_mask[c], SRCAND);
        putimage(startX2 + i * gap, startY2, &testchess_n[c], SRCPAINT);
    }
}


//动画-----------------------------------------------------------------------------

//GBC胜利结算
void gbc_win() {
    // 使用双缓冲技术
    BeginBatchDraw();
    centerWindow();
    // 总帧数和每秒帧数
    const int total_frames = 2165;
    const double fps = 23.967;
    const int frame_delay = static_cast<int>(1000 / fps); // 每帧延迟时间

    // 初始化SDL2_mixer
    if (!initSDL2Mixer()) {
        return;
    }

    Mix_Music* bgMusic = loadMusic("res/bg3.mp3");
    if (bgMusic == nullptr) {
        std::cerr << "背景音乐加载失败" << std::endl;
        return;
    }

    Mix_VolumeMusic(bgVolume); // 设置背景音乐音量
    Mix_PlayMusic(bgMusic, -1); // 循环播放背景音乐

    for (int i = 0; i < total_frames; ++i) {
        stringstream ss;
        ss << "gbccar/GBC" << setw(4) << setfill('0') << i << ".jpg";
        string filename = ss.str();

        // 转换文件名为LPCTSTR类型
        wstring wide_filename = wstring(filename.begin(), filename.end());
        LPCTSTR w_filename = wide_filename.c_str();

        // 测试
        //cout << "加载图片: " << filename << endl;

        // 加载图片
        IMAGE img;
        loadimage(&img, w_filename);

        // 检查图片是否加载成功
        if (img.getwidth() == 0 || img.getheight() == 0) {
            cout << "图片加载失败: " << filename << endl;
            continue;
        }

        // 显示图片
        putimage(0, 0, &img);
        //cout << "显示图片: " << filename << endl;

        // 刷新屏幕
        FlushBatchDraw();

        // 等待下一帧
        Sleep(frame_delay);

        // 检查鼠标点击，如果检测到则跳转到主界面
        if (MouseHit()) {
            // 获取鼠标消息
            ExMessage msg = getmessage(EX_MOUSE);
            if (msg.message == WM_LBUTTONDOWN) {
                // 结束批量绘制
                EndBatchDraw();
                // 返回主界面
                menuView();
                // 停止播放背景音乐
                Mix_HaltMusic();
                freeMusic(bgMusic);
                return;
            }
        }
    }
    menuView();
    // 结束批量绘制
    EndBatchDraw();
    // 停止播放背景音乐
    Mix_HaltMusic();
    freeMusic(bgMusic);
}

//Mygo胜利结算
void mygo_win() {
    // 使用双缓冲技术
    BeginBatchDraw();
    centerWindow();
    // 总帧数和每秒帧数
    const int total_frames = 2165;
    const double fps = 23.967;
    const int frame_delay = static_cast<int>(1000 / fps); // 每帧延迟时间

    // 初始化SDL2_mixer
    if (!initSDL2Mixer()) {
        return;
    }

    Mix_Music* bgMusic = loadMusic("res/bg2.mp3");
    if (bgMusic == nullptr) {
        std::cerr << "背景音乐加载失败" << std::endl;
        return;
    }

    Mix_VolumeMusic(bgVolume); // 设置背景音乐音量
    Mix_PlayMusic(bgMusic, -1); // 循环播放背景音乐

    for (int i = 0; i < total_frames; ++i) {
        stringstream ss;
        ss << "mygocar/Mygo" << setw(4) << setfill('0') << i << ".jpg";
        string filename = ss.str();

        // 转换文件名为LPCTSTR类型
        wstring wide_filename = wstring(filename.begin(), filename.end());
        LPCTSTR w_filename = wide_filename.c_str();

        // 测试
        //cout << "加载图片: " << filename << endl;

        // 加载图片
        IMAGE img;
        loadimage(&img, w_filename);

        // 检查图片是否加载成功
        if (img.getwidth() == 0 || img.getheight() == 0) {
            cout << "图片加载失败: " << filename << endl;
            continue;
        }

        // 显示图片
        putimage(0, 0, &img);
        //cout << "显示图片: " << filename << endl;

        // 刷新屏幕
        FlushBatchDraw();

        // 等待下一帧
        Sleep(frame_delay);

        // 检查鼠标点击，如果检测到则跳转到主界面
        if (MouseHit()) {
            // 获取鼠标消息
            ExMessage msg = getmessage(EX_MOUSE);
            if (msg.message == WM_LBUTTONDOWN) {
                // 结束批量绘制
                EndBatchDraw();
                // 返回主界面
                menuView();
                // 停止播放背景音乐
                Mix_HaltMusic();
                freeMusic(bgMusic);
                return;
            }
        }
    }
    menuView();
    // 结束批量绘制
    EndBatchDraw();
    // 停止播放背景音乐
    Mix_HaltMusic();
    freeMusic(bgMusic);
}

// 结算界面
void winView(int i) {
    if (i == 1) {
        initgraph(1280, 800);
        mygo_win();
    }
    else if (i == 2) {
        initgraph(1280, 800);
        gbc_win();
    }
}

//开头动画
void logo() {
    // 使用双缓冲技术
    BeginBatchDraw();
    centerWindow();
    // 总帧数和每秒帧数
    const int total_frames = 445;
    const double fps = 45;
    const int frame_delay = static_cast<int>(1000 / fps); // 每帧延迟时间

    

    for (int i = 0; i < total_frames; ++i) {
        stringstream ss;
        ss << "logo/Logo" << setw(4) << setfill('0') << i << ".jpg";
        string filename = ss.str();

        // 转换文件名为LPCTSTR类型
        wstring wide_filename = wstring(filename.begin(), filename.end());
        LPCTSTR w_filename = wide_filename.c_str();

        // 测试
        //cout << "加载图片: " << filename << endl;

        // 加载图片
        IMAGE img;
        loadimage(&img, w_filename);

        // 检查图片是否加载成功
        if (img.getwidth() == 0 || img.getheight() == 0) {
            cout << "图片加载失败: " << filename << endl;
            continue;
        }

        // 显示图片
        putimage(0, 0, &img);
        //cout << "显示图片: " << filename << endl;

        // 刷新屏幕
        FlushBatchDraw();

        // 等待下一帧
        Sleep(frame_delay);

        // 检查鼠标点击，如果检测到则跳转到主界面
        if (MouseHit()) {
            // 获取鼠标消息
            ExMessage msg = getmessage(EX_MOUSE);
            if (msg.message == WM_LBUTTONDOWN) {
                // 结束批量绘制
                EndBatchDraw();
                // 返回主界面
                menuView();                              
                return;
            }
        }
    }
    menuView();
    // 结束批量绘制
    EndBatchDraw();
}

// 滚动字幕
void displayScrollingCredits() {
    // 初始化图形窗口，设置分辨率为1280x800
    initgraph(1280, 800);
    // 设置窗口居中（此功能可能需要具体实现，根据图形库不同）
    // centerWindow();
    // 开启双缓冲绘图模式
    BeginBatchDraw();

    // 字幕初始位置
    int yOffset = 800;

    // 主循环
    while (true) {
        // 清屏
        cleardevice();

        // 设置字体和颜色
        settextstyle(30, 0, _T("Arial"));
        settextcolor(WHITE);

        // 计算每行文字的高度
        int lineHeight = textheight(_T("A"));

        // 增大行距
        int lineSpacing = lineHeight + 20;

        // 绘制字幕
        for (size_t i = 0; i < credits.size(); ++i) {
            int yPosition = yOffset + i * lineSpacing;
            int textWidth = textwidth(credits[i]);
            int xPosition = (getmaxx() - textWidth) / 2; // 计算居中位置
            outtextxy(xPosition, yPosition, credits[i]);
        }

        // 更新位置
        yOffset -= 2; // 每次更新向上移动2个像素

        // 循环字幕
        if (yOffset + credits.size() * lineSpacing < 0) {
            yOffset = 800;
        }

        // 检测用户输入
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) { // 按下 ESC 键
                break;
            }
        }
        // 检测鼠标点击
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                break;
            }
        }

        // 延时控制速度
        Sleep(30);

        // 显示更新
        FlushBatchDraw();
    }

    // 关闭图形窗口
    closegraph();

    // 进入 setView 界面
    setView();
}


//开始-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    initgraph(1280, 800); // 初始化图形窗口
    
    logo();
    //测试
    //gbc_win();
    //mygo_win();
    return 0;
}
