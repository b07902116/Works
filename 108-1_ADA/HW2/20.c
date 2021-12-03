#include <stdio.h>
#include <stdlib.h>


int note[100000], dp[100000][300];
int N, M, K;

int effort(int a, int b){
    int rev = abs(a - b);
    if (rev <= K)
        rev = 0;
    else
        rev -= K;
    return rev;
}

int min(int a, int b){
    if (a < b)
        return a;
    else
        return b;
}


int main(void){
    scanf("%d%d%d", &N, &M, &K);
    for (int i = 0; i < N; i++)
        scanf("%d", &note[i]);
    for (int j = 0; j < M; j++)
        dp[0][j] = 0;
    int change[M], deq[M];
    int head = 0, tail = 0, eff, rb;
    for (int i = 1; i < N; i++){
        //change
        for (int j = 0; j < M; j++)
            change[j] = effort(note[i - 1], j) + effort(note[i], j);
        //not_change
        head = 1;
        tail = 0;
        eff = effort(note[i - 1], note[i]);
        for (int j = 0; j < K; j++){
            while ((head <= tail) && (dp[i - 1][deq[tail]] > dp[i - 1][j]))
                tail--;
            deq[++tail] = j;
        }
        for (int j = 0; j < M; j++){
            rb = j + K;
            if (rb < M){//push
                while ((head <= tail) && (dp[i - 1][deq[tail]] > dp[i - 1][rb]))
                    tail--;
                deq[++tail] = rb;
            }
            if (deq[head] < (j - K))//pop
                head++;
            dp[i][j] = min((dp[i - 1][deq[head]] + eff), dp[i - 1][j] + change[j]);
            printf("%d  %d\n", dp[i - 1][deq[head]] + eff, dp[i - 1][j] + change[j]);
        }
    }
    for (int j = 0; j < M; j++){
        for (int i = 0; i < N; i++){
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }
    int ans = N * M;
    for (int j = 0; j < M; j++)
        if (dp[N - 1][j] < ans)
            ans = dp[N - 1][j];
    printf("%d\n", ans);
    return 0;
}
