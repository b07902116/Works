#include <stdio.h>
#include <stdlib.h>


typedef struct node{
    long long max, sum, prefix, suffix;
    struct node *left;
    struct node *right;
}Node;

long long max(long long a, long long b, long long c){
    long long tmp = a;
    if (tmp < b)
        tmp = b;
    if (tmp < c)
        tmp = c;
    return tmp;
}


/*Node *new_node(void){
    Node *new = (Node *) malloc(sizeof(Node));
    new->right = NULL;
    new->left = NULL;
    new->max = 0;
    new->sum = 0;
    new->prefix = 0;
    new->suffix = 0;
    return new;
}*/


Node *construct_tree(int arr[], int l, int r, int N, Node *root){
    if (r == l){
        root->left = NULL;
        root->right = NULL;
        root->sum = arr[l];
        root->max = arr[l];
        root->prefix = arr[l];
        root->suffix = arr[l];
        return root;
    }
    int m = (l + r) / 2;
    root->left = (Node *) malloc(sizeof(Node));
    root->right = (Node *) malloc(sizeof(Node));
    root->left = construct_tree(arr, l, m, N, root->left);
    root->right = construct_tree(arr, m + 1, r, N, root->right);

    root->sum = root->left->sum + root->right->sum;
    root->prefix = max(root->left->prefix, root->left->sum + root->right->prefix, 0);
    root->suffix = max(root->right->suffix, root->right->sum + root->left->suffix, 0);
    long long left = max(root->prefix, root->left->max, 0);
    long long right = max(root->suffix, root->right->max, 0);
    root->max = max(left, right, root->left->suffix + root->right->prefix);
    return root;
}

Node *update(Node *root, int pos, int val, int l, int r){
    if (r <= l){
        root->max = val;
        root->sum = val;
        root->prefix = val;
        root->suffix = val;
        return root;
    }
    int m = (l + r) / 2;
    if (pos <= m)
        root->left = update(root->left, pos, val, l, m);
    else
        root->right = update(root->right, pos, val, m + 1, r);

    root->sum = root->left->sum + root->right->sum;
    root->prefix = max(root->left->prefix, root->left->sum + root->right->prefix, 0);
    root->suffix = max(root->right->suffix, root->right->sum + root->left->suffix, 0);
    long long left = max(root->prefix, root->left->max, 0);
    long long right = max(root->suffix, root->right->max, 0);
    root->max = max(left, right, root->left->suffix + root->right->prefix);
    return root;
}



int main(void){
    int N, Q;
    scanf("%d%d", &N, &Q);
    int arr[N];
    for (int i = 0; i < N; i++)
        scanf("%d", &arr[i]);
    Node *root = (Node *) malloc(sizeof(Node));
    root = construct_tree(arr, 0, N - 1, N, root);
    printf("%lld\n", root->max);
    int pos, val;
    for (int i = 0; i < Q; i++){
        scanf("%d%d", &pos, &val);
        root = update(root, pos - 1, val, 0, N - 1);
        printf("%lld\n", root->max);
    }
    return 0;
}
