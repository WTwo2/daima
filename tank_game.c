#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define PDCURSES
#include "pdcurses.h"  // 使用PDCurses头文件
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20

int gameOver;
int score;
int x, y; // 蛇头坐标
int fruitX, fruitY; // 食物坐标
int tailX[100], tailY[100];
int nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

// 初始化游戏
void Setup() {
    gameOver = 0;
    dir = STOP;
    x = WIDTH / 2;
    y = HEIGHT / 2;
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    score = 0;
}

// 绘制游戏界面
void Draw() {
    clear(); // curses清屏
    
    // 绘制顶部边界
    for (int i = 0; i < WIDTH+2; i++)
        printf("#");
    printf("\n");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0) mvaddch(i+1, j, '#'); // 左侧边界
            
            // 绘制蛇头
            if (i == y && j == x) mvaddch(i+1, j+1, 'O' | A_BOLD);
            // 绘制食物
            else if (i == fruitY && j == fruitX) mvaddch(i+1, j+1, 'F' | COLOR_PAIR(1));
            else {
                int printTail = 0;
                // 绘制蛇身
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        mvaddch(i+1, j+1, 'o');
                        printTail = 1;
                    }
                }
                if (!printTail) mvaddch(i+1, j+1, ' ');
            }
            
            if (j == WIDTH-1) mvaddch(i+1, j+2, '#'); // 右侧边界
        }
        printf("\n");
    }

    // 绘制底部边界
    for (int i = 0; i < WIDTH+2; i++)
        printf("#");
    printf("\n");
    
    attron(COLOR_PAIR(2));
    mvprintw(HEIGHT+2, 0, "分数: %d", score);
    attroff(COLOR_PAIR(2));
    refresh();
}

// 处理输入
void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': dir = LEFT; break;
            case 'd': dir = RIGHT; break;
            case 'w': dir = UP; break;
            case 's': dir = DOWN; break;
            case 'x': gameOver = 1; break;
        }
    }
}

// 更新游戏逻辑
void Logic() {
    // 保存前一节位置
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    
    // 更新蛇身位置
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // 移动蛇头
    switch (dir) {
        case LEFT:  x--; break;
        case RIGHT: x++; break;
        case UP:    y--; break;
        case DOWN:  y++; break;
    }

    // 碰撞检测
    if (x >= WIDTH) x = 0; else if (x < 0) x = WIDTH - 1;
    if (y >= HEIGHT) y = 0; else if (y < 0) y = HEIGHT - 1;

    // 检查自我碰撞
    for (int i = 0; i < nTail; i++) {
        if (tailX[i] == x && tailY[i] == y)
            gameOver = 1;
    }

    // 吃到食物
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        nTail++;
    }
}

int main() {
    initscr();
    start_color();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    timeout(100);
    
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    
    srand(time(NULL));
    Setup();
    
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // 控制游戏速度
    }
    
    endwin();
    printf("游戏结束！最终得分：%d\n", score);
    return 0;
}
