#include <stdio.h>
#include <stdlib.h>

#define MAXRATE 101
#define EXP 10001

typedef struct pm{
    int A, B;
    double rate;
}Pokemon;

int max (int a, int b){
    if (a > b)
        return a;
    else
        return b;
}

int cmp(const void *a, const void *b){
    Pokemon *x = (Pokemon *) a;
    Pokemon *y = (Pokemon *) b;
    if (x->rate < y->rate)
        return 1;
    else if (x->rate > y->rate)
        return -1;
}


int main(void){
    int N, K;
    scanf("%d%d", &N, &K);
    int dp[2][K + 1][EXP];
    Pokemon all[N];
    for (int i = 0; i < N; i++){
        scanf("%d%d", &all[i].A, &all[i].B);
        if (all[i].A != 0)
            all[i].rate = (double)all[i].B / all[i].A;
        else
            all[i].rate = MAXRATE;
    }
    qsort(all, N, sizeof(Pokemon), cmp);

    for (int i = 0; i < 2; i++){
        for (int j = 0; j <= K; j++){
            for (int exp = 0; exp < EXP; exp++){
                dp[i][j][exp] = -1;
            }
        }
    }

    dp[0][1][all[0].B] = 0;
    int now, last;
    for (int i = 1; i < N; i++){//i = current to which pm
        if (i % 2){
            now = 1;
            last = 0;
        }
        else{
            now = 0;
            last = 1;
        }
        for (int j = 1; j <= K; j++){//j = pick number
            if (j > i + 1)//pick more than current
                continue;
            for (int exp = 0; exp < EXP; exp++){//exp = possible experience combination
                if (j == 1){
                    if (exp == all[i].B)
                        dp[now][j][exp] = 0;
                    else
                        dp[now][j][exp] = dp[last][j][exp];
                }
                else{
                    if (dp[last][j - 1][exp - all[i].B] != -1)
                        dp[now][j][exp] = max(dp[last][j - 1][exp - all[i].B] + all[i].A * (exp - all[i].B), dp[last][j][exp]);
                    else
                        dp[now][j][exp] = dp[last][j][exp];
                }
            }
        }
    }
    int ans = -1;
    for (int i = 0; i < 2; i++){
        for (int exp = 0; exp < EXP; exp++){
            if (dp[i][K][exp] > ans)
                ans = dp[i][K][exp];
        }
    }
    printf("%d\n", ans);
    return 0;
}
