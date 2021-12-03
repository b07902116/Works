#include <stdio.h>
#include <algorithm>

typedef struct driver{
    int prefer, range;
}Driver;

void fill(Driver *a, Driver *b){
    a->prefer = b->prefer;
    a->range = b->range;
    return;
}


void Divide(Driver one[], int l, int r, long long *greet){
    if (l >= r)
        return;
    int m = (l + r) / 2;
    Divide(one, l, m, greet);
    Divide(one, m + 1, r, greet);
    int len1 = m - l + 1;
    int len2 = r - m;
    int Larr[len1], Rarr[len2];
    Driver L[len1], R[len2];
    int j = 0, k = 0, pos = l;
    for (int i = 0; i < len1; i++){
        Larr[i] = one[i + l].prefer;
        fill(&L[i], &one[i + l]);
    }
    for (int i = 0; i < len2; i++){
        Rarr[i] = one[i + m + 1].prefer;
        fill(&R[i], &one[i + m + 1]);
    }
    while (j < len1 && k < len2){
        if (L[j].prefer <= R[k].prefer)
            fill(&one[pos++], &L[j++]);
        else
            fill(&one[pos++], &R[k++]);
    }
    while (j < len1)
        fill(&one[pos++], &L[j++]);
    while (k < len2)
        fill(&one[pos++], &R[k++]);

    for (int i = 0; i < len2; i++){
        int low = std::lower_bound(Larr, Larr + len1, R[i].prefer - R[i].range) - Larr;
        int high = std::upper_bound(Larr, Larr + len1, R[i].prefer + R[i].range) - Larr;
        if (low != len1)
            (*greet) += (high - low);
    }
    return;
}


int main(void){
    int N;
    long long greet = 0;
    scanf("%d", &N);
    Driver one[N];
    for (int i = 0; i < N; i++)
        scanf("%d", &one[i].prefer);
    for (int i = 0; i < N; i++)
        scanf("%d", &one[i].range);
    Divide(one, 0, N - 1, &greet);
    printf("%lld\n", greet);
    return 0;
}

//discuss with °ªªø¸t
