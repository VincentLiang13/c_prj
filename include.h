#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#include <time.h>
#include <ctype.h>
#include <conio.h>

//user regist包括账号密码，个人历史最高分、最高关数。
typedef struct The_users   //typedef 可以定义结构体别名
{
    char id[11]; //账号 
    char pwd[20]; //密码 
    int highMark;//最高分
    int highLevel;//最高关数
}users;
