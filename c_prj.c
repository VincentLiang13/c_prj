
#include "include.h"
#include "interface.h"
#include "game.h"


int main()
{
    users* curr;//当前结构体指针
    int rank;
    srand((unsigned)time(NULL));//时间种子

    curr = LoginInterface();
    gaming();
    rank = Score_Rank(curr);

}
