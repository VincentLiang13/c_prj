#pragma once
//�൱��ifndef

//user regist�����˺����룬������ʷ��߷֡���߹�����
typedef struct The_users   //typedef ���Զ���ṹ�����
{
    char id[11]; //�˺� 
    char pwd[20]; //���� 
    int highMark;//��߷�
    int highLevel;//��߹���
}users; 

void Create_File();
void Login();
void LoginInterface();
void Register();