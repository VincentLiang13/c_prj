
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
void  Login()
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
                    return;
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
            return;
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
                return;
            }
            printf("���������룡\n");
            scanf("%s", &a.pwd);
        }
    }
}

//��½����
void LoginInterface()//�Ľ��ַ�����������ĸ��һֱ��ʾ��������
{   
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
    Login();
}

