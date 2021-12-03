#include <stdio.h>
#include <stdlib.h>

#define WHITE 0
#define GRAY 1
#define BLACK 2


typedef struct list{
	struct node *to_node;
	struct list *next;
}List;

typedef struct node{
	int song, note, color;
	long long max;
	struct node *next;
	List *neighbor;
}Node;

List *new_neighbor(List *head, Node *to){
	List *add = (List *) malloc(sizeof(List));
	add->to_node = to;
	add->next = head;
	return add;
}

List *change_neighbor(List *head, Node *old, Node *rpl){
	List *cur = head;
	while (cur->to_node != old){
		cur = cur->next;
	}
	cur->to_node = rpl;
	return head;
}

Node *new_song(int i, int length){
	Node *end = (Node *) malloc(sizeof(Node));
	end->song = i;
	end->note = length;
	end->color = WHITE;
	end->max = -1;
	end->next = NULL;
	end->neighbor = NULL;
	Node *head = (Node *) malloc(sizeof(Node));
	head->song = i;
	head->note = 0;
	head->color = WHITE;
	head->max = -1;
	head->neighbor = new_neighbor(NULL, end);
	head->next = end;
	return head;
}

Node *new_vertex(Node *head, int new_note){
	Node *cur = head, *prev;
	while (cur != NULL){
		if (cur->note == new_note){//the vertex already exist
			return head;
		}
		else if (cur->note < new_note){
			prev = cur;
			cur = cur->next;
		}
		else{
			Node *add = (Node *) malloc(sizeof(Node));
			add->song = cur->song;
			add->note = new_note;
			add->color = WHITE;
			add->max = -1;
			add->neighbor = new_neighbor(NULL, cur);
			add->next = cur;
			prev->neighbor = change_neighbor(prev->neighbor, cur, add);
			prev->next = add;
			return head;
		}
	}
}

void new_edge(Node *allsong[], int A, int a, int B, int b){
	allsong[A] = new_vertex(allsong[A], a);
	allsong[B] = new_vertex(allsong[B], b);
	Node *head = allsong[A], *nodeB = allsong[B];
	while (head->note != a)
		head = head->next;
	while (nodeB->note != b)
		nodeB = nodeB->next;
	head->neighbor = new_neighbor(head->neighbor, nodeB);
	return;
}

int find_cycle(List *all_neighbor){
	List *head = all_neighbor;
	while (head != NULL){
		Node *u = head->to_node;
		if (u->color == WHITE){
			u->color = GRAY;
			if (find_cycle(u->neighbor)){
				return 1;
			}
		}
		else if (u->color == GRAY){
			return 1;
		}
		u->color = BLACK;
		head = head->next;
	}
	return 0;
}

long long get_length(Node *to_find){
	if (to_find->max != -1)
		return to_find->max;
	if (to_find->neighbor == NULL){//end of a song
		to_find->max = 0;
		return to_find->max;
	}
	long long tmp;
	List *head = to_find->neighbor;
	while (head != NULL){
		Node *u = head->to_node;
		if (u->song == to_find->song)
			tmp = get_length(u) + (u->note - to_find->note);//same song
		else
			tmp = get_length(u) + 1;//jump
		if (tmp > to_find->max)
			to_find->max = tmp;
		head = head->next;
	}
	return to_find->max;
}

void free_neighbor(List *head){
	if (head == NULL)
		return;
	List *tmp = head->next;
	free(head);
	while (tmp != NULL){
		head = tmp;
		tmp = tmp->next;
		free(head);
	}
	return;
}

void free_all(Node *allsong[], int N){
	for (int i = 1; i <= N; i++){
		Node *head = allsong[i], *prev;
		while (head != NULL){
			free_neighbor(head->neighbor);
			prev = head;
			head = head->next;
			free(prev);
		}
	}
	return;
}

typedef struct edge{
	int A, a, B, b;
}Edge;

int cmp(const void *p, const void *q){
	Edge *x = (Edge *) p;
	Edge *y = (Edge *) q;
	if (x->A < y->A)
		return 1;
	if (x->A > y->A)
		return -1;
	if (x->a < y->a)
		return 1;
	if (x->a > y->a)
		return -1;
	if (x->B < y->B)
		return 1;
	if (x->B > y->B)
		return -1;
	if (x->b < y->b)
		return 1;
	if (x->b > y->b)
		return -1;
	return 1;
}

int main(void){
	int T, round;
	scanf("%d", &T);
	for (round = 0; round < T; round++){
		int N, M;
		scanf("%d%d", &N, &M);
		Node *allsong[N + 1];
		int length;
		//build every song's begin and end
		for (int i = 1; i <= N; i++){
			scanf("%d", &length);
			allsong[i] = new_song(i, length);
		}
		//build jump points
		Edge alledge[M];
		for (int i = 0; i < M; i++)
			scanf("%d%d%d%d", &alledge[i].A, &alledge[i].a, &alledge[i].B, &alledge[i].b);
		qsort(alledge, M, sizeof(Edge), cmp);
		for (int i = 0; i < M; i++)
			new_edge(allsong, alledge[i].A, alledge[i].a, alledge[i].B, alledge[i].b);
		//DFS detect cycle
		int cycle = 0;
		for (int i = 1; i <= N; i++){
			cycle = find_cycle(allsong[i]->neighbor);
			if (cycle){
				printf("LoveLive!\n");
				//free_all(allsong, N);
				break;
			}
		}
		if (cycle)
			continue;
		//find the longest path
		long long max = -1;
		for (int i = 1; i <= N; i++){
			if (max < get_length(allsong[i]))
				max = get_length(allsong[i]);
		}
		printf("%lld\n", max);

		//free_all(allsong, N);
	}
	return 0;
}