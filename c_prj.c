
#include "include.h"
#include "interface.h"
#include "game.h"

    users* curr;
int main()
{   

    //��ǰ�û��ṹ��ָ��
    //int rank;
    srand((unsigned)time(NULL));//ʱ������

    curr = LoginInterface();//��¼��ǰ�û��ṹ��ָ��
    gaming(&curr);//�޸�Highlevel Highscore
    //printf("�ܷ�����%d  ��߹ؿ���%d\n", curr->highMark, curr->highLevel);
    //rank = Score_Rank(curr);
    //printf("rank:%d", rank);
}
