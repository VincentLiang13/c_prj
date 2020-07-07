#pragma once

#define LENMAX 5//�ַ������С
#define LEVELMAX 4//���ؿ���
#define LETTERCOUNT 30//��������
//��ĸ�ṹ��
typedef struct tag_letter
{
    int x, y;   //��ĸ����λ��
    char ch;    //��ĸ
    char str[LENMAX];   //�ַ��� sdf
    int len;    
    int life;   //��ĸ�Ƿ���
}Letter;

/* a)��Ļ������ĸ���򵥴ʣ��·���һ�����ߣ�����ĸ�򵥴��䵽����֮ǰ������ȷ�������ĸ�򵥴ʣ�����������ĸ�򵥴ʣ��˹���������������롣�����ĸ�򵥴��䵽�����ϣ�game over */
int gaming(users* curr);
/*b)	����λ�ã���Ļ�ϵĺ����꣩��������������ٶȺ͵��ʳ��ȡ���Сд����Ʋ�ͬ�Ѷȹؿ�����Ϸ��ʼǰ��Ҵӵ�һ�ؿ�ʼ��ͨ�غ����ڶ��أ��Դ����ơ�*/
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
