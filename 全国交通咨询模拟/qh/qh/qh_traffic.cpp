#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define ERROR 0
#define OK 1
#define OVER -1
#define TRUE 1
#define FALSE 0
#define UNISITED 0
#define MAXINT 10000
#define VISTED 1
#define INFI MAXINT
#define UNSELECTED 0
#define SELECTED 1
#define VISITED 1
#define UNVISITED 0
typedef int ElemType;
typedef int Status;
typedef char VexType;
typedef enum { DG, DN, UDG, UDN }GraphKind;
typedef enum { Plane, Train }ToolKind;
int change;
//����Ա��½�˺�������
int accountNum = 3117004993, passWord = 123456;
int Std, Sth, Stm, Etd, Eth, Etm;
typedef struct LNode {
	int data;
	struct LNode *next;
}LNode, *QueuePtr;

typedef struct {
	QueuePtr front;
	QueuePtr rear;
}LQueue;

typedef struct { //����ѭ�����еĽṹ
	ElemType *elem;
	int front;
	int rear;
	int maxsize;
}SqQueue;


/*
 *�����洢��������г���ʱ�̱�
 *������������ʽ���洢ʱ�̣�����Ѱ��·��ʱ�Ƚ�
 *���磺 20181225��ʾ2018��12��25��
 */
typedef struct Time {
	int StartDate;  //����ʱ��
	int StartHour;
	int StartMin;
	int EndDate;    //����ʱ��
	int EndHour;
	int EndMin;
}*TimeInfoP, TimeInfo;


/*
 * ��ͨ��ʽ�� ��OR�ɻ�������Ʊ�ۡ�ʱ���
 */
typedef struct Node {
	double cost;			//Ʊ��
	//int line;			//�����or�г���
	TimeInfoP time;		    //��Ӧʱ���
}*ToolP, Tool;



typedef struct AdjVexNode {
	int adjvex;			//�ó�����G.vexs�е��±�
	struct AdjVexNode *nextArc;
	Tool* Plane;
	Tool* Train;
}AdjVexNode, *AdjVexNodeP;

typedef struct VexNode {
	VexType data[7];	//���鱣������
	struct AdjVexNode *firstArc;
}VexNode, *VexNodeP;

typedef struct {
	VexType v[7], w[7];		//v--��㣬w--�յ�
	Tool* Plane;
	Tool* Train;
}ArcInfo, ArcInfoP;			//�߼�

typedef struct {
	VexNode *vexs;
	int n, e;
	GraphKind kind;
	int *tags;
}AlGraph;


//·����Ϣ����ר��
typedef struct {
	int prev;
	double lowcost;
}DistInfo, *Dist;

typedef struct {
	int prev;
	int lowcost;	//������
	int h;			//hour
	int min;		//minute
}DisInfo, *Dis;
/*
 *ʱ���ת��
 */
int TrangeY(int Time) {  //�����
	return Time / 10000;
}

int TrangeM(int Time) {  //���·�
	return Time % 10000 / 1000  * 10 + Time % 10000 % 1000 / 100;
}

int TrangeD(int Time) {  //�����
	return Time % 10000 % 1000 % 1000 % 100 / 10 + Time % 10000 % 1000 % 100 % 10;
}

int TrangeHM(int Time) { //��Сʱ����
	return Time / 10 * 10 + Time % 10;
}

Status InitQueue_Sq(SqQueue &Q, int size);
Status DeQueue_Sq(SqQueue &Q, ElemType &e);
Status EnQueue_Sq(SqQueue &Q, ElemType e);
Status EnQueue_LQ(LQueue &Q, int e);
Status DeQueue_LQ(LQueue &Q, int &e);
int LocateVex_AL(AlGraph G, VexType v[]);
Status CreateDG_Al(AlGraph &G, VexType **vexs, int n, ArcInfo *arcs, int e);
void Print(AlGraph G);
Status RemoveArc(AlGraph G, int k, int m);
Status TheLessMoneyWay(AlGraph G, int i, Dist & Dist, ToolKind kind);
Status TheLessChangeWay(AlGraph G, int i, Dist & Dis1, ToolKind kind);
int compare(AlGraph G, int i, int j, ToolKind kind, int d, int h, int m);
void OutputPath2(AlGraph G, DisInfo *Dist, int k, int i, ToolKind kind, int &dd, int &hh, int &mm);
void PrintTheFastMoneyWay(AlGraph G, DisInfo *Dist, int k, int i, ToolKind kind);
void OutputPath(AlGraph G, DistInfo *Dist, int k, int i, ToolKind kind, int &dd, int &hh, int &mm);
void PrintTheLessMoneyWay(AlGraph G, DistInfo *Dist, int k, int i, ToolKind kind);
void PrintTheLessChangeWay(AlGraph G, DistInfo *Dist, int k, int i, ToolKind kind);
Status AddCity(AlGraph &G, VexType c[]);
Status AddLine(AlGraph &G, ArcInfo arc);
Status DeleteLine(AlGraph &G, VexType v[], VexType w[], ToolKind kind);
Status DeleteCity(AlGraph &G, VexType v[]);
Status RenewLine(AlGraph &G, ArcInfo arc, ToolKind kind);
void DFS(AlGraph G, int i, int j, Dis & Dis1, Dis & Dis2, ToolKind kind, int Between1, int Between2, int Between3);
void DFSTraverse(AlGraph G, VexType v[], VexType w[], Dis & Dis1, ToolKind kind);
void MakeNode(ArcInfo *arc, char a[], char b[], int k1, int k2, int k3, int k4, int k5, int k6, double cost1, int t1, int t2, int t3, int t4, int t5, int t6, double cost2);
void Master(AlGraph &G);
void MasterLogin(AlGraph &G);
void User(AlGraph &G);
void window(AlGraph &G);



