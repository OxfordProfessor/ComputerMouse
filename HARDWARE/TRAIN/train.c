#include "train.h"
#include "malloc.h"
#include "pwm.h"
#include "stmflash.h"
#include "string.h"

#define Start_Address  0x0000FF    //路线信息存放的flash初始地址
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
LinkList L;    //用于存放路径的链表

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
  函数名称：链表长度计算函数
  函数作用：计算链表长度
  输入参数：@para1：链表
  输出参数：链表的长度
*/
int LengthList(LinkList *L)					//计算链表长度
{
	int length = 0;							//计算链表长度
	LinkList p;
	p = (*L)->next;						//使p指向链表的第一个节点
	while (p)							
	{
		length++;						
		p = p->next;
	}
	return length;					//返回链表长度
}

/*
  函数名称：动作函数
  函数作用：执行相应的动作
  输入参数：@para1:下一步方位状态，@para2:当前方位状态
  输出参数：电脑鼠相应运行动作
*/

void movement(Box *temp,Box *cur_tem_loc)
{
		if((*temp).x - (*cur_tem_loc).x == 0)            //下一步向y轴移动
		{
			if((*temp).y > (*cur_tem_loc).y)              //下一步坐标在下方
			{
				//本次方向是否对准下方？
				if((*cur_tem_loc).di == 0)   //自身右转90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 2)  //自身左转90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 3)               //自身选择180
				{
					turn_left(180);
				}
				Go_Onestep(); 
				//下一次方向是否正确？
				if((*temp).di == 0)
				{
					turn_left(90);
				}
				else if((*temp).di == 2)
				{
					turn_right(90);
				}
				else if((*temp).di == 3)
				{
					turn_right(180);
				}
			}
			else                                    //下一步坐标在上方
			{
				//本次方向是否对准上方？
				if((*cur_tem_loc).di == 2)   //自身右转90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 0)  //自身左转90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 1)             //自身选择180
				{
					turn_left(180);
				}
				Go_Onestep();
				//下一次方向是否正确？
				if((*temp).di == 0)
				{
					turn_right(90);
				}
				else if((*temp).di == 2)
				{
					turn_left(90);
				}
				else if((*temp).di == 1)
				{
					turn_right(180);
				}
			}
		}
		if((*temp).y - (*cur_tem_loc).y == 0)            //下一步向y轴移动
		{
			if((*temp).x > (*cur_tem_loc).x)              //下一步坐标在左方
			{
				//本次方向是否对准左方？
				if((*cur_tem_loc).di == 1)   //自身右转90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 3)  //自身左转90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 0)              //自旋转180
				{
					turn_left(180);
				}
				Go_Onestep();
				//下一次方向是否正确？
				if((*temp).di == 1)
				{
					turn_left(90);
				}
				else if((*temp).di == 3)
				{
					turn_right(90);
				}
				else if((*temp).di == 0)
				{
					turn_right(180);
				}
			}
			else                                    //下一步坐标在上方
			{
				//本次方向是否对准右方？
				if((*cur_tem_loc).di == 3)   //自身右转90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 1)  //自身左转90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 2)               //自身选择180
				{
					turn_left(180);
				}
			}
			Go_Onestep();
			//下一次方向是否正确？
			if((*temp).di == 1)
			{
				turn_right(90);
			}
			else if((*temp).di == 3)
			{
				turn_left(90);
			}
			else if((*temp).di == 2)
			{
				turn_right(180);
			}
		}
}

