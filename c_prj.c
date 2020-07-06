
#include "include.h"
#include "interface.h"
#include "game.h"

    users* curr;
int main()
{   

    //当前用户结构体指针
    //int rank;
    srand((unsigned)time(NULL));//时间种子

    curr = LoginInterface();//记录当前用户结构体指针
    gaming(&curr);//修改Highlevel Highscore
    //printf("总分数：%d  最高关卡：%d\n", curr->highMark, curr->highLevel);
    //rank = Score_Rank(curr);
    //printf("rank:%d", rank);
}
