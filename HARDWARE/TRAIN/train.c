#include "train.h"
#include "malloc.h"
/*
  训练模式：迷宫探测
*/


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
void CreateListHead(LinkList *L,Box *D)			//头插法创建一个单链表
{
	int i, n;
	LinkList p;										//声明一个指针p
	(*L) = (LinkList)malloc(sizeof(Node));			
	(*L)->next = NULL;								//先建立一个带头节点的空链表
	p = (LinkList)malloc(sizeof(Node));					//生成新节点
	p->data.x = (*D).x;		
	p->data.y = (*D).y;		
	p->data.di = (*D).di;
	p->next = (*L)->next;							
	(*L)->next = p;										//插入到表头
}
/*
  函数名称：DFS算法函数
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
	InitList(&L);  //创建一个链表
	CreateListHead(&L,&temp);   //将当前下一步方向数据写入链表
}