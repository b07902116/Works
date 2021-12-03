#include <stdio.h>
#include <stdlib.h>

int cmp(const void *a, const void *b){
    int *x = (int *)a;
    int *y = (int *)b;
    if (*x >= *y)
        return -1;
    return 1;
}

int initial(int s[], int ss[], int pos[], int enemy[], int N){
    ss[0] = s[0] + s[1] + s[2];
    ss[1] = s[0] + s[1];
    ss[2] = s[0] + s[2];
    ss[3] = s[1] + s[2];
    ss[4] = s[0];
    ss[5] = s[1];
    ss[6] = s[2];
    int r = 0;
    if (ss[3] <= ss[4]){
        r = ss[3];
        ss[3] = ss[4];
        ss[4] = r;
        r = 1;
    }
    int index = 0;
    for (int i = 0; i < N; i++){
        while (index < 7 && enemy[i] <= ss[index]){
            pos[index++] = i;
        }
    }
    while (index < 7)
        pos[index++] = N;
    return r;
}

int next_pos(int enemy[], int N, int cur_pos){
    while (cur_pos < N && enemy[cur_pos] == -1)
        cur_pos++;
    return cur_pos;
}

int main(void){
    int N, s[3], ss[7], pos[7];
    scanf("%d%d%d%d", &N, &s[0], &s[1], &s[2]);
    int time = 0;
    int enemy[N + 1];
    for (int i = 0; i < N; i++)
        scanf("%d", &enemy[i]);
    enemy[N] = -1;
    qsort(s, 3, sizeof(int), cmp);
    qsort(enemy, N, sizeof(int), cmp);
    int rev = initial(s, ss, pos, enemy, N);
    int ori_pos[7];
    for (int i = 0; i < 7; i++)
        ori_pos[i] = pos[i];
    if (enemy[0] > ss[0]){
        printf("-1\n");
        return 0;
    }
    for (int i = pos[0]; i < ori_pos[1]; i++){
        time++;
        enemy[i] = -1;
    }
    for (int i = pos[1]; i < ori_pos[2]; i++){
        pos[1]++;
        pos[6] = next_pos(enemy, N, pos[6]);
        time++;
        enemy[i] = -1;
        enemy[pos[6]] = -1;
    }
    for (int i = pos[2]; i < ori_pos[3]; i++){
        pos[2]++;
        pos[5] = next_pos(enemy, N, pos[5]);
        time++;
        enemy[i] = -1;
        enemy[pos[5]] = -1;
    }
    if (rev == 0){//s[1] + s[2] > s[0]
        for (int i = pos[3]; i < ori_pos[4]; i++){
            pos[3]++;
            pos[4] = next_pos(enemy, N, pos[4]);
            time++;
            enemy[i] = -1;
            enemy[pos[4]] = -1;
        }
        pos[5] = next_pos(enemy, N, pos[5]);
        pos[6] = next_pos(enemy, N, pos[6]);
        while (pos[5] < N || pos[6] < N){
            pos[4] = next_pos(enemy, N, pos[4]);
            enemy[pos[4]] = -1;
            pos[5] = next_pos(enemy, N, pos[5]);
            enemy[pos[5]] = -1;
            pos[6] = next_pos(enemy, N, pos[6]);
            enemy[pos[6]] = -1;
            pos[5] = next_pos(enemy, N, pos[5]);
            pos[6] = next_pos(enemy, N, pos[6]);
            time++;
        }
        pos[3] = next_pos(enemy, N, pos[3]);
        pos[4] = next_pos(enemy, N, pos[4]);
        while (pos[3] < ori_pos[5] || pos[4] < ori_pos[5]){
            pos[3] = next_pos(enemy, N, pos[3]);
            enemy[pos[3]] = -1;
            pos[4] = next_pos(enemy, N, pos[4]);
            enemy[pos[4]] = -1;
            pos[3] = next_pos(enemy, N, pos[3]);
            pos[4] = next_pos(enemy, N, pos[4]);
            time++;
        }
    }
    else{//s[1] + s[2] <= s[0]
        pos[5] = next_pos(enemy, N, pos[5]);
        pos[6] = next_pos(enemy, N, pos[6]);
        while (pos[5] < N || pos[6] < N){
            pos[3] = next_pos(enemy, N, pos[3]);
            enemy[pos[3]] = -1;
            pos[5] = next_pos(enemy, N, pos[5]);
            enemy[pos[5]] = -1;
            pos[6] = next_pos(enemy, N, pos[6]);
            enemy[pos[6]] = -1;
            pos[5] = next_pos(enemy, N, pos[5]);
            pos[6] = next_pos(enemy, N, pos[6]);
            time++;
        }
        pos[3] = next_pos(enemy, N, pos[3]);
        pos[4] = next_pos(enemy, N, pos[4]);
        while (pos[3] < ori_pos[5] || pos[4] < ori_pos[5]){
            pos[3] = next_pos(enemy, N, pos[3]);
            enemy[pos[3]] = -1;
            pos[4] = next_pos(enemy, N, pos[4]);
            enemy[pos[4]] = -1;
            pos[3] = next_pos(enemy, N, pos[3]);
            pos[4] = next_pos(enemy, N, pos[4]);
            time++;
        }
    }
    printf("%d\n", time);
    return 0;
}
