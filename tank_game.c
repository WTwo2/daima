#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600
#define TANK_SIZE 30
#define BULLET_SIZE 5
#define ENEMY_MAX 5

// 坦克结构体
struct Tank {
    int x, y;
    int direction; // 0:上 1:下 2:左 3:右 
    int speed;
    int color;
};

// 子弹结构体 
struct Bullet {
    int x, y;
    int direction;
    int active;
};

struct Tank player;
struct Tank enemies[ENEMY_MAX];
struct Bullet bullets[10];
int score = 0;

void initGame() {
    // 初始化玩家坦克
    player.x = WIDTH/2;
    player.y = HEIGHT-50;
    player.direction = 0;
    player.speed = 5;
    player.color = GREEN;

    // 初始化敌人坦克
    for(int i=0; i<ENEMY_MAX; i++) {
        enemies[i].x = 50 + i*150;
        enemies[i].y = 50;
        enemies[i].direction = 1;
        enemies[i].speed = 3;
        enemies[i].color = RED;
    }

    // 初始化子弹
    for(int i=0; i<10; i++) {
        bullets[i].active = 0;
    }
}

void drawTank(struct Tank t) {
    setcolor(t.color);
    rectangle(t.x-TANK_SIZE/2, t.y-TANK_SIZE/2, 
             t.x+TANK_SIZE/2, t.y+TANK_SIZE/2);
    // 绘制炮管
    switch(t.direction) {
        case 0: line(t.x, t.y, t.x, t.y-TANK_SIZE); break;
        case 1: line(t.x, t.y, t.x, t.y+TANK_SIZE); break;
        case 2: line(t.x, t.y, t.x-TANK_SIZE, t.y); break;
        case 3: line(t.x, t.y, t.x+TANK_SIZE, t.y); break;
    }
}

void moveBullets() {
    for(int i=0; i<10; i++) {
        if(bullets[i].active) {
            setcolor(WHITE);
            circle(bullets[i].x, bullets[i].y, BULLET_SIZE);
            
            // 移动子弹
            switch(bullets[i].direction) {
                case 0: bullets[i].y -= 10; break;
                case 1: bullets[i].y += 10; break;
                case 2: bullets[i].x -= 10; break;
                case 3: bullets[i].x += 10; break;
            }
            
            // 边界检测
            if(bullets[i].x < 0 || bullets[i].x > WIDTH ||
               bullets[i].y < 0 || bullets[i].y > HEIGHT) {
                bullets[i].active = 0;
            }
        }
    }
}

void enemyAI() {
    static int counter = 0;
    counter++;
    
    for(int i=0; i<ENEMY_MAX; i++) {
        // 随机改变方向
        if(counter % 50 == 0) {
            enemies[i].direction = rand()%4;
        }
        
        // 移动敌人
        switch(enemies[i].direction) {
            case 0: enemies[i].y -= enemies[i].speed; break;
            case 1: enemies[i].y += enemies[i].speed; break;
            case 2: enemies[i].x -= enemies[i].speed; break;
            case 3: enemies[i].x += enemies[i].speed; break;
        }
        
        // 边界限制
        if(enemies[i].x < 50) enemies[i].x = 50;
        if(enemies[i].x > WIDTH-50) enemies[i].x = WIDTH-50;
        if(enemies[i].y < 50) enemies[i].y = 50;
        if(enemies[i].y > HEIGHT/2) enemies[i].y = HEIGHT/2;
    }
}

void checkCollisions() {
    // 子弹与敌人碰撞检测
    for(int i=0; i<10; i++) {
        if(bullets[i].active) {
            for(int j=0; j<ENEMY_MAX; j++) {
                if(abs(bullets[i].x - enemies[j].x) < TANK_SIZE/2 &&
                   abs(bullets[i].y - enemies[j].y) < TANK_SIZE/2) {
                    score += 100;
                    bullets[i].active = 0;
                    enemies[j].x = 50 + j*150;
                    enemies[j].y = 50;
                }
            }
        }
    }
}

int main() {
    initgraph(WIDTH, HEIGHT);
    initGame();
    
    while(1) {
        cleardevice();
        
        // 处理输入
        if(kbhit()) {
            char key = getch();
            switch(key) {
                case 'w': player.direction = 0; player.y -= player.speed; break;
                case 's': player.direction = 1; player.y += player.speed; break;
                case 'a': player.direction = 2; player.x -= player.speed; break;
                case 'd': player.direction = 3; player.x += player.speed; break;
                case ' ': // 发射子弹
                    for(int i=0; i<10; i++) {
                        if(!bullets[i].active) {
                            bullets[i].x = player.x;
                            bullets[i].y = player.y;
                            bullets[i].direction = player.direction;
                            bullets[i].active = 1;
                            break;
                        }
                    }
                    break;
                case 27: exit(0); // ESC退出
            }
        }
        
        // 游戏逻辑
        enemyAI();
        moveBullets();
        checkCollisions();
        
        // 绘制元素
        drawTank(player);
        for(int i=0; i<ENEMY_MAX; i++) {
            drawTank(enemies[i]);
        }
        
        // 显示分数
        char scoreText[20];
        sprintf(scoreText, "Score: %d", score);
        outtextxy(10, 10, scoreText);
        
        Sleep(50);
    }
    
    closegraph();
    return 0;
}
