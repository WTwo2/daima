#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 20
#define HEIGHT 10

int playerX = WIDTH/2;
int playerY = HEIGHT-1;
int bulletX = -1, bulletY = -1;
int enemyX = 5, enemyY = 0;
int score = 0;

void draw() {
    system("cls");
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == playerX && y == playerY) printf("@");
            else if (x == bulletX && y == bulletY) printf("*");
            else if (x == enemyX && y == enemyY) printf("V");
            else printf(" ");
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

int main() {
    srand(time(NULL));
    while (1) {
        if (_kbhit()) {
            char c = _getch();
            if (c == 'a' && playerX > 0) playerX--;
            else if (c == 'd' && playerX < WIDTH-1) playerX++;
            else if (c == ' ') { bulletX = playerX; bulletY = playerY - 1; }
        }

        // 更新子弹位置
        if (bulletY != -1) {
            bulletY--;
            if (bulletY < 0) { bulletX = -1; bulletY = -1; }
        }

        // 更新敌机位置（速度减半）
        static int enemyCounter = 0;
        if (enemyCounter++ % 2 == 0) {
            enemyY++;
            if (enemyY >= HEIGHT) {
                enemyX = rand() % WIDTH;
                enemyY = 0;
            }
        }

        // 检测碰撞
        if (bulletX == enemyX && bulletY == enemyY) {
            score += 10;
            enemyX = rand() % WIDTH;
            enemyY = 0;
            bulletX = -1;
            bulletY = -1;
        }

        draw();
        Sleep(100);
    }
    return 0;
}
