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
    memset(vis, 0, sizeof(int) * letterCount);//��ʼ�������������ǽ�ĳһ���ڴ��е�����ȫ������Ϊָ����ֵ
    memset(visBullet, 0, sizeof(int) * letterCount);
    for (int i = 0; i < letterCount; i++)
    {
        bulletflag[i] = i;
    }
}

//��ʼ���ַ�������sdf
void initLetters()
{        
    int len;//�ַ�������
    letters = (struct tag_letter*)malloc(sizeof(struct tag_letter) * letterCount);
    //srand((unsigned)time(NULL));//ʱ������
    //Sleep(200);
    for (int i = 0; i < letterCount; i++)
    {
        len = rand() % LENMAX;//����Ϊlen����\0 ������ַ���
        if (0 == len || 1 == len) len = 2;//���ٰ���һ����Ч�ַ�

        letters[i].len = len;//��¼�ַ�������
        RandomString(letters[i].str, len);
        //letters[i].ch = rand() % 26 + 'A';
        letters[i].x = rand() % 80;
        letters[i].y = -i * 2;
        letters[i].life = 1;
    }
}
/*//��ʼ����ĸ����
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
//�����ַ���sdf
void drawString(char str[], int x, int y)
{
    EnterCriticalSection(&csCursor);//������ʱ���
    gotoxy(x, y);
    printf("%s", str);
    LeaveCriticalSection(&csCursor);
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
DWORD WINAPI runBullet(void* p)//�ӵ�������� p����&bulletflag[i] ����ʱ�Ѿ�˵��������ȫ��ȷ
{
    int letterId = *(int*)p;//letterId ���� i
    if (!visBullet[letterId] == 1)//ÿ���ӵ���ʼΪ1��δ���У�
    {
        visBullet[letterId] = 1;
    }
    int y = 25;
    int x = letters[letterId].x;
    while (y > letters[letterId].y)//ֱ���ӵ�����ĸy�������
    {
        drawLetter(' ', x, y);
        drawLetter(12, x, y - 1);

        Sleep(bulletSpeed);
        y--;
    }
    drawLetter(' ', x, y);//һ�����
    letters[letterId].life = 0;//��ĸ����ȡ0
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
            for (int j = 0; j < letters[i].len; j++)
            {
                drawLetter(' ', letters[i].x + j, letters[i].y);//��Ҫ֪���ַ�������  4���ո�
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
                drawLetter(' ', letters[i].x + j, letters[i].y);//��Ҫ֪���ַ�������  4���ո�
            }
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
void gameOver(users* curr)
{
    curr->highMark = dropLetter;
    //curr->highLevel = level;
    gotoxy(30, 12);
    printf("����ĸ:%d�� ������:%d�� ���󰴼�:%d��", letterCount, dropLetter, errorCount);

}


int gaming(users* curr)
{
    int i = 0;
    int j = 0;
    system("title ������Ϸ");
    gameBegin();
    hideCursor();
    InitializeCriticalSection(&csCursor);//�ٽ�����
    init();
    initLetters();
    CreateThread(NULL, 0, runLetter, NULL, 0, NULL);
    _beginthread(setTitle, 0, NULL);
    while (beginFlag)
    {
        if (_kbhit())//��ȡ���̰���
        {
            char ch = _getch();//���� enter �û��Ӽ�������

            //ch = toupper(ch);//toupper�������ڰ�Сд��ĸת��Ϊ��д��ĸ��
            if (ch == 27)//esc ascii
            {
                break;
            }
            int flag = 1;
            int nextCharFlag = 0;
            for (;i < letterCount && nextCharFlag == 0; i++)//ѭ���ж���ĸ�Ƿ񱻴�� û����errorCount++
            {
                int strFlag = 0;//�ַ���������ȷ�жϱ�־
                //int j = 1;//��ֵ������0==strCnt

                //�ַ�����ʾ�����ڣ��������ڣ��ӵ�����
                if (letters[i].y < 25 && letters[i].y >= 0 && visBullet[i] == 0)//��·ԭ��
                {
                    nextCharFlag = 1;//��Ҫ��һ������
                    while (1)//'0'? //�ַ�������ȷ�ж�
                    {
                        if (letters[i].str[j] == ch) 
                        { 
                            flag = 0;
                            dropLetter++;
                            j++;
                            break;
                        }//����ԣ��ж���һ��
                        else break;//flag = 1

                        if (j == letters[i].len)//ȫ��
                        {
                            i = 0;
                            j = 0;
                            CreateThread(NULL, 0, runBullet, &bulletflag[i], 0, NULL);// &bulletflag[i]Ϊ����  runBulletΪ�̵߳�ַ
                            break;
                        }
                        //ch = _getch();//�����ȷ�����ٴ����� ��ʱ�޷�esc
                    }
                }
            }
            if (flag) errorCount++;//û�ҵ����������
        }
    }
    gameOver(curr);
    InitializeCriticalSection(&csCursor);
    system("\n\n\npause");
    return 0;
}


//��������Ϊlength������ַ���  
char* RandomString(char*string,int len)
{
    int flag, i;

    //srand((unsigned)time(NULL));
    for (i = 0; i < len - 1; i++)
    {
        flag = rand() % 1;//���Ѷ� %3
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