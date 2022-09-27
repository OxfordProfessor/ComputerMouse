#ifndef _TRAIN_H
#define _TRAIN_H
#include "sys.h"

//用来记录走迷宫顺序的结构体
typedef struct {
	//int cx,int cy表示x和y方向的增量
	//cy+1:表示向右走  cy-1:向左走
	//cx+1:表示向下走  cx-1:向上走
	int cx, cy;
}Direction;
//数组中方向存储顺序：右下左上

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

int InitList(LinkList *L);
void PUSHList(LinkList *L,Box *D);
Box POPList(LinkList *L);
int LengthList(LinkList *L);
void movement(Box *temp,Box *cur_tem_loc);
_Bool findpath(void);
void tract_storage(LinkList *L);
void Traverse_List(LinkList *L);
void tract_follow(void);
void tract_delect(void);

#endif
