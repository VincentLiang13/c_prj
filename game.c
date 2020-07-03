/*****************************************************************************************************/

#include "include.h"
#include "game.h"


/********************ȫ�ֱ�����************************/
int letterCount = 100;  //������ĸ��������
int beginFlag = 1;  //��Ϸ��ʱ������0-��ʾ����
int downCount;  //������ĸ����ͳ��
int dropLetter; //�����ĸ����ͳ��
int errorCount; //��������ͳ��
int speed = 500;    //�����ٶ�
int bulletSpeed = 30;   //�ӵ��ٶ�
//���߳�-�߳���
CRITICAL_SECTION csCursor;//�����


Letter* letters;    //������ĸ����
int* vis;           //�����ĸ�Ƿ���ֹ�
int* visBullet;     //��Ƕ�Ӧ��ĸ�Ƿ����ӵ����ڷ���
int* bulletflag;    //�ӵ����

//��ʼ��ȫ�ֱ������Ϳ��ٶ�Ӧ����ռ�
void init()
{
    downCount = 0;
    dropLetter = 0;
    errorCount = 0;
    vis = (int*)malloc(sizeof(int) * letterCount);
    visBullet = (int*)malloc(sizeof(int) * letterCount);
    bulletflag = (int*)malloc(sizeof(int) * letterCount);
    memset(vis, 0, sizeof(int) * letterCount);
    memset(visBullet, 0, sizeof(int) * letterCount);
    for (int i = 0; i < letterCount; i++)
    {
        bulletflag[i] = i;
    }
}
//��ʼ����ĸ����
void initLetters()
{
    letters = (struct tag_letter*)malloc(sizeof(struct tag_letter) * letterCount);
    srand((unsigned)time(NULL));
    for (int i = 0; i < letterCount; i++)
    {
        letters[i].ch = rand() % 26 + 'A';
        letters[i].x = rand() % 80;
        letters[i].y = -i * 2;
        letters[i].life = 1;
    }
}

/***************���ܺ���***************/
//��λ����Ļ��ĳ��λ��
void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(handle, coord);
}

//������ĸ
void drawLetter(char ch, int x, int y)
{
    EnterCriticalSection(&csCursor);//������ʱ���
    gotoxy(x, y);
    printf("%c", ch);
    LeaveCriticalSection(&csCursor);
}
//���ô��ڱ���
void setTitle(void* p)
{
    while (1)
    {
        char str[30] = "";
        sprintf(str, "title ����ĸ:%d ������ĸ:%d ����:%d", letterCount, downCount, dropLetter);
        system(str);
    }
}
DWORD WINAPI runBullet(void* p)//�ӵ��������
{
    int letterId = *(int*)p;
    if (!visBullet[letterId])
    {
        visBullet[letterId] = 1;
    }
    int y = 25;
    int x = letters[letterId].x;
    while (y > letters[letterId].y)
    {

        drawLetter(' ', x, y);
        drawLetter(12, x, y - 1);

        Sleep(bulletSpeed);
        y--;
    }
    drawLetter(' ', x, y);
    letters[letterId].life = 0;
    drawLetter(' ', letters[letterId].x, letters[letterId].y);
}
//letterMoving:��������ĸ�½�һ��λ��
void letterMoving()
{
    int cnt = 0;
    for (int i = 0; i < letterCount; i++)
    {
        if (letters[i].life == 1)
        {
            cnt++;
        }
        if (letters[i].y < 25 && letters[i].y >= 0 && letters[i].life == 1)
        {
            if (!vis[i])
            {
                downCount++;
                vis[i] = 1;
            }
            drawLetter(' ', letters[i].x, letters[i].y);
            drawLetter(letters[i].ch, letters[i].x, letters[i].y + 1);
        }
        else if (letters[i].y > 25)
        {
            letters[i].life = 0;
        }
        else if (letters[i].y == 25)
        {
            drawLetter(' ', letters[i].x, letters[i].y);
        }
        letters[i].y++;
    }
    if (cnt == 0)
    {
        beginFlag = 0;
    }
}
//��ĸ�����½�
DWORD WINAPI runLetter(void* p)
{
    while (letters[letterCount - 1].y < 25)
    {
        letterMoving();
        Sleep(speed);
    }
    beginFlag = 0;
}

//���ؿ���̨���
void hideCursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//��ȡ����̨�����Ϣ
    CursorInfo.bVisible = 0; //���ؿ���̨���
    SetConsoleCursorInfo(handle, &CursorInfo);//���ÿ���̨���״̬
}
//��ʼ��Ϸ��ʾ
void gameBegin()
{
    printf("-------------------------------------\n");
    printf("    һ���򵥵Ĵ�����Ϸ����ESC�˳�\n");
    printf("       ������ǰ�л���Ӣ�����뷨\n");
    printf("-------------------------------------\n");

    int type;
    int flag = 1;
    while (flag)
    {
        printf("1-��\n2-�е�\n3-����\n4-ħ��\n");
        printf("��ѡ����Ҫ���ֵĵȼ�:");
        while (scanf("%d", &type) != 1)
        {
            system("cls");
            scanf("%*s");
            printf("�������ֵ�����⣬���������֣�\n");
            printf("1-��\n2-�е�\n3-����\n4-ħ��\n");
            printf("��ѡ����Ҫ���ֵĵȼ�:");
        }
        switch (type)
        {
        case 1:
            speed = 500;
            bulletSpeed = 30;
            flag = 0;
            break;
        case 2:
            speed = 350;
            bulletSpeed = 25;
            flag = 0;
            break;
        case 3:
            speed = 250;
            bulletSpeed = 20;
            flag = 0;
            break;
        case 4:
            speed = 150;
            bulletSpeed = 10;
            flag = 0;
            break;
        default:
            printf("��ѡ���ֵ����������ѡ��\n");
        }
    }
    printf("��������Ҫ���ֵ�����:");
    while (scanf("%d", &letterCount) != 1)
    {
        system("cls");
        scanf("%*s");
        printf("�������ֵ�����⣬���������֣�\n");
        printf("��������Ҫ���ֵ�����:");
    }
    printf("��ĸ��׼������,");
    system("pause");
    system("cls");
}
//������Ϸ��ʾ
void gameOver()
{
    gotoxy(30, 12);
    printf("����ĸ:%d�� ������:%d�� ���󰴼�:%d��", letterCount, dropLetter, errorCount);
}

int gaming()
{
    system("title ������Ϸ");
    gameBegin();
    hideCursor();
    InitializeCriticalSection(&csCursor);
    init();
    initLetters();
    CreateThread(NULL, 0, runLetter, NULL, 0, NULL);
    _beginthread(setTitle, 0, NULL);
    while (beginFlag)
    {
        if (_kbhit())
        {
            char ch = _getch();
            ch = toupper(ch);
            if (ch == 27)
            {
                break;
            }
            int flag = 1;
            for (int i = 0; i < letterCount; i++)
            {
                if (letters[i].y < 25 && letters[i].y >= 0 && letters[i].life == 1 && letters[i].ch == ch && visBullet[i] == 0)
                {
                    flag = 0;
                    dropLetter++;
                    CreateThread(NULL, 0, runBullet, &bulletflag[i], 0, NULL);
                    break;
                }
            }
            if (flag) errorCount++;
        }
    }
    gameOver();
    InitializeCriticalSection(&csCursor);
    system("\n\n\npause");
    return 0;
}