Status InitQueue_Sq(SqQueue &Q, int size) { //ѭ�����г�ʼ��
	Q.elem = (ElemType *)malloc(size * sizeof(ElemType));
	if (NULL == Q.elem) return ERROR;
	Q.maxsize = size;
	Q.front = Q.rear = 0;
	return OK;
}
Status DeQueue_Sq(SqQueue &Q, ElemType &e) { //ɾ����ǰ���е�ͷԪ��
	if (Q.front == Q.rear) return ERROR;
	e = Q.elem[Q.front];
	Q.front = (Q.front + 1) % Q.maxsize;
	return OK;
}
Status EnQueue_Sq(SqQueue &Q, ElemType e) { //�ڵ�ǰ���е�βԪ�غ����Ԫ��e
	if ((Q.rear + 1) % Q.maxsize == Q.front) return ERROR;
	Q.elem[Q.rear] = e;
	Q.rear = (Q.rear + 1) % Q.maxsize;
	return OK;
}

//���
Status EnQueue_LQ(LQueue &Q, int e) {
	LNode *p;
	p = (LNode*)malloc(sizeof(LNode));
	if (NULL == p)
		return OVERFLOW;
	p->data = e;
	p->next = NULL;
	if (NULL == Q.front)
		Q.front = p;
	else
		Q.rear->next = p;
	Q.rear = p;
}

//����
Status DeQueue_LQ(LQueue &Q, int &e) {
	LNode *p;
	if (NULL == Q.front)
		return ERROR;
	p = Q.front;
	e = p->data;
	Q.front = p->next;
	if (p == Q.rear) Q.rear = NULL;
	free(p);
	return OK;
}

/*
 * ��λ�����±�
 */
int LocateVex_AL(AlGraph G, VexType v[]) {
	int i;
	for (i = 0; i < G.n; i++)
		if (0 == strcmp(v, G.vexs[i].data))
			return i;
	return -1;
}


/*
 * ����һ������ͼ
 */
Status CreateDG_Al(AlGraph &G, VexType **vexs, int n, ArcInfo *arcs, int e) {
	int i, j, k;
	VexType v[7], w[7];
	AdjVexNodeP p;
	G.n = n;
	G.e = e;
	G.vexs = (VexNode *)malloc(n * sizeof(VexNode));
	G.tags = (int *)malloc(n * sizeof(int));
	for (i = 0; i < G.n; i++) {
		G.tags[i] = UNISITED;
		strcpy_s(G.vexs[i].data, vexs[i]);
		G.vexs[i].firstArc = NULL;
	}
	for (k = 0; k < G.e; k++) {
		strcpy_s(v, arcs->v);
		strcpy_s(w, arcs->w);
		i = LocateVex_AL(G, v);
		j = LocateVex_AL(G, w);

		if (i < 0 || j < 0)
			return ERROR;
		p = (AdjVexNode *)malloc(sizeof(AdjVexNode));
		if (NULL == p)
			return OVERFLOW;
		p->adjvex = j;
		p->Plane = arcs->Plane;
		p->Train = arcs->Train;
		p->nextArc = G.vexs[i].firstArc;
		G.vexs[i].firstArc = p;
		arcs++;
	}
	return OK;
}


//��ӡ·��ͼ
void Print(AlGraph G) {
	int i;
	AdjVexNodeP p;
	printf("�ɻ�·��ͼΪ��\n");
	for (i = 0; i<G.n; i++) {
		printf("%s��", G.vexs[i].data);
		p = G.vexs[i].firstArc;
		while (p != NULL) {
			if(p->Plane != NULL)
				printf("%s  ", G.vexs[p->adjvex].data);
			p = p->nextArc;
		}
		printf("\n");
	}
	printf("\n��·��ͼΪ��\n");
	for (i = 0; i<G.n; i++) {
		printf("%s��", G.vexs[i].data);
		p = G.vexs[i].firstArc;
		while (p != NULL) {
			if (p->Train != NULL)
				printf("%s  ", G.vexs[p->adjvex].data);
			p = p->nextArc;
		}
		printf("\n");
	}
}

Status RemoveArc(AlGraph G, int k, int m) {
	//<k, m>
	if (k < 0 || m >= G.n)
		return ERROR;
	AdjVexNodeP pre, p;
	p = G.vexs[k].firstArc;
	if (p == NULL)
		G.vexs[k].firstArc = NULL;
	if (p->adjvex == m) {
		G.vexs[k].firstArc = NULL;
		free(p);
	}
	pre = NULL;
	while (p != NULL) {
		if (p->adjvex == m) {
			pre->nextArc = p->nextArc;
			free(p);
		}
		pre = p;
		p = p->nextArc;
	}
	return ERROR;
}

/*
 * Ѱ����ʡǮ��·��
 * i������±�
 * Dist����·����Ϣ
 */
Status TheLessMoneyWay(AlGraph G, int i, Dist & Dist, ToolKind kind) {
	int j, m, k;
	double min;
	AdjVexNodeP p;
	for (int j = 0; j < G.n; j++) {
		Dist[j].lowcost = INFI;
		G.tags[j] = UNSELECTED;  //���ж�������ټ���U��
	}
	for (p = G.vexs[i].firstArc; p != NULL; p = p->nextArc) {
		Dist[p->adjvex].prev = i;
		Dist[p->adjvex].lowcost = (kind == Plane) ? p->Plane->cost : p->Train->cost;   //���ֻ���ɻ��ķ���
	}
	Dist[i].prev = -1;
	Dist[i].lowcost = 0;   //Դ����Ϣi����Dist
	G.tags[i] = SELECTED;           //��ʼ��
	for (m = 1; m < G.n; m++) {
		double tip;
		min = INFI;
		k = 0;
		for (j = 0; j < G.n; j++)
			if (0 == G.tags[j] && Dist[j].lowcost < min) {
				k = j;
				min = Dist[j].lowcost;
			}
		G.tags[k] = SELECTED;  //��k������뼯��U
		for (p = G.vexs[k].firstArc; p != NULL; p = p->nextArc) {
			j = p->adjvex;
			if (UNSELECTED == G.tags[j] && Dist[k].lowcost + (tip = (kind == Plane) ? p->Plane->cost : p->Train->cost) < Dist[j].lowcost) {
				Dist[j].lowcost = Dist[k].lowcost + tip;
				Dist[j].prev = k;
			}
		}
	}
	return OK;
}

