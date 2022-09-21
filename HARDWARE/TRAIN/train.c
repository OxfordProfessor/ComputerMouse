#include "train.h"
#include "malloc.h"
/*
  ѵ��ģʽ���Թ�̽��
*/

_Bool terminal_flag;   //�����յ��־
_Bool block_flag;      //��·������־
//������¼���Թ�˳��Ľṹ��
typedef struct {
	//int cx,int cy��ʾx��y���������
	//cy+1:��ʾ������  cy-1:������
	//cx+1:��ʾ������  cx-1:������
	int cx, cy;
}Direction;
//�����з���洢˳����������
Direction direct[4] = { {0,1},{1,0},{0,-1},{-1,0} };

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


/*
  �������ƣ������ʼ������
  �������ã���ʼ��һ��������
  �������������Ľṹ��
  �����������
*/

int InitList(LinkList *L)							//����ͷ�ڵ�ĵ�����ĳ�ʼ��
{
	do
  {
		(*L) = (LinkList)malloc(sizeof(Node));				//���ٿռ�
	}while(!L);             //ֱ������ɹ�
	(*L)->next = NULL;									//ʹͷ�ڵ�nextΪNULL������ʾ������
	return 0;
}
/*
  �������ƣ�����Ԫ�ز���
  �������ã�����һ���������һ���µĽ��
  ���������@para1������@para2�����������
  �����������
*/
void PUSHList(LinkList *L,Box *D)			//ͷ�巨����һ��������
{
	int i, n;
	LinkList p;										//����һ��ָ��p
	(*L) = (LinkList)malloc(sizeof(Node));			
	(*L)->next = NULL;								//�Ƚ���һ����ͷ�ڵ�Ŀ�����
	p = (LinkList)malloc(sizeof(Node));					//�����½ڵ�
	p->data = (*D);		//���µĽṹ�������x��y��di������������
	p->next = (*L)->next;							
	(*L)->next = p;										//���뵽��ͷ
}
/*
  �������ƣ��������һ������Ԫ��
  �������ã�������������һ��Ԫ��
  ���������@para1������
  �����������
*/

Box POPList(LinkList *L)				//ɾ��L�е�i��Ԫ�أ�����e������ֵ
{
	LinkList p, q;
	Box *e;
	int j = 1;
	p = *L;
	while(p->next != NULL) 		//����Ѱ��β��
	{
		p = p->next;
	}
	q = p->next;
	*e= q->data;									//��q�ڵ��е����ݵ�����e
	p->next = q->next;								//��q�ĺ�̸�ֵ��p�ĺ��
	free(q);										//�ͷ�q�ڵ�
	return *e;
}


/*
  �������ƣ�DFS�㷨ѵ������
  �������ã�ѵ��Ѱ������Թ�·��
  �����������·������Ϣ
  ������������·����������Ϣ
*/
_Bool findpath(void)
{
	Box temp;
  int maze[100][100]; //��¼�߹�·�ߵ�����
	LinkList L;    //���ڴ��·��������
	int x = 0,y = 0,di = 0;    //��¼��ǰλ�úͷ���
	int line = 0,col = 0;      //��¼�´�һ���λ������
	temp.x = 1,temp.y = 1,temp.di = -1;   //��¼��ǰλ�ã���di����Ϊ1
	maze[1][1] = -1;    //һ��ʼ��λ��maze[1][1]��λ�ã����Խ��õ��Ϊ-1��˵���õ����ߣ������ٴ�ͨ��
	InitList(&L);  //����һ������
	PUSHList(&L,&temp);   //����ǰ��һ����������д������
	while(1)
	{
		temp = POPList(&L);   //���������е����һ��Ԫ��
		x = temp.x, y = temp.y, di=temp.di+1;
		while(di<4)
		{
			line = x + direct[di].cx;
			col = y + direct[di].cy;
			if(maze[line][col] == 0 && block_flag == 0) //ǰ������û���߹�����ǰ��û������
			{
				temp.x = x, temp.y = y, temp.di = di;
				PUSHList(&L,&temp);
				x = line;
				y = col;
				maze[line][col] = -1;
				if(terminal_flag == 1)       //�ҵ��յ�
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