#include <stdio.h>
#include <stdlib.h>


#define MAXN 200000
#define TIMES 375

typedef struct list{
	int index;
	struct list *next;
}List;

typedef struct node{
	int index, indeg, outdeg, dt, ft;
	//int min[TIMES];
	List *neighbor;
}Node;

Node *new_node(int i){
	Node *add = (Node *) malloc(sizeof(Node));
	add->index = i;
	add->indeg = 0;
	add->outdeg = 0;
	add->dt = 0;
	add->ft = 0;
	add->neighbor = NULL;
	return add;
}

List *new_neighbor(List *cur, int B){
	List *add = (List *) malloc(sizeof(List));
	add->index = B;
	add->next = cur;
	return add;
}

int dfstime = 1, count = 1;
int topo[MAXN + 1];

void topo_sort(Node *allnode[], int i){
	allnode[i]->dt = dfstime++;
	List *head = allnode[i]->neighbor;
	while (head != NULL){
		if (allnode[head->index]->dt == 0){
			topo_sort(allnode, head->index);
		}
		head = head->next;
	}
	allnode[i]->ft = dfstime++;
	topo[count++] = i;
	return;
}

int minhash[MAXN + 1][TIMES];

int main(void){
	int N, M, Q;
	scanf("%d%d%d", &N, &M, &Q);
	Node *allnode[N + 1];
	allnode[0] = NULL;
	for (int i = 1; i <= N; i++){
		allnode[i] = new_node(i);
	}
	int A, B;
	for (int i = 0; i < M; i++){
		scanf("%d%d", &A, &B);
		allnode[A]->outdeg += 1;
		allnode[B]->indeg += 1;
		allnode[A]->neighbor = new_neighbor(allnode[A]->neighbor, B);
	}
	for (int i = 1; i <= N; i++){
		if (allnode[i]->indeg == 0){
			topo_sort(allnode, i);
		}
	}

	for (int i = 1; i <= N; i++){
		for (int j = 0; j < TIMES; j++){
			minhash[i][j] = rand() * rand();
		}
	}

	/*for (int j = 0; j < TIMES; j++){
		for (int i = 1; i <= N; i++){
			allnode[i]->min[j] = (rand() * rand()) % 43112609;
		}
	}*/

	for (int i = 1; i <= N; i++){
		int cur = topo[i];
		List *head = allnode[cur]->neighbor;
		while (head != NULL){
			for (int j = 0; j < TIMES; j++){
				if (minhash[cur][j] > minhash[head->index][j])
					minhash[cur][j] = minhash[head->index][j];
				/*if (allnode[cur]->min[j] > allnode[head->index]->min[j])
					allnode[cur]->min[j] = allnode[head->index]->min[j];*/
			}
			head = head->next;
		}
	}
	int same;
	double ans;
	for (int i = 0; i < Q; i++){
		same = 0;
		scanf("%d%d", &A, &B);
		for (int j = 0; j < TIMES; j++){
			//if (allnode[A]->min[j] == allnode[B]->min[j])
			if (minhash[A][j] == minhash[B][j])
				same++;
		}
		ans = (double) same / (double) TIMES;
		printf("%lf\n", ans);
	}
	return 0;
}