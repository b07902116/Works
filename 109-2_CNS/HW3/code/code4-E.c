long long int calculate() {
    short int stop = 0;
    long long int result[2] = {1, 1};
    _Bool now = 0;
    printf("stop: ");
    scanf("%hd", &stop);
    if (stop == 0 || stop == 1) {
        return 1;
    }

    for (short int i = 1; i != stop; ++i) {
        result[now^1] += result[now];
        now ^= 1;
    }
    return result[now];
}