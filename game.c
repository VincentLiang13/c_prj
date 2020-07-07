/*****************************************************************************************************/

#include "include.h"
#include "game.h"
#include "interface.h"

extern int Score_Rank(users* a);
/********************全局变量区************************/
int letterCount = LETTERCOUNT;  //下落字母的总数量
int beginFlag = 1;  //游戏何时结束，0-表示结束
int downCount;  //下落字母数量统计
int dropLetter; //打掉字母数量统计
int errorCount; //按键错误统计
int speed = 500;    //下落速度
int bulletSpeed = 30;   //子弹速度
int level = 1;
int Lable = 0;
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
        char str[50] = "";
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
    return 0;
}
//letterMoving:让所有字母下降一个位置
int letterMoving()
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
        else if (letters[i].y > 25 && letters[i].life == 0)//被打掉
        {
            letters[i].life = 0;
        }
        else if (letters[i].y > 25 && letters[i].life != 0)//没打掉
        {
            beginFlag = -1;
            return 0;
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
    return 0;
}
//字母不断下降
DWORD WINAPI runLetter(void* p)
{
    while (letters[letterCount - 1].y < 25 && beginFlag != -1)
    {
        letterMoving();
        Sleep(speed);
    }
 
    if (beginFlag == -1) 
    {   
        Lable = -1;
        return Lable;
    }
    beginFlag = 0;
    return 0;
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
    int flag = 1;
    while (flag)
    {
        switch (level)
        {
        case 1:
            speed = 180;
            bulletSpeed = 10;
            flag = 0;
            break;
        case 2:
            speed = 170;
            bulletSpeed = 10;
            flag = 0;
            break;
        case 3:
            speed = 160;
            bulletSpeed = 10;
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
    printf("字母已准备就绪,");
    system("pause");
    system("cls");
}
//结束游戏提示
void gameOver(users* curr)
{
    system("cls");
    gotoxy(30, 12);
    curr->highMark = dropLetter - errorCount;
    curr->highLevel = level;
    printf("总字母:%d个 您击落:%d个 错误按键:%d次", letterCount, dropLetter, errorCount);
    printf("总分数：%d", curr->highMark);

}

int gaming(users* curr)
{
    vis = (int*)malloc(sizeof(int) * letterCount);
    visBullet = (int*)malloc(sizeof(int) * letterCount);
    bulletflag = (int*)malloc(sizeof(int) * letterCount);
    printf("-------------------------------------\n");
    printf("                     这是一个打字游戏\n");
    printf("                    要使用英文输入法 \n");
    printf("                 by 2191710109 梁嘉栋\n");
    printf("-------------------------------------\n");

for (level = 1; level <= 4; level++)
{   
    Lable = 0;
    beginFlag = 1;
    system("title 打字游戏");
    gameBegin();
    hideCursor();
    InitializeCriticalSection(&csCursor);
    init();
    initLetters();
    CreateThread(NULL, 0, runLetter, NULL, 0, NULL);
    _beginthread(setTitle, 0, NULL);
    gotoxy(0, 26);
    printf("-------------------------------------------------------------------------------");
    while (beginFlag)
    {
            if (_kbhit())
            {
                char ch = _getch();
                ch = toupper(ch);
                if (ch == 27)  break;

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
                if (Lable == -1) break;
            }
    }
    if (Lable == -1)  break;
    system("cls");
    gotoxy(30, 14);
    if (level <4)  printf("总分数：%d  下一个关卡：%d \n", dropLetter- errorCount,level+1);
    else           printf("恭喜通关！");
    gotoxy(30, 16);
    system("\n\n\npause");
    system("cls");
/*
    free(vis);
    free(visBullet);
    free(bulletflag);
    free(letters);
*/
    
}
    system("cls");
    curr->highMark = dropLetter - errorCount;
    curr->highLevel = level;
    //printf("总字母:%d个 您击落:%d个 错误按键:%d次\n", letterCount, dropLetter, errorCount);
    int rank = Score_Rank(curr);    
    //InitializeCriticalSection(&csCursor);
    gotoxy(30, 14);
    printf("总分数：%d  最高关卡：%d  排名：%d\n", curr->highMark,curr->highLevel, rank);

    gotoxy(30, 16);
    free(vis);
    free(visBullet);
    free(bulletflag);
    free(letters);
    system("\n\n\npause");


    return 0;
}
