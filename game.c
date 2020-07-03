/*****************************************************************************************************/

#include "include.h"
#include "game.h"


/********************全局变量区************************/
int letterCount = 100;  //下落字母的总数量
int beginFlag = 1;  //游戏何时结束，0-表示结束
int downCount;  //下落字母数量统计
int dropLetter; //打掉字母数量统计
int errorCount; //按键错误统计
int speed = 500;    //下落速度
int bulletSpeed = 30;   //子弹速度
//多线程-线程锁
CRITICAL_SECTION csCursor;//光标锁


Letter* letters;    //创建字母序列
int* vis;           //标记字母是否出现过
int* visBullet;     //标记对应字母是否有子弹正在发射
int* bulletflag;    //子弹编号

//初始化全局变量，和开辟对应数组空间
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
//初始化字母序列
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

/***************功能函数***************/
//定位到屏幕的某个位置
void gotoxy(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(handle, coord);
}

//绘制字母
void drawLetter(char ch, int x, int y)
{
    EnterCriticalSection(&csCursor);//互斥访问变量
    gotoxy(x, y);
    printf("%c", ch);
    LeaveCriticalSection(&csCursor);
}
//设置窗口标题
void setTitle(void* p)
{
    while (1)
    {
        char str[30] = "";
        sprintf(str, "title 总字母:%d 下落字母:%d 击落:%d", letterCount, downCount, dropLetter);
        system(str);
    }
}
DWORD WINAPI runBullet(void* p)//子弹发射过程
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
//letterMoving:让所有字母下降一个位置
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
//字母不断下降
DWORD WINAPI runLetter(void* p)
{
    while (letters[letterCount - 1].y < 25)
    {
        letterMoving();
        Sleep(speed);
    }
    beginFlag = 0;
}

//隐藏控制台光标
void hideCursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = 0; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}
//开始游戏提示
void gameBegin()
{
    printf("-------------------------------------\n");
    printf("    一个简单的打字游戏，按ESC退出\n");
    printf("       建议提前切换成英文输入法\n");
    printf("-------------------------------------\n");

    int type;
    int flag = 1;
    while (flag)
    {
        printf("1-简单\n2-中等\n3-困难\n4-魔鬼\n");
        printf("请选择你要打字的等级:");
        while (scanf("%d", &type) != 1)
        {
            system("cls");
            scanf("%*s");
            printf("您输入的值有问题，请输入数字！\n");
            printf("1-简单\n2-中等\n3-困难\n4-魔鬼\n");
            printf("请选择你要打字的等级:");
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
            printf("你选择的值有误，请重新选择\n");
        }
    }
    printf("请输入你要打字的数量:");
    while (scanf("%d", &letterCount) != 1)
    {
        system("cls");
        scanf("%*s");
        printf("您输入的值有问题，请输入数字！\n");
        printf("请输入你要打字的数量:");
    }
    printf("字母已准备就绪,");
    system("pause");
    system("cls");
}
//结束游戏提示
void gameOver()
{
    gotoxy(30, 12);
    printf("总字母:%d个 您击落:%d个 错误按键:%d次", letterCount, dropLetter, errorCount);
}

int gaming()
{
    system("title 打字游戏");
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