/*
 * ���ٻ���
 * i�����
 */

Status TheLessChangeWay(AlGraph G, int i, Dist & Dis1, ToolKind kind) {
	SqQueue Q;
	InitQueue_Sq(Q, 10);
	AdjVexNodeP p;
	for (int j = 0; j < G.n; j++) {
		Dis1[j].lowcost = INFI;
		G.tags[j] = UNSELECTED;  //���ж�������ټ���U��
	}
	for (p = G.vexs[i].firstArc; p != NULL; p = p->nextArc) {
		Dis1[p->adjvex].prev = i;  
		Dis1[p->adjvex].lowcost = 0;
		G.tags[p->adjvex] = SELECTED;
		EnQueue_Sq(Q, p->adjvex);
	}
	Dis1[i].prev = -1;   //Դ����Ϣi����Dist
	G.tags[i] = SELECTED;           //��ʼ��
	Dis1[i].lowcost = 0;

	int l;
	while (OK == DeQueue_Sq(Q, l)) {  //������ȱ���
		p = G.vexs[l].firstArc;
		while (NULL != p) {
			int k = p->adjvex;
			if (UNSELECTED == G.tags[k]) {
				G.tags[k] = SELECTED;
				Dis1[k].lowcost = Dis1[l].lowcost + 1;
				Dis1[k].prev = l;
				EnQueue_Sq(Q, k);
			}
			p = p->nextArc;
		}
	}
	return OK;
}


/*
 * �ж��Ƿ��������i>>j�����˳�
 * d, h, m�� ��һ�˵���i�ĳ��ĵ���ʱ��
 * ���Է���0�����򷵻�1
 */
int compare(AlGraph G, int i, int j, ToolKind kind, int d, int h, int m) {
	AdjVexNodeP p;
	for (p = G.vexs[i].firstArc; p != NULL; p = p->nextArc)
		if (p->adjvex == j)
			break;
	ToolP w;
	w = (kind == Plane) ? p->Plane : p->Train;
	TimeInfoP t;
	t = w->time;
	if (t->StartDate >= d && t->StartHour >= h && t->StartMin >= m)
		return 0;
	else
		return 1;
}

/*
 * ���·��
 * k ����㣬 iһֱ�����յ��±�
 */
void OutputPath2(AlGraph G, DisInfo *Dist, int k, int i, ToolKind kind, int &dd, int &hh, int &mm) {

	if (i == k)
		return;
	OutputPath2(G, Dist, Dist[k].prev, i, kind, dd, hh, mm);  //�ݹ��ȡ·���ϵĶ���
	int l = (Dist[k].prev == i) ? i : Dist[k].prev;
	AdjVexNodeP p;
	for (p = G.vexs[l].firstArc; p != NULL; p = p->nextArc)
		if (p->adjvex == k)
			break;
	ToolP w;
	w = (kind == Plane) ? p->Plane : p->Train;
	TimeInfoP t;
	t = w->time;
	int y1, y2, m1, m2, d1, d2, h1, h2, min1, min2, flag = 0;
	double cost1;
	y1 = TrangeY(t->StartDate);
	y2 = TrangeY(t->EndDate);
	m1 = TrangeM(t->StartDate);
	m2 = TrangeM(t->EndDate);
	d1 = TrangeD(t->StartDate);
	d2 = TrangeD(t->EndDate);
	h1 = TrangeHM(t->StartHour);
	h2 = TrangeHM(t->EndHour);
	min1 = TrangeHM(t->StartMin);
	min2 = TrangeHM(t->EndMin);
	cost1 = w->cost;
	if (Dist[k].prev == i)
		flag = compare(G, i, k, kind, 0, 0, 0);
	else
		flag = compare(G, Dist[k].prev, k, kind, dd, hh, mm);
	dd = t->EndDate; hh = t->EndHour; mm = t->EndMin;
	if (Dist[k].prev == i) {
		change = 0;
		Std = t->StartDate;
		Sth = t->StartHour;
		Stm = t->StartMin;
		Etd = t->EndDate;
		Eth = t->EndHour;
		Etm = t->EndMin;
	}
	else {
		Etd = t->EndDate;
		Eth = t->EndHour;
		Etm = t->EndMin;
		change++;
	}
	//printf("%s >>>>>> %s    ����ʱ�䣺%d.%d.%d %d:%d  ����ʱ�䣺%d.%d.%d %d:%d  Ʊ�ۣ�%lf\n", G.vexs[Dist[k].prev].data, G.vexs[k].data, y1, m1, d1 + flag, h1, min1, y2, m2, d2 + flag, h2, min2, cost1);		//���������ǰ·���ϵĶ���
	printf("%s >>>>>> %s    ����ʱ�䣺%d %d:%d  ����ʱ�䣺%d %d:%d  Ʊ�ۣ�%lf\n", G.vexs[Dist[k].prev].data, G.vexs[k].data, t->StartDate, t->StartHour, t->StartMin, t->EndDate, t->EndHour, t->EndMin, cost1);		//���������ǰ·���ϵĶ���

}



/*
 * ·�����
 * k������±�
 * i���յ��±�
 */
