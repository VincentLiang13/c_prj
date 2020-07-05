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
    memset(vis, 0, sizeof(int) * letterCount);//初始化函数。作用是将某一块内存中的内容全部设置为指定的值
    memset(visBullet, 0, sizeof(int) * letterCount);
    for (int i = 0; i < letterCount; i++)
    {
        bulletflag[i] = i;
    }
}

//初始化字符串序列sdf
void initLetters()
{        
    int len;//字符串长度
    letters = (struct tag_letter*)malloc(sizeof(struct tag_letter) * letterCount);
    //srand((unsigned)time(NULL));//时间种子
    //Sleep(200);
    for (int i = 0; i < letterCount; i++)
    {
        len = rand() % LENMAX;//长度为len包含\0 的随机字符串
        if (0 == len || 1 == len) len = 2;//至少包含一个有效字符

        letters[i].len = len;//记录字符串长度
        RandomString(letters[i].str, len);
        //letters[i].ch = rand() % 26 + 'A';
        letters[i].x = rand() % 80;
        letters[i].y = -i * 2;
        letters[i].life = 1;
    }
}
/*//初始化字母序列
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
*/
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
//绘制字符串sdf
void drawString(char str[], int x, int y)
{
    EnterCriticalSection(&csCursor);//互斥访问变量
    gotoxy(x, y);
    printf("%s", str);
    LeaveCriticalSection(&csCursor);
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
DWORD WINAPI runBullet(void* p)//子弹发射过程 p就是&bulletflag[i] 调用时已经说明输入完全正确
{
    int letterId = *(int*)p;//letterId 就是 i
    if (!visBullet[letterId] == 1)//每个子弹初始为1（未击中）
    {
        visBullet[letterId] = 1;
    }
    int y = 25;
    int x = letters[letterId].x;
    while (y > letters[letterId].y)//直到子弹于字母y坐标相等
    {
        drawLetter(' ', x, y);
        drawLetter(12, x, y - 1);

        Sleep(bulletSpeed);
        y--;
    }
    drawLetter(' ', x, y);//一发入魂
    letters[letterId].life = 0;//字母生命取0
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
            for (int j = 0; j < letters[i].len; j++)
            {
                drawLetter(' ', letters[i].x + j, letters[i].y);//需要知道字符串长度  4个空格
            }
            //drawLetter(letters[i].ch, letters[i].x, letters[i].y + 1);
            drawString(letters[i].str, letters[i].x, letters[i].y + 1);//sdf
        }
        else if (letters[i].y > 25)
        {
            letters[i].life = 0;
        }
        else if (letters[i].y == 25)
        {
            for (int j = 0; j < letters[i].len; j++)
            {
                drawLetter(' ', letters[i].x + j, letters[i].y);//需要知道字符串长度  4个空格
            }
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
void gameOver(users* curr)
{
    curr->highMark = dropLetter;
    //curr->highLevel = level;
    gotoxy(30, 12);
    printf("总字母:%d个 您击落:%d个 错误按键:%d次", letterCount, dropLetter, errorCount);

}


int gaming(users* curr)
{
    int i = 0;
    int j = 0;
    system("title 打字游戏");
    gameBegin();
    hideCursor();
    InitializeCriticalSection(&csCursor);//临界区锁
    init();
    initLetters();
    CreateThread(NULL, 0, runLetter, NULL, 0, NULL);
    _beginthread(setTitle, 0, NULL);
    while (beginFlag)
    {
        if (_kbhit())//获取键盘按键
        {
            char ch = _getch();//不用 enter 用户从键盘输入

            //ch = toupper(ch);//toupper函数用于把小写字母转换为大写字母。
            if (ch == 27)//esc ascii
            {
                break;
            }
            int flag = 1;
            int nextCharFlag = 0;
            for (;i < letterCount && nextCharFlag == 0; i++)//循环判断字母是否被打掉 没有则errorCount++
            {
                int strFlag = 0;//字符串输入正确判断标志
                //int j = 1;//初值不等于0==strCnt

                //字符在显示区存在，生命存在，子弹击中
                if (letters[i].y < 25 && letters[i].y >= 0 && visBullet[i] == 0)//短路原则
                {
                    nextCharFlag = 1;//需要下一个输入
                    while (1)//'0'? //字符串的正确判断
                    {
                        if (letters[i].str[j] == ch) 
                        { 
                            flag = 0;
                            dropLetter++;
                            j++;
                            break;
                        }//如果对，判断下一个
                        else break;//flag = 1

                        if (j == letters[i].len)//全对
                        {
                            i = 0;
                            j = 0;
                            CreateThread(NULL, 0, runBullet, &bulletflag[i], 0, NULL);// &bulletflag[i]为参数  runBullet为线程地址
                            break;
                        }
                        //ch = _getch();//如果正确，则再次输入 这时无法esc
                    }
                }
            }
            if (flag) errorCount++;//没找到或者输入错
        }
    }
    gameOver(curr);
    InitializeCriticalSection(&csCursor);
    system("\n\n\npause");
    return 0;
}


//产生长度为length的随机字符串  
char* RandomString(char*string,int len)
{
    int flag, i;

    //srand((unsigned)time(NULL));
    for (i = 0; i < len - 1; i++)
    {
        flag = rand() % 1;//改难度 %3
        switch (flag)
        {
        case 0:
            string[i] = 'a' + rand() % 26;
            break;
        case 1:
            string[i] = 'A' + rand() % 26;
            break;
        case 2:
            string[i] = '0' + rand() % 10;
            break;
        default:
            string[i] = 'x';
            break;
        }
    }
    string[len - 1] = '\0';

    return string;
}