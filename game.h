#pragma once

#define LENMAX 5//�ַ������С
#define LEVELMAX 4//���ؿ���
#define LETTERCOUNT 20//��������
//��ĸ�ṹ��
typedef struct tag_letter
{
    int x, y;   //��ĸ����λ��
    char ch;    //��ĸ
    char str[LENMAX];   //�ַ��� sdf
    int len;    
    int life;   //��ĸ�Ƿ���
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