/*
  函数名称：DFS算法训练函数
  函数作用：训练寻找最短迷宫路径
  输入参数：道路阻塞信息，路径终点信息
  输出参数：最短路径的链表信息
*/
_Bool findpath(void)
{
	Box temp;            //下一步动作状态
	Box cur_tem_loc;    //暂存当前位置状态，用于前进方式判断
  int maze[100][100]; //记录走过路线的数组
	int x = 0,y = 0,di = 0;    //记录当前位置和方向
	int line = 0,col = 0;      //记录下次一点的位置坐标
	temp.x = 1,temp.y = 1,temp.di = -1;   //记录当前位置，将di设置为1
	maze[1][1] = -1;    //一开始点位于maze[1][1]的位置，所以将该点变为-1，说明该点已走，不可再次通行
	InitList(&L);  //创建一个链表
	PUSHList(&L,&temp);   //将当前下一步方向数据写入链表
	while(1)
	{
		cur_tem_loc = temp;
		temp = POPList(&L);   //弹出链表中的最后一个元素
		//电脑鼠动作（走到相应坐标）
		movement(&temp,&cur_tem_loc);  //动作执行函数
		x = temp.x, y = temp.y, di=temp.di+1;   //更新当前位置信息
		while(di<4)
		{
			line = x + direct[di].cx;
			col = y + direct[di].cy;
			if(maze[line][col] == 0 && block_flag == 0) //前方格子没有走过，且前方没有阻塞
			{
				temp.x = x, temp.y = y, temp.di = di;   //当前位置状态写入链表
				PUSHList(&L,&temp);
				x = line;       //尝试新的坐标点
				y = col;
				maze[line][col] = -1; //记录该坐标点已走过
				cur_tem_loc.x = x, cur_tem_loc.y = y,cur_tem_loc.di = di;   //预存本次位置位置信息
				movement(&cur_tem_loc,&temp);    //动作执行函数,让电老鼠走向这个新的点
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
			else
			{
				di++;
			}
		}
	}
	return 0;
}

/*
  函数名称：路线存储函数
  函数作用：将存储有路线的链表存入flash
  输入参数：@para1：存储有路线的链表
  输出参数：存在flash中的路线,@ret1:总数据数
*/

void tract_storage(LinkList *L)
{
	u32 temp = 0;
	u32 count = 0;
	int i,s;
	LinkList p;
	p = (*L)->next;
	for(i = 0;i < LengthList(L);i++)   //对于链表中的结构体全部存入flash中
	{
		for(s = 0;s < sizeof(p->data) ; s++)   //获取结构存储的地址偏移量
		{
			memcpy(&temp,&(p->data)+s,1);            //从链表中数据结构体所在内存中拷贝一个byte
		}
		STMFLASH_Write(Start_Address+4*i*sizeof(p->data),(u32*)temp,sizeof(p->data)/4+((sizeof(p->data)%4)?1:0));         //数据写入flash
		p = p->next;     //指向下一位元素
	}
	count =  (sizeof(p->data)/4+((sizeof(p->data)%4)?1:0))*LengthList(L);
	STMFLASH_Write(Start_Address,&count,1);         //数据总数数据写入flash
}	

/*
  函数名称：路线遍历函数
  函数作用：读取flash中存储的路线信息到链表，并执行相应动作
  输入参数：无
  输出参数：相应动作函数																																				
*/
void tract_follow()
{
	int s;
  u32 temp = 0;
	u32 count = 0;
	Box data;
	STMFLASH_Read(Start_Address,(u32*)count,1);		 //获取存储的数据总量
	InitList(&L);  //创建一个链表
	for(s = 0; s < count/4 ; s++)
	{
		STMFLASH_Read(Start_Address+16*s,(u32*)temp,4);		 //根据地址依次取出一个链表中的结构体
		memcpy(&data,&temp,16);            //从取出的数据放入data中
		PUSHList(&L,&data);   //将flash数据写入链表     
	}	
}
/*
  函数名称：路线清除函数
  函数作用：删除现有的flash中存储的路线信息
  输入参数：无
  输出参数：无																																			
*/
void tract_delect()
{
	u32 temp= 0;
	STMFLASH_Write(Start_Address,&temp,1000);         //数据0写入flash,清0
}

void fskl()
{
	while(!findpath());   //找到路径后跳出
}