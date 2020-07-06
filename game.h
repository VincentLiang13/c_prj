#pragma once

#define LENMAX 5//字符串最长大小
#define LEVELMAX 4//最大关卡数
#define LETTERCOUNT 20//打字数量
//字母结构体
typedef struct tag_letter
{
    int x, y;   //字母下落位置
    char ch;    //字母
    char str[LENMAX];   //字符串 sdf
    int len;    
    int life;   //字母是否存活
}Letter;

int gaming(users* curr);


void init();
void initLetters();
void gotoxy(int x, int y);
void drawLetter(char ch, int x, int y);
void setTitle(void* p);
DWORD WINAPI runBullet(void* p);
void letterMoving();
DWORD WINAPI runLetter(void* p);
void hideCursor();
void gameBegin();
void gameOver(users* curr);

//char* RandomString(char* string, int len);
