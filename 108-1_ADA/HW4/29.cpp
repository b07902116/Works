#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <cstring>
#include "hex.h"

using namespace std;
typedef pair<int, int> pii;

const int maxn = 11;

int board[maxn][maxn];

int n;
int up, down;


void init(int _n) {
    n = _n;
    memset(board, 0, sizeof board);
}

pii decide_3(pii p){
    pii put;
    if (p == make_pair(-1, -1)){
        put = make_pair(1, 1);
        board[put.first][put.second] = 1;
        return put;
    }
    board[p.first][p.second] = 2;
    if (p == make_pair(0, 1)){
        put = make_pair(0, 2);
    }
    else if (p == make_pair(0, 2)){
        put = make_pair(0, 1);
    }
    else if (p == make_pair(2, 0)){
        put = make_pair(2, 1);
    }
    else if (p == make_pair(2, 1)){
        put = make_pair(2, 0);
    }
    else{
        for (int i = 0; i < n; i++){
            for (int j = 0; j < n; j++){
                if (board[i][j] == 0){
                    put = make_pair(i, j);
                    i = n;
                    j = n;
                }
            }
        }
    }
    board[put.first][put.second] = 1;
    return put;
}

pii decide_4(pii p){
    pii put;
    if (p == make_pair(-1, -1)){
        put = make_pair(1, 2);
        board[put.first][put.second] = 1;
        return put;
    }
    board[p.first][p.second] = 2;
    if (p == make_pair(0, 2)){
        put = make_pair(0, 3);
    }
    else if (p == make_pair(0, 3)){
        put = make_pair(0, 2);
    }
    else if (p == make_pair(2, 1)){
        put = make_pair(2, 2);
    }
    else if (p == make_pair(2, 2)){
        put = make_pair(2, 1);
    }
    else{
        for (int i = n - 1; i >= 0; i--){
            for (int j = 0; j < n; j++){
                if (board[i][j] == 0){
                    put = make_pair(i, j);
                    i = -1;
                    j = n;
                }
            }
        }
    }
    board[put.first][put.second] = 1;
    return put;
}


pii any(void){
    for (int i = n - 1; i >= 0; i--){
        for (int j = 0; j < n; j++){
            if (board[i][j] == 0){
                board[i][j] = 1;
                return make_pair(i, j);
            }
        }
    }
}

pii down_5(pii p){
    pii put = make_pair(-1, -1);
    if (down == 0){
        if (p == make_pair(3, 2) || p == make_pair(4, 2)){
            put = make_pair(3, 1);
            down = 1;
        }
        else if (p == make_pair(3, 1) || p == make_pair(4, 0)){
            put = make_pair(3, 2);
            down = 2;
        }
        else if (p == make_pair(4, 1)){
            put = make_pair(3, 3);
            down = 3;
        }
    }
    else if (down == 1){
        if (p == make_pair(4, 0)){
            put = make_pair(4, 1);
            down = 5;
        }
        else if (p == make_pair(4, 1)){
            put = make_pair(4, 0);
            down = 5;
        }
    }
    else if (down == 2){
        if (p == make_pair(4, 2)){
            put = make_pair(4, 1);
            down = 5;
        }
        else if (p == make_pair(4, 1)){
            put = make_pair(4, 2);
            down = 5;
        }
    }
    else if (down == 3 || down == 4){
        if (p == make_pair(2, 3)){
            put = make_pair(3, 2);
            down++;
        }
        else if (p == make_pair(3, 2)){
            put = make_pair(2, 3);
            down++;
        }
        else if (p == make_pair(4, 2)){
            put = make_pair(4, 3);
            down++;
        }
        else if (p == make_pair(4, 3)){
            put = make_pair(4, 2);
            down++;
        }
    }
    return put;
}


