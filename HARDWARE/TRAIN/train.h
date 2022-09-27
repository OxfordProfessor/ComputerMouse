#ifndef _TRAIN_H
#define _TRAIN_H
#include "sys.h"

//������¼���Թ�˳��Ľṹ��
typedef struct {
	//int cx,int cy��ʾx��y���������
	//cy+1:��ʾ������  cy-1:������
	//cx+1:��ʾ������  cx-1:������
	int cx, cy;
}Direction;
//�����з���洢˳����������

//������¼��ǰλ�ã�Ȼ��ѹ��ջ��
typedef struct {
	int x, y;//��¼��ǰλ�õ�����
	int di;//��¼��ǰλ����һ���ƶ��ķ���
}Box;
typedef struct Node								//����һ���ṹ��
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
