#include <stdio.h>

#define MAXN 1000000

int counting[2 * MAXN + 1] = {0};

void lmax_lmin(int l, int m, int r, int max_arr[], int min_arr[], long long *count){
    int dif, right;
    for (int i = m; i >= l; i--){
        dif = max_arr[i] - min_arr[i];
        right = i + dif;
        if ((right <= m) || (right > r))
            continue;
        if ((max_arr[i] > max_arr[right]) && (min_arr[i] < min_arr[right]))
            (*count)++;
    }
    return;
}

void rmax_rmin(int l, int m, int r, int max_arr[], int min_arr[], long long *count){
    int dif, left;
    for (int i = m + 1; i <= r; i++){
        dif = max_arr[i] - min_arr[i];
        left = i - dif;
        if ((left >= m + 1) || (left < 0))
            continue;
        if ((max_arr[i] > max_arr[left]) && (min_arr[i] < min_arr[left]))
            (*count)++;
    }
    return;
}

void lmax_rmin(int l, int m, int r, int max_arr[], int min_arr[], long long *count){
    int lpot = m + 1, rpot = m + 1;
    for (int i = m; i >= l; i--)
        counting[i + max_arr[i]] = 0;
    for (int i = m; i >= l; i--){
        while ((rpot != (r + 1)) && (max_arr[rpot] < max_arr[i])){
            counting[rpot + min_arr[rpot]]++;
            rpot++;
        }
        while ((lpot != (r + 1)) && (min_arr[lpot] > min_arr[i])){
            counting[lpot + min_arr[lpot]]--;
            lpot++;
        }
        if (lpot == (r + 1))
            break;
        if (lpot > rpot)
            continue;
        (*count) += counting[i + max_arr[i]];
    }
    return;
}


void rmax_lmin(int l, int m, int r, int max_arr[], int min_arr[], long long *count){
    int lpot = m, rpot = m;
    for (int i = m + 1; i <= r; i++)
        counting[i - max_arr[i] + MAXN] = 0;
    for (int i = m + 1; i <= r; i++){
        while ((lpot != -1) && (max_arr[lpot] < max_arr[i])){
            counting[lpot - min_arr[lpot] + MAXN]++;
            lpot--;
        }
        while ((rpot != -1) && (min_arr[rpot] > min_arr[i])){
            counting[rpot - min_arr[rpot] + MAXN]--;
            rpot--;
        }
        if (rpot == -1)
            break;
        if (lpot > rpot)
            continue;
        (*count) += counting[i - max_arr[i] + MAXN];
    }

}

void Divide(int arr[], int l, int r, int N, long long *count){
    if (l >= r){
        (*count)++;
        return;
    }
    int m = (l + r) / 2;
    Divide(arr, l, m, N, count);
    Divide(arr, m + 1, r, N, count);
    int len1 = m - l + 1;
    int len2 = r - m;
    int max_arr[r - l + 1], min_arr[r - l + 1];
    max_arr[len1 - 1] = min_arr[len1 - 1] = arr[m];
    max_arr[len1] = min_arr[len1] = arr[m + 1];
    for (int i = 1; i < len1; i++){
        if (arr[m - i] > max_arr[len1 - i])
            max_arr[len1 - 1 - i] = arr[m - i];
        else
            max_arr[len1 - 1 - i] = max_arr[len1 - i];
        if (arr[m - i] < min_arr[len1 - i])
            min_arr[len1 - 1 - i] = arr[m - i];
        else
            min_arr[len1 - 1 - i] = min_arr[len1 - i];
    }
    for (int i = 1; i < len2; i++){
        if (arr[m + 1 + i] > max_arr[len1 + i - 1])
            max_arr[len1 + i] = arr[m + 1 + i];
        else
            max_arr[len1 + i] = max_arr[len1 + i - 1];
        if (arr[m + 1 + i] < min_arr[len1 + i - 1])
            min_arr[len1 + i] = arr[m + 1 + i];
        else
            min_arr[len1 + i] = min_arr[len1 + i - 1];
    }
    lmax_lmin(0, len1 - 1, len1 + len2 - 1, max_arr, min_arr, count);//1
    lmax_rmin(0, len1 - 1, len1 + len2 - 1, max_arr, min_arr, count);//3
    rmax_lmin(0, len1 - 1, len1 + len2 - 1, max_arr, min_arr, count);//12
    rmax_rmin(0, len1 - 1, len1 + len2 - 1, max_arr, min_arr, count);//2
    return;
}

int main(void){
    int N;
    long long count = 0;
    scanf("%d", &N);
    int arr[N];
    for (int i = 0; i < N; i++)
        scanf("%d", &arr[i]);
    Divide(arr, 0, N - 1, N, &count);
    printf("%lld\n", count);
    return 0;
}

//discuss with ³\¥@¾§
