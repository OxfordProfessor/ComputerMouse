#include "train.h"
#include "malloc.h"
/*
  ѵ��ģʽ���Թ�̽��
*/


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
void CreateListHead(LinkList *L,Box *D)			//ͷ�巨����һ��������
{
	int i, n;
	LinkList p;										//����һ��ָ��p
	(*L) = (LinkList)malloc(sizeof(Node));			
	(*L)->next = NULL;								//�Ƚ���һ����ͷ�ڵ�Ŀ�����
	p = (LinkList)malloc(sizeof(Node));					//�����½ڵ�
	p->data.x = (*D).x;		
	p->data.y = (*D).y;		
	p->data.di = (*D).di;
	p->next = (*L)->next;							
	(*L)->next = p;										//���뵽��ͷ
}
/*
  �������ƣ�DFS�㷨����
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
	InitList(&L);  //����һ������
	CreateListHead(&L,&temp);   //����ǰ��һ����������д������
}