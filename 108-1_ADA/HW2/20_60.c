#include <stdio.h>
#include <stdlib.h>


int note[100000], dp[100000][300];
int N, M, K;

int effort(int old_one, int new_one, int old_two, int new_two, int change){
    int one, two;
    if (!change){
        one = abs(old_one - new_one) - K;
        two = abs(old_two - new_two) - K;
    }
    else{
        one = abs(old_one - new_two) - K;
        two = abs(old_two - new_one) - K;
    }
    if (one < 0)
        one = 0;
    if (two < 0)
        two = 0;
    return (one + two);
}


int main(void){
    scanf("%d%d%d", &N, &M, &K);
    for (int i = 0; i < N; i++)
        scanf("%d", &note[i]);
    for (int j = 0; j < M; j++)
        dp[0][j] = 0;
    int min, change, not_change, which;
    for (int i = 1; i < N; i++){
        for (int j = 0; j < M; j++){
            min = N * M;
            for (int k = 0; k < M; k++){
                change = dp[i - 1][k] + effort(note[i - 1], note[i], k, j, 1);
                not_change = dp[i - 1][k] + effort(note[i - 1], note[i], k, j, 0);
                which = change < not_change? change : not_change;
                if (which < min)
                    min = which;
            }
            dp[i][j] = min;
        }
    }
    min = N * M;
    for (int j = 0; j < M; j++){
        for (int i = 0; i < N; i++){
            printf("%d ", dp[i][j]);
        }
        printf("\n");
    }
    for (int j = 0; j < M; j++)
        if (dp[N - 1][j] < min)
            min = dp[N - 1][j];
    printf("%d\n", min);
    return 0;
}
