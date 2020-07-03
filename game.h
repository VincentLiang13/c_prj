#pragma once

//字母结构体
typedef struct tag_letter
{
    int x, y;   //字母下落位置
    char ch;    //字母
    int life;   //字母是否存活
}Letter;

int gaming();

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
void gameOver();