void OutputPath(AlGraph G, DistInfo *Dist, int k, int i, ToolKind kind, int &dd, int &hh, int &mm) {
		
	if (i == k)
		return;
	OutputPath(G, Dist, Dist[k].prev, i, kind, dd, hh, mm);  //�ݹ��ȡ·���ϵĶ���
	int l = (Dist[k].prev == i) ? i : Dist[k].prev;
	AdjVexNodeP p;
	for (p = G.vexs[l].firstArc; p != NULL; p = p->nextArc) 
		if (p->adjvex == k)
			break;
	ToolP w;
	w = (kind == Plane) ? p->Plane : p->Train;
	TimeInfoP t;
	t = w->time;
	int y1, y2, m1, m2, d1, d2, h1, h2, min1, min2, flag = 0;
	double cost1;
	y1 = TrangeY(t->StartDate);
	y2 = TrangeY(t->EndDate);
	m1 = TrangeM(t->StartDate);
	m2 = TrangeM(t->EndDate);
	d1 = TrangeD(t->StartDate);
	d2 = TrangeD(t->EndDate);
	h1 = TrangeHM(t->StartHour);
	h2 = TrangeHM(t->EndHour);
	min1 = TrangeHM(t->StartMin);
	min2 = TrangeHM(t->EndMin);
	cost1 = w->cost;
	if (Dist[k].prev == i)
		flag = compare(G, i, k, kind, 0, 0, 0);
	else
		flag = compare(G, Dist[k].prev, k, kind, dd, hh, mm);
	dd = t->EndDate; hh = t->EndHour; mm = t->EndMin;
	if (Dist[k].prev == i)
		change = 0;
	else
		change++;
	printf("%s >>>>>> %s    ����ʱ�䣺%d %d:%d  ����ʱ�䣺%d %d:%d  Ʊ�ۣ�%lf\n", G.vexs[Dist[k].prev].data, G.vexs[k].data, t->StartDate, t->StartHour, t->StartMin, t->EndDate, t->EndHour, t->EndMin, cost1);
}

/*
 * ��ӡ��ʡǮ·��
 */
void PrintTheLessMoneyWay(AlGraph G, DistInfo *Dist, int k, int i, ToolKind kind) {
	int dd = 0, hh = 0, mm = 0;
	printf("Ϊ���Ĳ�ѯ���ĵ�����ʡǮ�ĳ��а�������\n");
	OutputPath(G, Dist, k, i, kind, dd, hh, mm);
}

/*
 * ��ӡ���ٻ���·��
 */
void PrintTheLessChangeWay(AlGraph G, DistInfo *Dist, int k, int i, ToolKind kind) {
	int dd = 0, hh = 0, mm = 0;
	printf("Ϊ���Ĳ�ѯ�������ٻ��˵ĳ��а�������\n");
	OutputPath(G, Dist, k, i, kind, dd, hh, mm);
}

/*
* ��ӡ��쵽��·��
*/
void PrintTheFastMoneyWay(AlGraph G, DisInfo *Dist, int k, int i, ToolKind kind) {
	int dd = 0, hh = 0, mm = 0;
	printf("Ϊ���Ĳ�ѯ������쵽��ĳ��а�������\n");
	OutputPath2(G, Dist, k, i, kind, dd, hh, mm);
}

/* 
 *���ӳ���
 */
Status AddCity(AlGraph &G, VexType c[]) {

	//�����Ѵ���
	if (LocateVex_AL(G, c) != -1) {
		printf("The City Existed!\n");
		return OK;
	}
	G.vexs = (VexNode *)realloc(G.vexs, (G.n+1)*sizeof(VexNode));
	strcpy_s(G.vexs[G.n].data, c);
	G.vexs[G.n].firstArc = NULL;
	G.n++;
	printf("Create City Successed!\n");
	return OK;
}

/*
 * ����·��
 */
