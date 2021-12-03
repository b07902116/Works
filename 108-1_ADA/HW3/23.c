#include <stdio.h>
#include <stdlib.h>


typedef struct node{
	int index, indeg, delnode, visited;
	struct node *next, *jump, *prev;
}Node;

Node *new_node(int i){
	Node *head = (Node *) malloc(sizeof(Node));
	head->index = i;
	head->indeg = 0;
	head->delnode = 1;
	head->visited = 0;
	head->jump = NULL;
	return head;
}

void loop(Node *tail, Node *head){
	Node *cur = tail;
	while (cur != head){
		cur->visited = 3;
		cur = cur->prev;
	}
	head->visited = 3;
}

void back(Node *head, Node *tail){
	Node *cur = tail;
	while (cur != head){
		cur->visited = 2;
		cur = cur->prev;
	}
	head->visited = 2;
	return;
}

void remove_cycle(Node *head){
	Node *cur = head, *last = cur;
	while (cur != NULL){
		cur->visited = 1;
		if (cur->next->visited == 1){//first meet cycle
			loop(cur, cur->next);			
			back(head, cur->next->prev);
			cur->next->prev->next = NULL;
			return;
		}
		else if (cur->next->visited == 2){//cycle is removed
			back(head, cur);
			return;
		}
		else if (cur->next->visited == 3){//meet a removed cycle
			back(head, cur);
			cur->next = NULL;
			return;
		}
		cur->next->prev = cur;
		last = cur;
		cur = cur->next;
	}
	//no cycle
	back(head, last);
	return;
}

void find_jump(Node *head){
	Node *cur = head->next;
	while (cur != NULL){
		if (cur->indeg == 1){
			head->delnode += 1;
		}
		else{
			head->jump = cur;
			if (cur->jump == NULL)
				find_jump(cur);
			return;
		}
		cur = cur->next;
	}
	head->jump = head;
	return;
}


int jumpto[300000] = {0}, queue[300000];

int main(void){
	int N, u;
	scanf("%d", &N);
	Node *allnode[N + 1];
	allnode[0] = NULL;
	for (int i = 1; i <= N; i++){
		allnode[i] = new_node(i);
	}
	for (int i = 1; i <= N; i++){
		scanf("%d", &u);
		allnode[i]->next = allnode[u];
		if (u == 0)
			continue;
		allnode[u]->indeg += 1;
	}
	for (int i = 1; i <= N; i++)
		if (allnode[i]->indeg == 0)
			remove_cycle(allnode[i]);

	for (int i = 1; i <= N; i++)
		if (allnode[i]->indeg == 0)
			find_jump(allnode[i]);
	int Q, X, num = 0, tmp;
	scanf("%d", &Q);
	int lptr = 0, rptr = 0;
	for (int i = 0; i < Q; i++){
		scanf("%d", &X);
		num = 0;
		lptr = 0;
		rptr = 0;
		for (int j = 0; j < X; j++){
			scanf("%d", &tmp);
			num += allnode[tmp]->delnode;
			if (allnode[tmp]->jump != allnode[tmp])
				queue[rptr++] = allnode[tmp]->jump->index;
		}
		while (lptr != rptr){
			jumpto[queue[lptr]]++;
			if (jumpto[queue[lptr]] >= allnode[queue[lptr]]->indeg){
				num += allnode[queue[lptr]]->delnode;
				if (allnode[queue[lptr]]->jump != allnode[queue[lptr]])
					queue[rptr++] = allnode[queue[lptr]]->jump->index;
			}
			lptr++;
		}
		while (lptr >= 0){
			jumpto[queue[lptr--]] = 0;
		}
		printf("%d\n", num);
	}
	return 0;
}