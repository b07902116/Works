#include <stdio.h>
#include <string.h>


int min(int a, int b, int c){
    int rt = a < b? a : b;
    rt = rt < c? rt : c;
    return rt;
}


int table[2048][2048] = {0};

int main(void){
    char str1[2048], str2[2048];
    scanf("%s%s", str1, str2);
    int len1 = strlen(str1);
    int len2 = strlen(str2);
    for (int i = 0; i <= len1; i++)
        table[0][i] = i;
    for (int j = 0; j <= len2; j++)
        table[j][0] = j;
    int left, up, left_up;
    for (int j = 1; j <= len2; j++){
        for (int i = 1; i <= len1; i++){
            left = table[j][i - 1] + 1;
            up = table[j - 1][i] + 1;
            if (str1[i - 1] == str2[j - 1])
                left_up = table[j - 1][i - 1] + 1;
            else
                left_up = table[j - 1][i - 1];
            table[j][i] = min(left, up, left_up);
        }
    }
    int ans = table[len2][0];
    for (int i = 0; i <= len1; i++)
        if (table[len2][i] < ans)
            ans = table[len2][i];
    for (int j = 0; j <= len2; j++)
        if (table[j][len1] < ans)
            ans = table[j][len1];
    printf("%d\n", ans);
    return 0;
}
