#pragma once

#define LENMAX 5//�ַ������С


//��ĸ�ṹ��
typedef struct tag_letter
{
    int x, y;   //��ĸ����λ��
    char ch;    //��ĸ
    char str[LENMAX];   //�ַ��� sdf
    int len;    
    int life;   //��ĸ�Ƿ���
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

char* RandomString(char* string, int len);
