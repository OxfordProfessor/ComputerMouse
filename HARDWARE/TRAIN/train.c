#include "train.h"
#include "malloc.h"
/*
  训练模式：迷宫探测
*/

_Bool terminal_flag;   //到达终点标志
_Bool block_flag;      //道路阻塞标志
//用来记录走迷宫顺序的结构体
typedef struct {
	//int cx,int cy表示x和y方向的增量
	//cy+1:表示向右走  cy-1:向左走
	//cx+1:表示向下走  cx-1:向上走
	int cx, cy;
}Direction;
//数组中方向存储顺序：右下左上
Direction direct[4] = { {0,1},{1,0},{0,-1},{-1,0} };

//用来记录当前位置，然后压入栈中
typedef struct {
	int x, y;//记录当前位置的坐标
	int di;//记录当前位置下一次移动的方向
}Box;

typedef struct Node								//定义一个结构体
{
  Box data;
	struct Node *next;
}Node;
typedef struct Node *LinkList;


/*
  函数名称：链表初始化函数
  函数作用：初始化一个空链表
  输入参数：链表的结构体
  输出参数：无
*/

int InitList(LinkList *L)							//带有头节点的单链表的初始化
{
	do
  {
		(*L) = (LinkList)malloc(sizeof(Node));				//开辟空间
	}while(!L);             //直到申请成功
	(*L)->next = NULL;									//使头节点next为NULL，即表示空链表
	return 0;
}
/*
  函数名称：链表元素插入
  函数作用：在上一个结点后插入一个新的结点
  输入参数：@para1：链表，@para2：插入的数据
  输出参数：无
*/
void PUSHList(LinkList *L,Box *D)			//头插法创建一个单链表
{
	int i, n;
	LinkList p;										//声明一个指针p
	(*L) = (LinkList)malloc(sizeof(Node));			
	(*L)->next = NULL;								//先建立一个带头节点的空链表
	p = (LinkList)malloc(sizeof(Node));					//生成新节点
	p->data = (*D);		//将新的结构体参数（x，y，di）放入链表中
	p->next = (*L)->next;							
	(*L)->next = p;										//插入到表头
}
/*
  函数名称：弹出最后一个链表元素
  函数作用：弹出链表的最后一个元素
  输入参数：@para1：链表
  输出参数：无
*/

Box POPList(LinkList *L)				//删除L中第i个元素，并用e返回其值
{
	LinkList p, q;
	Box *e;
	int j = 1;
	p = *L;
	while(p->next != NULL) 		//遍历寻找尾部
	{
		p = p->next;
	}
	q = p->next;
	*e= q->data;									//将q节点中的数据弹出给e
	p->next = q->next;								//将q的后继赋值给p的后继
	free(q);										//释放q节点
	return *e;
}


/*
  函数名称：DFS算法训练函数
  函数作用：训练寻找最短迷宫路径
  输入参数：道路阻塞信息
  输出参数：最短路径的链表信息
*/
_Bool findpath(void)
{
	Box temp;
  int maze[100][100]; //记录走过路线的数组
	LinkList L;    //用于存放路径的链表
	int x = 0,y = 0,di = 0;    //记录当前位置和方向
	int line = 0,col = 0;      //记录下次一点的位置坐标
	temp.x = 1,temp.y = 1,temp.di = -1;   //记录当前位置，将di设置为1
	maze[1][1] = -1;    //一开始点位于maze[1][1]的位置，所以将该点变为-1，说明该点已走，不可再次通行
	InitList(&L);  //创建一个链表
	PUSHList(&L,&temp);   //将当前下一步方向数据写入链表
	while(1)
	{
		temp = POPList(&L);   //弹出链表中的最后一个元素
		x = temp.x, y = temp.y, di=temp.di+1;
		while(di<4)
		{
			line = x + direct[di].cx;
			col = y + direct[di].cy;
			if(maze[line][col] == 0 && block_flag == 0) //前方格子没有走过，且前方没有阻塞
			{
				temp.x = x, temp.y = y, temp.di = di;
				PUSHList(&L,&temp);
				x = line;
				y = col;
				maze[line][col] = -1;
				if(terminal_flag == 1)       //找到终点
				{
					temp.x = x, temp.y = y, temp.di = di;
					PUSHList(&L,&temp);
					return 1;
				}
				else
				{
					di = 0;
				}
			}
		}
	}
	return 0;
}