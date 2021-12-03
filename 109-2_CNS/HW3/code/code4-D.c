void service(const char *email) {
    int choice;
    char buf[256] = {0};
    long long int result;
    while (1) {
        printf("Your choice: ");
        scanf("%s", buf);
        if (strlen(buf) == 1){
            choice = (int) buf[0] - '0';
        }
        if (choice == 1) {
            result = calculate();
        } else if (choice == 2) {
            printf("Sending result %lld to %s\n", result, email);
        } else if (choice == 3) {
            return;
        } else {
            printf("Please input 1 or 2 or 3\n");
        }
    }
}