pii decide_5(pii p){
    pii put = make_pair(-1, -1);
    if (p == make_pair(-1, -1)){
        put = make_pair(2, 2);
        up = 0;
        down = 0;
        board[put.first][put.second] = 1;
        return put;
    }
    board[p.first][p.second] = 2;
    put = down_5(p);    
    if (up == 0){
        if (put == make_pair(-1, -1)){
            if (p == make_pair(1, 3) || p == make_pair(0, 4)){
                put = make_pair(1, 2);
                up = 1;
            }
            else if (p == make_pair(1, 2) || p == make_pair(0, 2)){
                put = make_pair(1, 3);
                up = 2;
            }
            else if (p == make_pair(0, 3)){
                put = make_pair(1, 1);
                up = 3;
            }
            else{
                put = make_pair(1, 2);
                up = 1;
            }
        }
    }
    else if (up == 1){
        if (put == make_pair(-1, -1)){
            if (p == make_pair(0, 2)){
                put = make_pair(0, 3);
                up = 5;
            }
            else if (p == make_pair(0, 3)){
                put = make_pair(0, 2);
                up = 5;
            }
            else{
                put = any();
            }
        }
    }
    else if (up == 2){
        if (put == make_pair(-1, -1)){
            if (p == make_pair(0, 3)){
                put = make_pair(0, 4);
                up = 5;
            }
            else if (p == make_pair(0, 4)){
                put = make_pair(0, 3);
                up = 5;
            }
            else{
                put = any();
            }
        }
    }
    else if (up == 3 || up == 4){
        if (put == make_pair(-1, -1)){
            if (p == make_pair(1, 2)){
                put = make_pair(2, 1);
                up++;
            }
            else if (p == make_pair(2, 1)){
                put = make_pair(1, 2);
                up++;
            }
            else if (p == make_pair(0, 1)){
                put = make_pair(0, 2);
                up++;
            }
            else if (p == make_pair(0, 2)){
                put = make_pair(0, 1);
                up++;
            }
            else{
                put = any();
            }
        }
    }
    else{
        if (put == make_pair(-1, -1))
            put = any();
    }
    board[put.first][put.second] = 1;
    return put;
}


pii head, tail;

pii decide(pii p) {
    if (n == 3){
        return decide_3(p);
    }
    else if (n == 4){
        return decide_4(p);
    }
    else if (n == 5){
        return decide_5(p);
    }
    //n == 8
    pii put;
    if (p == make_pair(-1, -1)){
        put = make_pair(3, 4);
        head = put;
        tail = put;
        board[put.first][put.second] = 1;
        return put;
    }
    board[p.first][p.second] = 2;
    pii head_left = make_pair(head.first - 1, head.second), head_right = make_pair(head.first - 1, head.second + 1);
    pii tail_left = make_pair(tail.first + 1, tail.second - 1), tail_right = make_pair(tail.first + 1, tail.second);
    int headend = 0, tailend = 0;
    if (tail.first == 7)
        tailend = 1;
    if (head.first == 0)
        headend = 1;

    if (board[head_left.first][head_left.second] == 2){
        if (board[head_right.first][head_right.second] == 2){
            head_left.first += 1;
            head_left.second -= 1;
        }
        else{
            head_left = head_right;
        }
    }
    if (board[head_right.first][head_right.second] == 2){
        if (board[head_left.first][head_left.second] == 2){
            head_right.first += 1;
        }
        else{
            head_right = head_left;
        }
    }
    if (board[tail_left.first][tail_left.second] == 2){
        if (board[tail_right.first][tail_right.second] == 2){
            tail_left.first -= 1;
        }
        else{
            tail_left = tail_right;
        }
    }
    if (board[tail_right.first][tail_right.second] == 2){
        if (board[tail_left.first][tail_left.second] == 2){
            tail_right.first -= 1;
            tail_right.second += 1;
        }
        else{
            tail_right = tail_left;
        }
    }

    if (p == head_left){
        put = head_right;
        head = head_right;
    }
    else if (p == head_right){
        put = head_left;
        head = head_left;
    }
    else if (p == tail_left){
        put = tail_right;
        tail = tail_right;
    }
    else if (p == tail_right){
        put = tail_left;
        tail = tail_left;
    }
    else{//other position
        if (p.first < head.first){//upper than head
            if (p.second > head.second){//rightside
                put = head_left;
                head = head_left;
            }
            else{
                put = head_right;
                head = head_right;
            }
        }
        else if (p.first > tail.first){
            if (p.second > tail.second){//rightside
                put = tail_left;
                tail = tail_left;
            }
            else{
                put = tail_right;
                tail = tail_right;
            }
        }
        else{
            if (!tailend){
                put = tail_right;
                tail = tail_right;
            }
            else if (!headend){
                put = head_left;
                head = head_left;
            }
        }
    }
    board[put.first][put.second] = 1;
    return put;
}
