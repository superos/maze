/*
 *  version 2012.08.23
 *
 *  void init_M()	初始化迷宫(随机障碍、随机起点终点)
 *  void show_M()	显示迷宫
 *  void walker()	寻找路径
 *  void walking()	显示当前走到的位置
 *
 *	初步优化:
 *		模拟人眼 对比起点和终点的相对位置决定优先走向(下/右)
 *
 *	未完成
 *		最优路径
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//正方形迷宫尺寸: LEN * LEN
#define LEN 10

//估算最大步骤数: LEN*LEN
#define MAX_STEP LEN*LEN

//迷宫数组
int M[LEN][LEN];

//起点、终点、步骤
struct POINT {
	int x,y;
} S = { 0, 0 }, E = {LEN-1,LEN-1}, G[MAX_STEP];

//步骤游标
int G_index = 0;

//迷宫初始化
void init_M()
{
	int x,y;
	time_t now = time(NULL);
	srand(now);
	for(x=0;x<LEN;x++)
	{
		for(y=0;y<LEN;y++)
		{
			//随机分配通 或不通
			if( (rand()%10) % 2 )
			{
				M[x][y]=0;
			}
			else
			{
				M[x][y]=1;
			}

			//保证路至少有一路是通的
			if(x>0 && y>0 && M[x][y]==0)
			{
				//保证上下是通的
				if( (rand()%100) >50 )
				{
					M[x][y-1]=0;
					M[x][y+1]=0;
				}
				//保证左右是通的
				else
				{
					M[x-1][y]=0;
					M[x+1][y]=0;
				}

            	//如果上下左右都通则关闭一路
				if( M[x][y]==0 && M[x][y-1]==0 && M[x][y+1]==0 && M[x-1][y]==0 && M[x+1][y] ==0)
				{
					//关闭上
					if( (rand()%100) >=75 )
					{
						M[x][y-1]=1;
					}
						//关闭下
					else if ( (rand()%100) >=50 )
					{
						M[x][y+1]=1;
					}
					//关闭右
					else if ( (rand()%100) >=25 )
					{
						M[x+1][y]=1;
					}
					//关闭左
					else
					{
						M[x-1][y]=1;
					}
				}
			}
		}
	}

	//起点终点随机
	S.x = rand() % (LEN/2);
	S.y = rand() % (LEN/2);
	E.x = rand() % (LEN/2)+LEN/2;
	E.y = rand() % (LEN/2)+LEN/2;

	//起点、终点必须通
	M[S.x][S.y]=0;
	M[E.x][E.y]=0;

	//防死路补丁
	M[S.x][S.y+1]=0;
	M[S.x][S.y-1]=0;
	M[S.x+1][S.y]=0;
	M[S.x-1][S.y]=0;

	M[E.x][E.y+1]=0;
	M[E.x][E.y-1]=0;
	M[E.x+1][E.y]=0;
	M[E.x-1][E.y]=0;
}

//显示原始迷宫
void show_M()
{
	int x,y;
	for(x=0; x<LEN; x++)
	{
		printf("---");
	}
	printf("\n");
	for(x=0;x<LEN;x++)
	{
		for(y=0;y<LEN;y++)
		{
			if( x==S.x && y==S.y)
			{
				printf(" S ");
			}
			else if( x==E.x && y==E.y)
			{
				printf(" E ");
			}
			else if(M[x][y] == 0 )
			{
				printf(" . ");
			}
			else
			{
				printf(" # ");
			}
		}
		printf("\n");
	}
}

//逐步绘图
void walking()
{
	int x,y;
	for(x=0; x<LEN; x++)
	{
		printf("---");
	}
	printf("\n");

	//绘制简化图
	for(x=0; x<LEN; x++)
	{
		for(y=0;y<LEN;y++)
		{
			//显示清理之后的路径
			if ( x==S.x && y==S.y )
			{
				printf(" o ");
			}
			else if ( x==E.x && y==E.y )
			{
				printf(" x ");
			}
			else if ( M[x][y] == 0 )
			{
				printf(" - ");
			}
			else if ( M[x][y] == 1 )
			{
				printf("   ");
			}
			else
			{
				printf(" > ");
			}
		}
		printf("\n");
	}
}

//寻找路径: 核心算法
void walker()
{
	struct POINT ThisStep;
    int x;
	for(x=0; x<LEN; x++)
	{
		printf("---");
	}
	printf("\n");
	printf(" Walk Direction: Right / Down / Left / Up\n");
	for(x=0; x<LEN; x++)
	{
		printf("---");
	}
	printf("\n");
	//初始化第一步
	ThisStep = S;		//第一步为起点
	G[G_index] = ThisStep;  		//当前步压栈
	G_index++;              		//指针上移
	while(G_index>=0)
	{
		//显示逐步绘图
		//walking();
		G_index--;
		ThisStep = G[G_index];
		//寻线规则：右、下、左、上
		//如果已经是终点
		if( ThisStep.x == E.x && ThisStep.y == E.y)
		{
			break;
		}

		//模拟人眼初步判断
		if( (E.x-S.x)>(LEN/2) && (E.y-S.y)<(LEN/2) )
		{
			//下边可走
			if ( (ThisStep.x+1)<LEN && M[ThisStep.x+1][ThisStep.y]==0)
			{
				printf("%d: \t(%d, %d)\t\\/\t", G_index+1, ThisStep.x, ThisStep.y);
				//下走
				ThisStep.x = ThisStep.x +1;

				//设置已走
				M[ThisStep.x][ThisStep.y]--;

				//步骤入库
				G_index++;
				G[G_index] = ThisStep;
				printf("\t(%d, %d)\n",ThisStep.x,ThisStep.y);

				//下一步骤准备
				G_index++;

				continue;
			}
			//右边可走
			if ( (ThisStep.y+1)<LEN && M[ThisStep.x][ThisStep.y+1]==0)
			{
				printf("%d: \t(%d, %d)\t->\t", G_index+1, ThisStep.x, ThisStep.y);
				//右走
				ThisStep.y = ThisStep.y+1;

				//设置已走
				M[ThisStep.x][ThisStep.y]--;

				//步骤入库
				G_index++;
				G[G_index] = ThisStep;
				printf("\t(%d, %d)\n",ThisStep.x,ThisStep.y);

				//下一步骤准备
				G_index++;

				continue;
			}
		}
		else
		{
			//右边可走
			if ( (ThisStep.y+1)<LEN && M[ThisStep.x][ThisStep.y+1]==0)
			{
				printf("%d: \t(%d, %d)\t->\t", G_index+1, ThisStep.x, ThisStep.y);
				//右走
				ThisStep.y = ThisStep.y+1;

				//设置已走
				M[ThisStep.x][ThisStep.y]--;

				//步骤入库
				G_index++;
				G[G_index] = ThisStep;
				printf("\t(%d, %d)\n",ThisStep.x,ThisStep.y);

				//下一步骤准备
				G_index++;

				continue;
			}
			//下边可走
			if ( (ThisStep.x+1)<LEN && M[ThisStep.x+1][ThisStep.y]==0)
			{
				printf("%d: \t(%d, %d)\t\\/\t", G_index+1, ThisStep.x, ThisStep.y);
				//下走
				ThisStep.x = ThisStep.x +1;

				//设置已走
				M[ThisStep.x][ThisStep.y]--;

				//步骤入库
				G_index++;
				G[G_index] = ThisStep;
				printf("\t(%d, %d)\n",ThisStep.x,ThisStep.y);

				//下一步骤准备
				G_index++;

				continue;
			}
		}
		//左边可走
		if ( (ThisStep.x-1)>=0 && M[ThisStep.x-1][ThisStep.y]==0)
		{
			printf("%d: \t(%d, %d)\t<-\t", G_index+1, ThisStep.x, ThisStep.y);
			//左走
			ThisStep.x = ThisStep.x-1;

			//设置已走
			M[ThisStep.x][ThisStep.y]--;

			//步骤入库
			G_index++;
			G[G_index] = ThisStep;
			printf("\t(%d, %d)\n",ThisStep.x,ThisStep.y);

			//下一步骤准备
			G_index++;

			continue;
		}
		//上边可走
		if( (ThisStep.y-1)>=0 && M[ThisStep.x][ThisStep.y-1]==0)
		{
			printf("%d: \t(%d, %d)\t/\\\t", G_index+1, ThisStep.x, ThisStep.y);
			//上走
			ThisStep.y = ThisStep.y-1;

			//设置已走
			M[ThisStep.x][ThisStep.y]--;

			//步骤入库
			G_index++;
			G[G_index] = ThisStep;
			printf("\t(%d, %d)\n",ThisStep.x,ThisStep.y);

			//下一步骤准备
			G_index++;
			continue;
		}
	}
	if(G_index==-1)
	{
		for(x=0; x<LEN; x++)
		{
			printf("---");
		}
		printf("\n");

		printf("\tNO WAY!!!\n");

	}
	walking();
}

//主函数入口
int main()
{
	init_M();
	show_M();
	walker();
	return 0;
}
