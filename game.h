#pragma once

#define LENMAX 5//字符串最长大小
#define LEVELMAX 4//最大关卡数
#define LETTERCOUNT 30//打字数量
//字母结构体
typedef struct tag_letter
{
    int x, y;   //字母下落位置
    char ch;    //字母
    char str[LENMAX];   //字符串 sdf
    int len;    
    int life;   //字母是否存活
}Letter;

/* a)屏幕下落字母，或单词，下方有一个横线，在字母或单词落到横线之前键盘正确输入该字母或单词，则消除该字母或单词，此过程中允许错误输入。如果字母或单词落到横线上，game over */
int gaming(users* curr);
/*b)	下落位置（屏幕上的横坐标）随机，根据下落速度和单词长度、大小写等设计不同难度关卡，游戏开始前玩家从第一关开始，通关后进入第二关，以此类推。*/
void initLetters();

void init();
void gotoxy(int x, int y);
void drawLetter(char ch, int x, int y);
void setTitle(void* p);
DWORD WINAPI runBullet(void* p);
int letterMoving();
DWORD WINAPI runLetter(void* p);
void hideCursor();
void gameBegin();
void gameOver(users* curr);

//char* RandomString(char* string, int len);
