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

/*c)	玩家注册个人信息（保存在文件中），包括账号密码，个人历史最高分、最高关数。*/
users* LoginInterface();
/*d)	每消除一个字母或单词，游戏者加分，最后game over时给出最终分数和过关数，根据分数和过关数，显示是否打破个人记录，是否打破世界纪录，或者在所有玩家中的排名。*/
int Score_Rank(users* curr);