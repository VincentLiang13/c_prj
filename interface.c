
#include "include.h"
#include "interface.h"

const short TRYMAX = 3;//最大尝试次数

//创建储存用户账号密码的文件
void Create_File()
{
    FILE* fp;
    if ((fp = fopen("users.txt", "r")) == NULL)                 //如果此文件不存在
    {
        if ((fp = fopen("users.txt", "w+")) == NULL)
        {
            printf("无法建立文件！\n");
            exit(0);
        }
    }
}
//注册账号
void Register()  //按结构体的大小，一次写入或者读入1个结构体大小
{
    users a;//结构体 The_users 重命名定义
    FILE* fp;
    char temp[20];
    int cnt = 0;//尝试次数
    users b = { {0},{0},0,0 };//初始化

    printf("欢迎来到注册界面！\n");
    Sleep(1000);
    fp = fopen("users.txt", "r");

    fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
    //fgets(&b.id, sizeof(b.id)/sizeof(char), fp);//selfdef
    //fread(&b, sizeof(struct The_users), 1, fp); //读入一个结构体字符块 到b
    printf("请创建账号\n");
    scanf("%s", &a.id);

    while (1)
    {
        if (strcmp(a.id, b.id)) //如果两串不相等
        {
            if (!feof(fp))    //如果未到文件尾
            {
                fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
                //fread(&b, sizeof(struct The_users), 1, fp);//继续读取下一个字符块
            }
            else//到达文件尾
                break;
        }
        else//如果两串相等
        {
            printf("此用户名已存在！请重新注册！\n");
            Sleep(500);
            cnt++;
            printf("剩余次数:%d\n", TRYMAX - cnt);
            if (TRYMAX - cnt == 0)//如果超过最大次数
            {
                printf("超过最大次数，即将退出\n");
                return;
            }
            printf("请创建账号\n");
            scanf("%s", &a.id);

        }
    }
    cnt = 0;//重新计数
    printf("请输入密码\n");
    scanf(" %s", &a.pwd);
    printf("请确认密码\n");
    scanf(" %s", &temp);
    do {
        if (!strcmp(a.pwd, temp)) //两次输入相等
        {
            fp = fopen("users.txt", "a");//追加方式写入
            fprintf(fp, "%s\t%s\t%d\t%d\n", a.id, a.pwd, a.highLevel, a.highMark);//selfef
            //fwrite(&a, sizeof(struct The_users), 1, fp);
            printf("账号注册成功，请登录！\n");
            Sleep(500);
            fclose(fp);
            return;
        }
        else {
            printf("两次密码不匹配！\n");
            cnt++;
            printf("剩余次数:%d\n", TRYMAX - cnt);
            Sleep(500);
            if (TRYMAX - cnt == 0)//如果超过最大次数
            {
                printf("超过最大次数，即将退出\n");
                return;
            }
            printf("请重新输入！\n");
            scanf("%s", &a.pwd);
            printf("请确认密码\n");
            scanf("%s", &temp);
        }
    } while (1);
}

