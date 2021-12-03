#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <queue>

#define INF 9223372036854775807

using namespace std;

typedef struct road{
	int x, y, length, danger;
}Road;

typedef struct list{
	int index, length, danger;
	struct list *next;
}List;

typedef struct node{
	int index;
	long long distance;
}Node;



int find_set(int vertex[][2], int index){
	if (vertex[index][0] != index)
		vertex[index][0] = find_set(vertex, vertex[index][0]);
	return vertex[index][0];
}

void Union(int vertex[][2], int i, int j){
	if (vertex[i][1] > vertex[j][1])
		vertex[j][0] = vertex[i][0];
	else if (vertex[i][1] < vertex[j][1])
		vertex[i][0] = vertex[j][0];
	else{
		vertex[j][0] = vertex[i][0];
		(vertex[i][1])++;
	}
	return;
}

int kruskal(Road edge[], int N, int M, int start, int end){
	int vertex[N][2], count = 0, rev;
	for (int i = 0; i < N; i++){
		vertex[i][0] = i;//point to itself
		vertex[i][1] = 0;//rank
	}
	while (find_set(vertex, start) != find_set(vertex, end)){
		if (count == M)
			break;
		if (find_set(vertex, edge[count].x) == find_set(vertex, edge[count].y)){
			count++;
			continue;
		}
		Union(vertex, find_set(vertex, edge[count].x), find_set(vertex, edge[count].y));
		rev = edge[count].danger;
		count++;
	}
	return rev;
}

List *new_edge(int y, int l, int d, List *neighbor){
	List *node = (List *) malloc(sizeof(List));
	node->index = y;
	node->length = l;
	node->danger = d;
	node->next = neighbor;
	return node;
}


typedef pair<long long, int> pi;

long long dijkstra(List *neighbor[], int N, int start, int end){
	Node point[N];
	priority_queue<pi , vector<pi>, greater<pi> > q;
	for (int i = 0; i < N; i++){
		point[i].index = i;
		point[i].distance = (i == start? 0 : INF);
	}
	pi u;
	long long tmp;
	List *head;
	q.push(make_pair(0, start));
	while (!q.empty()){
		u = q.top();
		q.pop();
		if (u.second == end)
			break;
		head = neighbor[u.second];
		while (head != NULL){
			tmp = u.first + head->length;
			if (tmp < point[head->index].distance){
				point[head->index].distance = tmp;
				q.push(make_pair(tmp, head->index));
			}
			head = head->next;
		}
	}
	return point[end].distance;
}

int cmp(const void *a, const void *b){
	Road *x = (Road *) a;
	Road *y = (Road *) b;
	if (x->danger >= y->danger)
		return 1;
	else
		return -1;
}

int main(void){
	int N, M, start, end;
	scanf("%d%d%d%d", &N, &M, &start, &end);
	Road edges[M];
	List *neighbor[N];
	for (int i = 0; i < M; i++)
		scanf("%d%d%d%d", &edges[i].x, &edges[i].y, &edges[i].length, &edges[i].danger);
	for (int i = 0; i < N; i++)
		neighbor[i] = NULL;
	qsort(edges, M, sizeof(Road), cmp);
	int least = kruskal(edges, N, M, start, end);
	for (int i = 0; i < M; i++){
		if (edges[i].danger > least)
			break;
		neighbor[edges[i].x] = new_edge(edges[i].y, edges[i].length, edges[i].danger, neighbor[edges[i].x]);
		neighbor[edges[i].y] = new_edge(edges[i].x, edges[i].length, edges[i].danger, neighbor[edges[i].y]);
	}
	long long shortest = dijkstra(neighbor, N, start, end);
	printf("%lld %d\n", shortest, least);
	return 0;
}