#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

//user regist�����˺����룬������ʷ��߷֡���߹�����
typedef struct The_users   //typedef ���Զ���ṹ�����
{
    char id[11]; //�˺� 
    char pwd[20]; //���� 
    int highMark;//��߷�
    int highLevel;//��߹���
}users;