//登录系统
users*  Login()
{
    users a, b;//定义结构体The_users别名
    int cnt = 0;//尝试次数计数
    FILE* fp;

    printf("欢迎来到登录界面！\n");
    Sleep(1000);
    fp = fopen("users.txt", "r");

    fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
    //fread(&b, sizeof(struct The_users), 1, fp); //读入一个结构体字符块 写入b
    printf("请输入账号\n");
    scanf("%s", &a.id);

    while (1)
    {
        if (strcmp(a.id, b.id) == 0)  //如果有此用户名
        {
            printf("读取用户名成功！\n");
            break;
        }
        else
        {
            if (!feof(fp))  //如果文件没有读完
            {   
                fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
                //fread(&b, sizeof(struct The_users), 1, fp);
            }
            else
            {
                printf("此用户名不存在，请重新输入！\n");
                cnt++;
                printf("剩余次数:%d\n", TRYMAX - cnt);
                Sleep(500);
                if (TRYMAX - cnt == 0)//如果超过最大次数
                {
                    printf("超过最大次数，即将退出\n");
                    exit(0);//退出程序
                }
                printf("请输入账号\n");
                scanf("%s", &a.id);
            }
        }
    }
    cnt = 0;//重新计数
    printf("请输入密码\n");
    scanf("%s", &a.pwd);
    while (1)
    {
        if (strcmp(a.pwd, b.pwd) == 0)            //如果密码匹配
        {
            fclose(fp);
            printf("登录成功,开始你的表演吧!\n");
            Sleep(500);
            return &a;
        }
        else
        {
            printf("密码不正确!请重新输入密码\n");
            cnt++;
            printf("剩余次数:%d\n", TRYMAX - cnt);
            Sleep(500);
            if (TRYMAX - cnt == 0)//如果超过最大次数
            {
                printf("超过最大次数，即将退出\n");
                exit(0);//退出程序
            }
            printf("请重新输入！\n");
            scanf("%s", &a.pwd);
        }
    }
}

//登陆界面
users* LoginInterface()//改进字符输入输入字母会一直提示重新输入
{   
    users* curr;
    char f;//登陆or注册标志
    printf("登陆请输入1，注册请输入2\n");
    while (1)
    {
        scanf("%c%*c", &f);
        fflush(stdin);//清除数据缓存
        if (('1' == f) || ('2' == f)) break;
        printf("选择错误，请重新输入：\n");
    }
    if ('2' == f)//注册程序
    {
        Register();
    }
    curr = Login();
    return curr;
}

int Score_Rank(users* a)
{
    FILE* fp;
    if ((fp = fopen("users.txt", "r")) == NULL)                 //如果此文件不存在
    {
        if ((fp = fopen("users.txt", "w+")) == NULL)
        {
            printf("无法建立文件！\n");
            exit(0);
        }
    }

    //id score level  bublerank
    int num = 0;//在user里面的人数
    unsigned int current_read_position = ftell(fp);
    int file_size;
    fseek(fp, 0, SEEK_END);
    //获取文件的大小
    file_size = ftell(fp);
    //恢复文件原来读取的位置
    fseek(fp, current_read_position, SEEK_SET);
    num = file_size / sizeof(users);
    fclose(fp);    
    //在game里面已经更新了score和level

    int rank = 1;
    users b = { {0},{0},0,0 };//初始化
//打开rank。txt
    if ((fp = fopen("rank.txt", "r")) == NULL)                 //如果此文件不存在
    {
        if ((fp = fopen("rank.txt", "w+")) == NULL)
        {
            printf("无法建立文件！\n");
            exit(0);
        }
    }

    fscanf(fp, "%s\t%d\t%d\n", b.id, &b.highLevel, &b.highMark);//selfdef
    for (int i = 0;i < num;i++)
    {
        if (a->highMark > b.highMark) //a>b
        {
            fprintf(fp, "%s\t%d\t%d\n", a->id, a->highLevel, a->highMark);//selfdef
            for (int i = 0; i < num; i++)
            {
                fprintf(fp, "%s\t%d\t%d\n", b.id, b.highLevel, b.highMark);//selfdef
            }
            break;
        }
        else
        {
            fprintf(fp, "%s\t%d\t%d\n", b.id, b.highLevel, b.highMark);//selfdef
            rank++;
            fscanf(fp, "%s\t%d\t%d\n", b.id, &b.highLevel, &b.highMark);//selfdef
        }
    }
        return rank;
}


/*
int p, temp;
for (int i = 0; i < num - 1; i++)
{
    p = i;
    for (int j = i; j < num; j++)
    {
        if (a[p] > a[j])  p = j;
    }
    if (p != i)
    {
        temp = a[p];
        a[p] = a[i];
        a[i] = temp;
    }


}*/