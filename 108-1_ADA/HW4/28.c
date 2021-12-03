#include <stdio.h>
#include <stdlib.h>


typedef struct list{
	int index;
	struct list *next;
}List;

typedef struct node{
	int index, degree;
	List *neighbor;
}Node;


Node *newnode(int i){
	Node *head = (Node *) malloc(sizeof(Node));
	head->index = i;
	head->degree = 0;
	head->neighbor = NULL;
	return head;
}

void connect(Node *A, int i){
	List *add = (List *) malloc(sizeof(List));
	add->index = i;
	add->next = A->neighbor;
	A->neighbor = add;
	return;
	/*List *prev, *cur;
	List *add = (List *) malloc(sizeof(List));
	add->index = i;
	add->next = NULL;
	cur = A->neighbor;
	prev = cur;
	if (cur == NULL){
		A->neighbor = add;
	}
	else{
		while (cur->index < add->index){
			prev = cur;
			cur = cur->next;
			if (cur == NULL)
				break;
		}
		add->next = cur;
		if (cur != prev)
			prev->next = add;
	}
	return;*/
}


int QMAX[120], QMAXsize = 0, found = 0;
int c[120] = {0};




int min_in_R(List *Rhead){
	List *tmp = Rhead;
	int ret = tmp->index;
	while (tmp != NULL){
		ret = tmp->index;
		tmp = tmp->next;
	}
	return ret;
}

List *del_min(List *U, int v){
	List *head = U;
	if (head->index == v)
		return NULL;
	List *prev = head, *tmp = head->next;
	while (tmp->index != v){
		prev = tmp;
		tmp = tmp->next;
	}
	return prev;
}


List *build_Uint(List *U, List *Nv, int *size){
	List *Uint = NULL, *tail = NULL, *add;
	List *Uptr = U, *Nvptr = Nv;
	while (Uptr != NULL && Nvptr != NULL){
		if (Uptr->index == Nvptr->index){
			add = (List *) malloc(sizeof(List));
			add->index = Uptr->index;
			add->next = NULL;
			*size += 1;
			if (Uint == NULL){
				Uint = add;
				tail = add;
			}
			else{
				tail->next = add;
				tail = add;
			}
		}
		else if (Uptr->index > Nvptr->index){
			Uptr = Uptr->next;
		}
		else{
			Nvptr = Nvptr->next;
		}
	}
	return Uint;
}


void MaxClique(Node *allnode[], int N, List *Rhead, int Rsize, int Q[], int size){
	if (Rhead == NULL){
		if (size > QMAXsize){
			QMAXsize = size;
			for (int i = 0; i < size; i++){
				QMAX[i] = Q[i];
			}
			found = 1;
		}
		return;
	}
	List *U = Rhead;
	while (U != NULL){
		if (size + Rsize <= QMAXsize)
			return;
		int v = min_in_R(Rhead);
		if (size + c[v] <= QMAXsize)
			return;
		U = del_min(U, v);
		int Uintsize = 0;
		List *Uint = build_Uint(U, allnode[v]->neighbor, &Uintsize);
		Q[size] = v;
		MaxClique(allnode, N, Uint, Uintsize, Q, size + 1);
	}
	return;
}



int main(void){
	int N;
	scanf("%d", &N);
	char matrix[N][N + 2];
	Node *allnode[N];
	List *Rhead = NULL, *Rtail = NULL, *add;
	int Rsize = 0;
	for (int i = 0; i < N; i++){
		scanf("%s", matrix[i]);
	}
	for (int i = 0; i < N; i++){
		allnode[i] = newnode(i);
	}
	
	for (int i = 0; i < N; i++){
		for (int j = i; j < N; j++){
			if (matrix[i][j] == '1' && matrix[j][i] == '1'){
				if (i == j){
					Rsize++;
					add = (List *) malloc (sizeof(List));
					add->index = i;
					add->next = Rhead;
					Rhead = add;
				}
				else{
					allnode[i]->degree += 1;
					allnode[j]->degree += 1;
					connect(allnode[i], j);
					connect(allnode[j], i);
				}
			}
		}
	}
	/*while (Rhead != NULL){
		printf("%d ", Rhead->index);
		Rhead = Rhead->next;
	}*/
	int Q[N];
	for (int i = N - 1; i >= 0; i--){
		if (Rhead == NULL){
			break;
		}
		found = 0;
		Q[0] = i;
		MaxClique(allnode, N, Rhead, Rsize, Q, 1);
		c[i] = QMAXsize;
		if (Rhead->index == i){
			Rhead = Rhead->next;
			Rsize--;
			//printf("i : %d\n", i);
		}
	}
	for (int i = 0; i < QMAXsize; i++){
		printf("%d ", QMAX[i]);
	}
	return 0;
}