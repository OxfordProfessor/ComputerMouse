#include "train.h"
#include "malloc.h"
#include "pwm.h"
#include "stmflash.h"
#include "string.h"

#define Start_Address  0x0000FF    //·����Ϣ��ŵ�flash��ʼ��ַ
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
LinkList L;    //���ڴ��·��������

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
  �������ƣ������ȼ��㺯��
  �������ã�����������
  ���������@para1������
  �������������ĳ���
*/
int LengthList(LinkList *L)					//����������
{
	int length = 0;							//����������
	LinkList p;
	p = (*L)->next;						//ʹpָ������ĵ�һ���ڵ�
	while (p)							
	{
		length++;						
		p = p->next;
	}
	return length;					//����������
}

/*
  �������ƣ���������
  �������ã�ִ����Ӧ�Ķ���
  ���������@para1:��һ����λ״̬��@para2:��ǰ��λ״̬
  �����������������Ӧ���ж���
*/

void movement(Box *temp,Box *cur_tem_loc)
{
		if((*temp).x - (*cur_tem_loc).x == 0)            //��һ����y���ƶ�
		{
			if((*temp).y > (*cur_tem_loc).y)              //��һ���������·�
			{
				//���η����Ƿ��׼�·���
				if((*cur_tem_loc).di == 0)   //������ת90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 2)  //������ת90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 3)               //����ѡ��180
				{
					turn_left(180);
				}
				Go_Onestep(); 
				//��һ�η����Ƿ���ȷ��
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
			else                                    //��һ���������Ϸ�
			{
				//���η����Ƿ��׼�Ϸ���
				if((*cur_tem_loc).di == 2)   //������ת90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 0)  //������ת90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 1)             //����ѡ��180
				{
					turn_left(180);
				}
				Go_Onestep();
				//��һ�η����Ƿ���ȷ��
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
		if((*temp).y - (*cur_tem_loc).y == 0)            //��һ����y���ƶ�
		{
			if((*temp).x > (*cur_tem_loc).x)              //��һ����������
			{
				//���η����Ƿ��׼�󷽣�
				if((*cur_tem_loc).di == 1)   //������ת90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 3)  //������ת90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 0)              //����ת180
				{
					turn_left(180);
				}
				Go_Onestep();
				//��һ�η����Ƿ���ȷ��
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
			else                                    //��һ���������Ϸ�
			{
				//���η����Ƿ��׼�ҷ���
				if((*cur_tem_loc).di == 3)   //������ת90
				{
					turn_right(90);
				}
				else if((*cur_tem_loc).di == 1)  //������ת90
				{
					turn_left(90);
				}
				else if((*cur_tem_loc).di == 2)               //����ѡ��180
				{
					turn_left(180);
				}
			}
			Go_Onestep();
			//��һ�η����Ƿ���ȷ��
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
  �������ƣ�DFS�㷨ѵ������
  �������ã�ѵ��Ѱ������Թ�·��
  �����������·������Ϣ��·���յ���Ϣ
  ������������·����������Ϣ
*/
_Bool findpath(void)
{
	Box temp;            //��һ������״̬
	Box cur_tem_loc;    //�ݴ浱ǰλ��״̬������ǰ����ʽ�ж�
  int maze[100][100]; //��¼�߹�·�ߵ�����
	int x = 0,y = 0,di = 0;    //��¼��ǰλ�úͷ���
	int line = 0,col = 0;      //��¼�´�һ���λ������
	temp.x = 1,temp.y = 1,temp.di = -1;   //��¼��ǰλ�ã���di����Ϊ1
	maze[1][1] = -1;    //һ��ʼ��λ��maze[1][1]��λ�ã����Խ��õ��Ϊ-1��˵���õ����ߣ������ٴ�ͨ��
	InitList(&L);  //����һ������
	PUSHList(&L,&temp);   //����ǰ��һ����������д������
	while(1)
	{
		cur_tem_loc = temp;
		temp = POPList(&L);   //���������е����һ��Ԫ��
		//�����������ߵ���Ӧ���꣩
		movement(&temp,&cur_tem_loc);  //����ִ�к���
		x = temp.x, y = temp.y, di=temp.di+1;   //���µ�ǰλ����Ϣ
		while(di<4)
		{
			line = x + direct[di].cx;
			col = y + direct[di].cy;
			if(maze[line][col] == 0 && block_flag == 0) //ǰ������û���߹�����ǰ��û������
			{
				temp.x = x, temp.y = y, temp.di = di;   //��ǰλ��״̬д������
				PUSHList(&L,&temp);
				x = line;       //�����µ������
				y = col;
				maze[line][col] = -1; //��¼����������߹�
				cur_tem_loc.x = x, cur_tem_loc.y = y,cur_tem_loc.di = di;   //Ԥ�汾��λ��λ����Ϣ
				movement(&cur_tem_loc,&temp);    //����ִ�к���,�õ�������������µĵ�
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
			else
			{
				di++;
			}
		}
	}
	return 0;
}

/*
  �������ƣ�·�ߴ洢����
  �������ã����洢��·�ߵ��������flash
  ���������@para1���洢��·�ߵ�����
  �������������flash�е�·��,@ret1:��������
*/

void tract_storage(LinkList *L)
{
	u32 temp = 0;
	u32 count = 0;
	int i,s;
	LinkList p;
	p = (*L)->next;
	for(i = 0;i < LengthList(L);i++)   //���������еĽṹ��ȫ������flash��
	{
		for(s = 0;s < sizeof(p->data) ; s++)   //��ȡ�ṹ�洢�ĵ�ַƫ����
		{
			memcpy(&temp,&(p->data)+s,1);            //�����������ݽṹ�������ڴ��п���һ��byte
		}
		STMFLASH_Write(Start_Address+4*i*sizeof(p->data),(u32*)temp,sizeof(p->data)/4+((sizeof(p->data)%4)?1:0));         //����д��flash
		p = p->next;     //ָ����һλԪ��
	}
	count =  (sizeof(p->data)/4+((sizeof(p->data)%4)?1:0))*LengthList(L);
	STMFLASH_Write(Start_Address,&count,1);         //������������д��flash
}	

/*
  �������ƣ�·�߱�������
  �������ã���ȡflash�д洢��·����Ϣ��������ִ����Ӧ����
  �����������
  �����������Ӧ��������																																				
*/
void tract_follow()
{
	int s;
  u32 temp = 0;
	u32 count = 0;
	Box data;
	STMFLASH_Read(Start_Address,(u32*)count,1);		 //��ȡ�洢����������
	InitList(&L);  //����һ������
	for(s = 0; s < count/4 ; s++)
	{
		STMFLASH_Read(Start_Address+16*s,(u32*)temp,4);		 //���ݵ�ַ����ȡ��һ�������еĽṹ��
		memcpy(&data,&temp,16);            //��ȡ�������ݷ���data��
		PUSHList(&L,&data);   //��flash����д������     
	}	
}
/*
  �������ƣ�·���������
  �������ã�ɾ�����е�flash�д洢��·����Ϣ
  �����������
  �����������																																			
*/
void tract_delect()
{
	u32 temp= 0;
	STMFLASH_Write(Start_Address,&temp,1000);         //����0д��flash,��0
}

void fskl()
{
	while(!findpath());   //�ҵ�·��������
}