Status AddLine(AlGraph &G, ArcInfo arc) {
	int k, j, flag = 0;
	//���ж��㲻�����ȴ���
	if (-1 == LocateVex_AL(G, arc.v))
		AddCity(G, arc.v);
	if (-1 == LocateVex_AL(G, arc.w))
		AddCity(G, arc.w);
	k = LocateVex_AL(G, arc.v);
	j = LocateVex_AL(G, arc.w);
	AdjVexNodeP p, q;
	q = G.vexs[k].firstArc;
	for ( ; q != NULL; q = q->nextArc) {
		if (q->adjvex == j) {
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		p = (AdjVexNode *)malloc(sizeof(AdjVexNode));
		p->Plane = (Tool *)malloc(sizeof(Tool));
		p->Plane->time = (TimeInfo *)malloc(sizeof(TimeInfo));
		p->Train = (Tool *)malloc(sizeof(Tool));
		p->Train->time = (TimeInfo *)malloc(sizeof(TimeInfo));
		if (NULL == p)
			return OVERFLOW;
		p->adjvex = j;
		p->Plane = arc.Plane;
		p->Train = arc.Train;
		p->nextArc = G.vexs[k].firstArc;
		G.vexs[k].firstArc = p;
	}
	else {
		q->Plane = arc.Plane;
		q->Train = arc.Train;
	}
	printf("Create Line Successed!\n");
	return OK;
}

/*
 *ɾ��·��
 */
Status DeleteLine(AlGraph &G, VexType v[], VexType w[], ToolKind kind) {
	int i = LocateVex_AL(G, v);
	int j = LocateVex_AL(G, w);
	if (i == -1 || j == -1) {
		printf("Failed To Find The Line��\n");
		return ERROR;
	}
	AdjVexNode *q;
	ToolP p;
	TimeInfoP t;
	for (q = G.vexs[i].firstArc; q != NULL; q = q->nextArc) {
		if (q->adjvex == j) {
			if (kind == Plane) {
				p = q->Plane;
				t = q->Plane->time;
				q->Plane = NULL;
			}
			else {
				p = q->Train;
				t = q->Train->time;
				q->Train = NULL;
			}
			free(p);
			free(t);
			printf("Successed To Delete The Line!\n");
			return OK;
		}
	}
	printf("Failed To Find The Line!\n");
	return ERROR;
}
/*
 * ɾ������
 */
Status DeleteCity(AlGraph &G, VexType v[]) {
	if (-1 == LocateVex_AL(G, v)) {
		printf("The City Not Existed!\n");
		return ERROR;
	}
	int i = LocateVex_AL(G, v);
	int j;
	AdjVexNode *p, *q, *m;

	//�ͷŶ�����ڽӱ�
	/*p = G.vexs[i].firstArc;
	while (NULL != p) {
		q = p;
		p = p->nextArc;
		free(q);
	}*/
	//�ͷŶ���
	for (j = i; j < G.n - 1; j++) {
		strcpy(G.vexs[j].data, G.vexs[j+1].data);
		G.vexs[j].firstArc = G.vexs[j + 1].firstArc;
	}
	G.vexs[j].firstArc = NULL;

	//����ͼ��Ѱ��ͨ������v�ıߣ���ɾ���ýڵ�
	for (int k = 0; k < G.n-1; k++) {
		p = G.vexs[k].firstArc;
		while (NULL != p) {
			if (p->adjvex == i) {
				if (p == G.vexs[k].firstArc) {
					m = p;
					G.vexs[k].firstArc = p->nextArc;
					p = p->nextArc;
					//free(m);
				}
				else {
					q->nextArc = p->nextArc;
					m = p;
					p = p->nextArc;
					//free(q);
				}
			}
			else if (p->adjvex > i) {
				p->adjvex--;
				q = p;
				p = p->nextArc;
			}
			else {
				q = p;
				p = p->nextArc;
			}
		}
	}
	G.n--;
	printf("Successed To Delete The City!\n");
	return OK;
}

/*
 * �޸�·����Ϣ
 */
Status RenewLine(AlGraph &G, ArcInfo arc, ToolKind kind) {
	int k, j;
	k = LocateVex_AL(G, arc.v);
	j = LocateVex_AL(G, arc.w);
	if (-1 == k || -1 == j) {
		printf("Failed To Find The Line!\n");
		return ERROR;
	}
	AdjVexNodeP p;
	Tool *l, *s;
	TimeInfoP m;
	p = G.vexs[k].firstArc;
	for (; p != NULL; p = p->nextArc) {
		if (p->adjvex == j) {
			l = s = (kind == Plane) ? p->Plane : p->Train;
			
			if (NULL == l) {
				l = (Tool *)malloc(sizeof(Tool));
				l->time = (TimeInfo *)malloc(sizeof(TimeInfo));
			}
			else {
				m = s->time;
				free(s);
				free(m);
			}
			l = (kind == Plane) ? arc.Plane : arc.Train;
			G.vexs[k].firstArc = p;
			printf("Create Line Successed!\n");
		}
	}
	return OK;
}



/*
 * ������ȵݹ�����쵽��·��
 * Dis1��������·����Ϣ
 * Dis2���浱ǰ��������·����Ϣ
 * Between1-3 ��������һ�˳��ĵ���ʱ��
 */
void DFS(AlGraph G, int i, int j, Dis & Dis1, Dis & Dis2, ToolKind kind, int Between1, int Between2, int Between3) {
	AdjVexNode *p;
	G.tags[i] = TRUE;
	p = G.vexs[i].firstArc;
	if (!p) {
		return;
	}
	while (NULL != p) {
		if(p->adjvex == j)
		{				//�ҵ��յ�֮�����Dist1
			ToolP t;
			t = (kind == Plane) ? p->Plane : p->Train;
			
			//�ɴ��Dis2��¼����
			if (Between1 < t->time->StartDate) {
				Dis2[p->adjvex].lowcost = t->time->EndDate;
				Dis2[p->adjvex].h = t->time->EndHour;
				Dis2[p->adjvex].min = t->time->EndMin;
				G.tags[j] = TRUE;
			}
			else if (Between1 == t->time->StartDate && Between2 < t->time->StartHour) {
				Dis2[p->adjvex].lowcost = t->time->EndDate;
				Dis2[p->adjvex].h = t->time->EndHour;
				Dis2[p->adjvex].min = t->time->EndMin;
				G.tags[j] = TRUE;
			}
			else if (Between1 == t->time->StartDate && Between2 == t->time->StartHour && Between3 <= t->time->StartMin) {
				Dis2[p->adjvex].lowcost = t->time->EndDate;
				Dis2[p->adjvex].h = t->time->EndHour;
				Dis2[p->adjvex].min = t->time->EndMin;
				G.tags[j] = TRUE;
			}
			Dis2[j].prev = i;
			if (Dis1[j].lowcost > Dis2[j].lowcost)
				//�ɴ��ұ�Dis1�죬�����Dis1
				for (int l = 0; l < G.n; l++) {
					Dis1[l].prev = Dis2[l].prev;
					Dis1[l].lowcost = Dis2[l].lowcost;
					Dis1[l].h = Dis2[l].h;
					Dis1[l].min = Dis2[l].min;
					}
			else if (Dis1[j].lowcost == Dis2[j].lowcost && Dis1[j].h > Dis2[j].h)
				for (int l = 0; l < G.n; l++) {
					Dis1[l].prev = Dis2[l].prev;
					Dis1[l].lowcost = Dis2[l].lowcost;
					Dis1[l].h = Dis2[l].h;
					Dis1[l].min = Dis2[l].min;
				}
			else if (Dis1[j].lowcost == Dis2[j].lowcost && Dis1[j].h == Dis2[j].h && Dis1[j].min >= Dis2[j].min)
				for (int l = 0; l < G.n; l++) {
					Dis1[l].prev = Dis2[l].prev;
					Dis1[l].lowcost = Dis2[l].lowcost;
					Dis1[l].h = Dis2[l].h;
					Dis1[l].min = Dis2[l].min;
				}
			return;
		}
		else if (G.tags[p->adjvex] == FALSE) {  //���յ㣬������
			ToolP t;
			t = (kind == Plane)? p->Plane : p->Train;
			//�жϿɴ�
			if (Between1 < t->time->StartDate) {
				Dis2[p->adjvex].lowcost = t->time->EndDate;
				Dis2[p->adjvex].h = t->time->EndHour;
				Dis2[p->adjvex].min = t->time->EndMin;

				//�ݹ�����յ�
				DFS(G, p->adjvex, j, Dis1, Dis2, kind, t->time->EndDate, t->time->EndHour, t->time->EndMin);
				Dis2[p->adjvex].prev = i;
			}else if(Between1 == t->time->StartDate && Between2 < t->time->StartHour) {
				Dis2[p->adjvex].lowcost = t->time->EndDate;
				Dis2[p->adjvex].h = t->time->EndHour;
				Dis2[p->adjvex].min = t->time->EndMin;
				DFS(G, p->adjvex, j, Dis1, Dis2, kind, t->time->EndDate, t->time->EndHour, t->time->EndMin);
				Dis2[p->adjvex].prev = i;
			}else if(Between1 == t->time->StartDate && Between2 == t->time->StartHour && Between3 <= t->time->StartMin) {
				Dis2[p->adjvex].lowcost = t->time->EndDate;
				Dis2[p->adjvex].h = t->time->EndHour;
				Dis2[p->adjvex].min = t->time->EndMin;
				DFS(G, p->adjvex, j, Dis1, Dis2, kind, t->time->EndDate, t->time->EndHour, t->time->EndMin);
				Dis2[p->adjvex].prev = i;
			}
		}
		p = p->nextArc;
	}
	return;
}

/*�������·��*/
void DFSTraverse(AlGraph G, VexType v[], VexType w[], Dis & Dis1, ToolKind kind) {
	int i = LocateVex_AL(G, v);
	int j = LocateVex_AL(G, w);
	for (int k = 0; k < G.n; k++)
		G.tags[k] = FALSE;

	Dis Dis2;
	Dis2 = (DisInfo *)malloc(G.n * sizeof(DisInfo));
	for (int j = 0; j < G.n; j++) {
		Dis1[j].lowcost = INFI*INFI;
		Dis2[j].lowcost = INFI*INFI;
		Dis1[j].h = INFI*INFI;
		Dis2[j].h = INFI*INFI;
		Dis1[j].min = INFI*INFI;
		Dis2[j].min = INFI*INFI;
	}
	Dis1[i].prev = -1;
	Dis2[i].prev = -1;
	DFS(G, i, j, Dis1, Dis2, kind, 0, 0, 0);
}


/*
 *��ʼ��ͼ, a-cost �Ǳ���Ϣ
 */
void MakeNode(ArcInfo *arc, char a[], char b[], int k1, int k2, int k3, int k4, int k5, int k6, double cost1, int t1, int t2, int t3, int t4, int t5, int t6, double cost2) {
	ToolP address1 = (ToolP)malloc(sizeof(Tool));
	ToolP address2 = (ToolP)malloc(sizeof(Tool));
	TimeInfoP P = (TimeInfo *)malloc(sizeof(TimeInfo));
	TimeInfoP T = (TimeInfo *)malloc(sizeof(TimeInfo));

	*P = { k1, k2, k3, k4, k5, k6 };
	*T = { t1, t2, t3, t4, t5, t6 };
	address1->cost = cost1; address1->time = P;
	address2->cost = cost2; address2->time = T;
	strcpy_s(arc->v, a); strcpy_s(arc->w, b);
	arc->Plane = address1;
	arc->Train = address2;
}

/*
 *����Ա����
 */
void Master(AlGraph &G) {
	int mas, Sd, Ed, Sh, Eh, Sm, Em, $cost;
	char a[7], b[7], c[7];
	char Pl[7] = "�ɻ�";
	ArcInfo p;
	ToolKind kind;
	printf("\n\n\n\n\n\n\n");
	printf("                       **************************************\n");
	printf("                       ***           ����Աϵͳ	         ***\n");
	printf("                       ***                                ***\n");
	printf("                       ***          1�����·��           ***\n");
	printf("                       ***          2��ɾ��·��           ***\n");
	printf("                       ***          3���޸�·��           ***\n");
	printf("                       ***          4����ӳ���           ***\n");
	printf("                       ***          5��ɾ������           ***\n");
	printf("                       ***          6��������ҳ           ***\n");
	printf("                       **************************************\n");
	scanf_s("%d", &mas);
	system("cls");
	if (mas == 1) {
		int cz;
		printf("\t\t\t\t\t������'1'���ع���ҳ��\n");
		printf("\t\t\t\t\t������'2'��������\n");
		scanf("%d", &cz);
		getchar();
		if (cz == 1)
			Master(G);
		printf("\t\t\t\t���������� ������ �յ���� �ɻ�/�� ����ʱ��(����������ʱ��) ����ʱ�� Ʊ��\n");
		
		gets_s(a);
		gets_s(b);
		gets_s(c);
		scanf_s("%d %d %d %d %d %d %d", &Sd, &Sh, &Sm, &Ed, &Eh, &Em, &$cost);
		strcpy_s(p.v, a);
		strcpy_s(p.w, b);
		if (0 == strcmp(c, Pl)) {
			p.Plane = (Tool *)malloc(sizeof(Tool));
			p.Plane->time = (TimeInfo *)malloc(sizeof(TimeInfo));
			p.Plane->cost = $cost;
			*(p.Plane->time) = {Sd, Sh, Sm, Ed, Eh, Em};
			p.Train = NULL;
		}
		else {
			p.Train = (Tool *)malloc(sizeof(Tool));
			p.Train->time = (TimeInfo *)malloc(sizeof(TimeInfo));
			p.Train->cost = $cost;
			*(p.Train->time) = { Sd, Sh, Sm, Ed, Eh, Em };
			p.Plane = NULL;
		}
		AddLine(G, p);
		printf("\n�밴�������������");
		if (getchar())
			Master(G);
	}
	else if (2 == mas) {
		printf("����������Ҫɾ��·����Ϣ ������ �յ���� �ɻ�/��\n");
		getchar();
		gets_s(a);
		gets_s(b);
		gets_s(c);
		kind = (0 == strcmp(c, Pl)) ? Plane : Train;
		DeleteLine(G, a, b, kind);
		printf("\n�밴�������������");
		getchar();
		Master(G);
	}
	else if (3 == mas) {
		printf("�����������޸���Ϣ ������ �յ���� �ɻ�/�� ����ʱ��(����������ʱ��) ����ʱ�� Ʊ��\n");
		getchar();
		gets_s(a);
		gets_s(b);
		gets_s(c);
		scanf_s("%d %d %d %d %d %d %d", &Sd, &Sh, &Sm, &Ed, &Eh, &Sm, &$cost);
		strcpy_s(p.v, a);
		strcpy_s(p.w, b);
		kind = (0 == strcmp(c, Pl)) ? Plane : Train;
		if (kind == Plane) {
			p.Plane = (Tool *)malloc(sizeof(Tool));
			p.Plane->time = (TimeInfo *)malloc(sizeof(TimeInfo));
			p.Plane->cost = $cost;
			*(p.Plane->time) = { Sd, Sh, Sm, Ed, Eh, Em };
			p.Train = NULL;
		}
		else {
			p.Train = (Tool *)malloc(sizeof(Tool));
			p.Train->time = (TimeInfo *)malloc(sizeof(TimeInfo));
			p.Train->cost = $cost;
			*(p.Train->time) = { Sd, Sh, Sm, Ed, Eh, Em };
			p.Plane = NULL;
		}
		RenewLine(G, p, kind);
		printf("\n�밴�������������");
		getchar();
		getchar();
		Master(G);
	}
	else if (4 == mas) {
		printf("����������Ҫ����ĳ�����\n");
		getchar();
		gets_s(a);
		AddCity(G, a);
		printf("\n�밴�������������");
		getchar();
		Master(G);
	}
	else if(5 == mas){
		printf("����������Ҫɾ���ĳ�����\n");
		getchar();
		gets_s(a);
		DeleteCity(G, a);
		printf("\n�밴�������������");
		getchar();
		Master(G);
	}
	else {
		window(G);
	}
	return;
}


/* 
 * ��¼����
 */
void MasterLogin(AlGraph &G) {
	int num, pass;
	printf("�������˺ţ� ");
	scanf_s("%d", &num);
	printf("���������룺 ");
	scanf_s("%d", &pass);
	getchar();
	system("cls");
	if (num == accountNum && pass == passWord)
		Master(G);
	else {
		int i;
		printf("\t\t\t�˺Ż�����������\n1��������ҳ�棬\t2����������");
		scanf_s("%d", &i);
		system("cls");
		if (i == 1)
			window(G);
		else
			MasterLogin(G);
	}
	return;
}


/*
 * �û�����
 */
void User(AlGraph &G) {
	int sel, way;
	char a[7], b[7];
	ToolKind kind;
	Dist dist1;
	int ere;
	printf("\t\t������  0�������ѯ�� 2�����뷵����һ����\n\n");
	scanf_s("%d", &ere);
	if (ere == 1)
		window(G);
	printf("���������������յ���У�\n");
	getchar();
	gets_s(a);
	gets_s(b);
	printf("\n\n\n\n\n\n\n");
	printf("               **************************************\n");
	printf("               ***          ��ѡ��ͨ��ʽ��	      ***\n");
	printf("               ***            1���ɻ�             ***\n");
	printf("               ***            2����             ***\n");
	printf("               **************************************\n");
	scanf_s("%d", &sel);
	system("cls");
	kind = (sel == 1) ? Plane : Train;
	printf("              ********************************************\n");
	printf("              ***     ����Ҫ��ѯ����                   ***\n");
	printf("              ***            1����쵽��·��           ***\n");
	printf("              ***            2����ʡǮ·��             ***\n");
	printf("              ***            3�����ٻ���·��           ***\n");
	printf("              ***            4��������ҳ               ***\n");
	printf("              ***                                      ***\n");
	printf("              ********************************************\n\n");
	scanf_s("%d", &way);
	int i = LocateVex_AL(G, a);
	int j = LocateVex_AL(G, b);
	int dd, hh, mm;
	if (1 == way) {
		Dis dis1;
		dis1 = (DisInfo *)malloc(G.n * sizeof(DisInfo));
		DFSTraverse(G, a, b, dis1, kind);
		if (G.tags[j] == TRUE){
			PrintTheFastMoneyWay(G, dis1, j, i, kind);
		}
		else
			printf("Failed To Find The Line!");
		int read;
		printf("\n\n����������1��������ѯ��2��������ҳ\n");
		scanf_s("%d", &read);
		if (read == 1)
			User(G);
		else
			window(G);
	}
	else if (2 == way) {
		double money = 0.0;
		dist1 = (DistInfo *)malloc(G.n * sizeof(DistInfo));
		TheLessMoneyWay(G, i, dist1, kind);
		//OutputPath(G, dist1, j, i, kind, dd, hh, mm);
		PrintTheLessMoneyWay(G, dist1, j, i, kind);
		printf("�ܵĻ���Ϊ%lf\n", dist1[j].lowcost);
		int read;
		printf("\n\n����������1��������ѯ��2��������ҳ\n");
		scanf_s("%d", &read);
		if (read == 1)
			User(G);
		else
			window(G);
	}
	else if (3 == way) {
		dist1 = (DistInfo *)malloc(G.n * sizeof(DistInfo));
		TheLessChangeWay(G, i, dist1, kind);
		PrintTheLessChangeWay(G, dist1, j, i, kind);
		printf("���ٻ��˴���Ϊ%d\n", change);
		int read;
		printf("\n\n����������1��������ѯ��2��������ҳ\n");
		scanf_s("%d", &read);
		if (read == 1)
			User(G);
		else
			window(G);
	}
	else if (4 == way) {
		User(G);
	}
	return;
}

/*
 * ����������ʼ���
 */
void window(AlGraph &G) {
	printf("\n\n\n\n\n\n\n********************************************************************************\n");
	printf("********************************************************************************\n");
	printf("*~*~*~*~*~                         ѧԺ�������ѧԺ                    ~*~*~*~*~*\n");
	printf("*~*~*~*~*~                         רҵ���������                      ~*~*~*~*~*\n");
	printf("*~*~*~*~*~                         ������ ���                         ~*~*~*~*~*\n");
	printf("*~*~*~*~*~                         ѧ�ţ�3117004993                    ~*~*~*~*~*\n");
	printf("********************************************************************************\n");
	printf("********************************************************************************\n");
	printf("�밴�κμ��Լ�������");
	getchar();
	system("cls");

	int mas;
	printf("\n\n\n\n\n\n\n");
	printf("                       ***************************************\n");
	printf("                       ***    ��ӭʹ��ȫ����ͨ����ϵͳ     ***\n");
	printf("                       ***                                 ***\n");
	printf("                       ***          ��������             ***\n");
	printf("                       ***          1���ɰ��Ĺ���Ա        ***\n");
	printf("                       ***          2���������Ŀͻ�        ***\n");
	printf("                       ***          3������                ***\n");
	printf("                       ***************************************\n");
	scanf_s("%d", &mas);
	system("cls");
	if (1 == mas)
		MasterLogin(G);  //����Ա��½
	else if (2 == mas)
		User(G);    //�û�����
	else
		return;
}

int main()
{
	//printf("Hello World!\n");
	AlGraph G;
	ArcInfo *arcs;
	char a[7] = "����";
	char b[7] = "����";
	char c[7] = "���";
	char d[7] = "����";
	char e[7] = "����";
	char f[7] = "����";
	char g[7] = "����";
	char h[7] = "�Ϻ�";
	char i[7] = "����";
	char j[7] = "�½�";
	char *vexs1[] = {a, b, c, d, e, f, g, h, i, j};  //���м���
	arcs = (ArcInfo*)malloc(16*sizeof(ArcInfo));
	
	//����·����Ϣ
	MakeNode(&arcs[0], a, b, 20190107, 5, 20, 20190107, 21, 50, 300, 20190107, 9, 30, 20190109, 6, 30, 210);
	MakeNode(&arcs[1], b, a, 20190108, 21, 20, 20190109, 6, 50, 300, 20190106, 7, 20, 20190108, 6, 10, 210);
	MakeNode(&arcs[2], b, e, 20190102, 15, 10, 20190102, 23, 50, 200, 20190101, 10, 30, 20190103, 10, 20, 170);
	MakeNode(&arcs[3], b, h, 20190111, 5, 20, 20190111, 18, 50, 290, 20190110, 16, 30, 20190111, 6, 30, 220);
	MakeNode(&arcs[4], b, d, 20190107, 23, 50, 20190108, 7, 50, 100, 20190109, 11, 30, 20190110, 6, 30, 60);
	MakeNode(&arcs[5], a, d, 20190108, 6, 20, 20190108, 22, 50, 210, 20190109, 20, 30, 20190111, 4, 30, 180);
	MakeNode(&arcs[6], d, h, 20190107, 6, 20, 20190111, 22, 50, 260, 20190107, 6, 30, 20190111, 6, 30, 210);
	MakeNode(&arcs[7], h, c, 20190201, 14, 20, 20190201, 15, 50, 100, 20190202, 9, 30, 20190203, 6, 30, 60);
	MakeNode(&arcs[8], c, i, 20190202, 13, 15, 20190203, 12, 50, 800, 20190204, 10, 30, 20190207, 12, 30, 500);
	MakeNode(&arcs[9], i, c, 20190204, 5, 20, 20190204, 17, 50, 800, 20190109, 12, 25, 201902012, 7, 15, 500);
	MakeNode(&arcs[10], a, g, 20190107, 5, 20, 20190108, 13, 50, 400, 20190107, 13, 30, 20190110, 12, 30, 200);
	MakeNode(&arcs[11], g, j, 20190108, 14, 20, 20190109, 21, 50, 180, 20190110, 15, 30, 20190111, 6, 30, 100);
	MakeNode(&arcs[12], j, h, 20190109, 12, 20, 20190111, 23, 50, 400, 20190112, 9, 30, 20190114, 6, 30, 300);
	MakeNode(&arcs[13], g, f, 201901010, 5, 20, 20190110, 21, 50, 200, 20190112, 9, 30, 20190114, 6, 30, 150);
	MakeNode(&arcs[14], f, e, 201901011, 5, 20, 20190111, 21, 50, 200, 20190115, 9, 30, 20190115, 6, 30, 150);
	MakeNode(&arcs[15], a, e, 20190108, 5, 20, 20190109, 6, 50, 200, 20190107, 9, 30, 20190110, 6, 30, 120);
	
	//printf("%d\n", arcs[1].Train->time->StartDate);
	/*
	*P = {20190107, 5, 20, 20190107, 21, 50};
	*T = { 20190107, 9, 30, 20190109, 6, 30 };
	address1->cost = 300; address1->time = P;
	address2->cost = 210; address2->time = T;
	strcpy(arcs[0].v, a); strcpy(arcs[0].w, b);
	arcs[0].Plane = address1;
	arcs[0].Train = address2;
	*/
	//printf("%d", arcs[0].Plane->time->EndDate);
	//printf("%d", sizeof(arcs[0].v));
	CreateDG_Al(G, vexs1, 10, arcs, 16);
	Print(G);
	window(G);
	Print(G);
	getchar();
	return 0;
}