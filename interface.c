
#include "include.h"
#include "interface.h"

const short TRYMAX = 3;//����Դ���

//���������û��˺�������ļ�
void Create_File()
{
    FILE* fp;
    if ((fp = fopen("users.txt", "r")) == NULL)                 //������ļ�������
    {
        if ((fp = fopen("users.txt", "w+")) == NULL)
        {
            printf("�޷������ļ���\n");
            exit(0);
        }
    }
}
//ע���˺�
void Register()  //���ṹ��Ĵ�С��һ��д����߶���1���ṹ���С
{
    users a;//�ṹ�� The_users ����������
    FILE* fp;
    char temp[20];
    int cnt = 0;//���Դ���
    users b = { {0},{0},0,0 };//��ʼ��

    printf("��ӭ����ע����棡\n");
    Sleep(1000);
    fp = fopen("users.txt", "r");

    fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
    //fgets(&b.id, sizeof(b.id)/sizeof(char), fp);//selfdef
    //fread(&b, sizeof(struct The_users), 1, fp); //����һ���ṹ���ַ��� ��b
    printf("�봴���˺�\n");
    scanf("%s", &a.id);

    while (1)
    {
        if (strcmp(a.id, b.id)) //������������
        {
            if (!feof(fp))    //���δ���ļ�β
            {
                fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
                //fread(&b, sizeof(struct The_users), 1, fp);//������ȡ��һ���ַ���
            }
            else//�����ļ�β
                break;
        }
        else//����������
        {
            printf("���û����Ѵ��ڣ�������ע�ᣡ\n");
            Sleep(500);
            cnt++;
            printf("ʣ�����:%d\n", TRYMAX - cnt);
            if (TRYMAX - cnt == 0)//�������������
            {
                printf("�����������������˳�\n");
                return;
            }
            printf("�봴���˺�\n");
            scanf("%s", &a.id);

        }
    }
    cnt = 0;//���¼���
    printf("����������\n");
    scanf(" %s", &a.pwd);
    printf("��ȷ������\n");
    scanf(" %s", &temp);
    do {
        if (!strcmp(a.pwd, temp)) //�����������
        {
            fp = fopen("users.txt", "a");//׷�ӷ�ʽд��
            fprintf(fp, "%s\t%s\t%d\t%d\n", a.id, a.pwd, a.highLevel, a.highMark);//selfef
            //fwrite(&a, sizeof(struct The_users), 1, fp);
            printf("�˺�ע��ɹ������¼��\n");
            Sleep(500);
            fclose(fp);
            return;
        }
        else {
            printf("�������벻ƥ�䣡\n");
            cnt++;
            printf("ʣ�����:%d\n", TRYMAX - cnt);
            Sleep(500);
            if (TRYMAX - cnt == 0)//�������������
            {
                printf("�����������������˳�\n");
                return;
            }
            printf("���������룡\n");
            scanf("%s", &a.pwd);
            printf("��ȷ������\n");
            scanf("%s", &temp);
        }
    } while (1);
}

//��¼ϵͳ
users*  Login()
{
    users a, b;//����ṹ��The_users����
    int cnt = 0;//���Դ�������
    FILE* fp;

    printf("��ӭ������¼���棡\n");
    Sleep(1000);
    fp = fopen("users.txt", "r");

    fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
    //fread(&b, sizeof(struct The_users), 1, fp); //����һ���ṹ���ַ��� д��b
    printf("�������˺�\n");
    scanf("%s", &a.id);

    while (1)
    {
        if (strcmp(a.id, b.id) == 0)  //����д��û���
        {
            printf("��ȡ�û����ɹ���\n");
            break;
        }
        else
        {
            if (!feof(fp))  //����ļ�û�ж���
            {   
                fscanf(fp, "%s\t%s\t%d\t%d\n", b.id, b.pwd, &b.highLevel, &b.highMark);//selfdef
                //fread(&b, sizeof(struct The_users), 1, fp);
            }
            else
            {
                printf("���û��������ڣ����������룡\n");
                cnt++;
                printf("ʣ�����:%d\n", TRYMAX - cnt);
                Sleep(500);
                if (TRYMAX - cnt == 0)//�������������
                {
                    printf("�����������������˳�\n");
                    exit(0);//�˳�����
                }
                printf("�������˺�\n");
                scanf("%s", &a.id);
            }
        }
    }
    cnt = 0;//���¼���
    printf("����������\n");
    scanf("%s", &a.pwd);
    while (1)
    {
        if (strcmp(a.pwd, b.pwd) == 0)            //�������ƥ��
        {
            fclose(fp);
            printf("��¼�ɹ�,��ʼ��ı��ݰ�!\n");
            Sleep(500);
            return &a;
        }
        else
        {
            printf("���벻��ȷ!��������������\n");
            cnt++;
            printf("ʣ�����:%d\n", TRYMAX - cnt);
            Sleep(500);
            if (TRYMAX - cnt == 0)//�������������
            {
                printf("�����������������˳�\n");
                exit(0);//�˳�����
            }
            printf("���������룡\n");
            scanf("%s", &a.pwd);
        }
    }
}

//��½����
users* LoginInterface()//�Ľ��ַ�����������ĸ��һֱ��ʾ��������
{   
    users* curr;
    char f;//��½orע���־
    printf("��½������1��ע��������2\n");
    while (1)
    {
        scanf("%c%*c", &f);
        fflush(stdin);//������ݻ���
        if (('1' == f) || ('2' == f)) break;
        printf("ѡ��������������룺\n");
    }
    if ('2' == f)//ע�����
    {
        Register();
    }
    curr = Login();
    return curr;
}

int Score_Rank(users* a)
{
    FILE* fp;
    if ((fp = fopen("users.txt", "r")) == NULL)                 //������ļ�������
    {
        if ((fp = fopen("users.txt", "w+")) == NULL)
        {
            printf("�޷������ļ���\n");
            exit(0);
        }
    }

    //id score level  bublerank
    int num = 0;//��user���������
    unsigned int current_read_position = ftell(fp);
    int file_size;
    fseek(fp, 0, SEEK_END);
    //��ȡ�ļ��Ĵ�С
    file_size = ftell(fp);
    //�ָ��ļ�ԭ����ȡ��λ��
    fseek(fp, current_read_position, SEEK_SET);
    num = file_size / sizeof(users);
    fclose(fp);    
    //��game�����Ѿ�������score��level

    int rank = 1;
    users b = { {0},{0},0,0 };//��ʼ��
//��rank��txt
    if ((fp = fopen("rank.txt", "r")) == NULL)                 //������ļ�������
    {
        if ((fp = fopen("rank.txt", "w+")) == NULL)
        {
            printf("�޷������ļ���\n");
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