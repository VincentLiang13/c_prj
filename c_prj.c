
#include "include.h"
#include "interface.h"
#include "game.h"


int main()
{
    users* curr;//��ǰ�ṹ��ָ��
    int rank;
    srand((unsigned)time(NULL));//ʱ������

    curr = LoginInterface();
    gaming();
    rank = Score_Rank(curr);

}
