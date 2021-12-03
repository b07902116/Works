#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>


#define DATANUM 60000
#define TestDataNum 10000
#define DIMENSION 784
#define KINDS 10


unsigned char X_train_data[DATANUM][DIMENSION];
unsigned char X_test_data[TestDataNum][DIMENSION];
char Y_train_data[DATANUM][KINDS];
char Y_test_data[TestDataNum];
double predict[DATANUM][KINDS];
double weight[DIMENSION][KINDS] = {0};
double gradient[DIMENSION][KINDS];
int thread_num;



void read_train_data(char *X_train_name, char *Y_train_name){
	FILE *fpX, *fpY;
	fpX = fopen(X_train_name, "rb");
	fpY = fopen(Y_train_name, "rb");
	char y;
	for (int i = 0; i < DATANUM; i++){
		for (int j = 0; j < DIMENSION; j++){
			fread(&(X_train_data[i][j]), 1, 1, fpX);
		}
		fread(&y, 1, 1, fpY);
		for (int j = 0; j < KINDS; j++){
			Y_train_data[i][j] = y == j? 1 : 0;
		}
	}
	fclose(fpX);
	fclose(fpY);
	return;
}

void read_test_data(char *X_test_name){
	FILE *fpX;
	fpX = fopen(X_test_name, "rb");
	for (int i = 0; i < TestDataNum; i++){
		for (int j = 0; j < DIMENSION; j++){
			fread(&(X_test_data[i][j]), 1, 1, fpX);
		}
	}
	fclose(fpX);
	FILE *fpY;
	fpY = fopen("y_test", "rb");
	for (int i = 0; i < TestDataNum; i++){
		fread(&(Y_test_data[i]), 1, 1, fpY);
	}
	fclose(fpY);
	return;
}


/*void get_predict(void){
	double tmp = 0;
	for (int i = 0; i < DATANUM; i++){
		for (int j = 0; j < KINDS; j++){
			tmp = 0;
			for (int k = 0; k < DIMENSION; k++){
				tmp += X_train_data[i][k] * weight[k][j] / DATANUM;
			}
			predict[i][j] = tmp;
		}
	}
	for (int i = 0; i < DATANUM; i++){
		tmp = 0;
		for (int j = 0; j < KINDS; j++){
			tmp += exp(predict[i][j]);
		}
		for (int j = 0; j < KINDS; j++){
			predict[i][j] = exp(predict[i][j]) / tmp;
		}
	}
	return;
}*/


void get_gradient(void){
	double tmp = 0;
	for (int i = 0; i < DIMENSION; i++){
		for (int j = 0; j < KINDS; j++){
			tmp = 0;
			for (int k = 0; k < DATANUM; k++){
				tmp += X_train_data[k][i] * (predict[k][j] - Y_train_data[k][j]) / DATANUM;
			}
			gradient[i][j] = tmp;
		}
	}
	return;
}


void update(int time){
	double lr = (double) 1 / time;
	for (int i = 0; i < DIMENSION; i++){
		for (int j = 0; j < KINDS; j++){
			weight[i][j] = weight[i][j] - lr * gradient[i][j];
		}
	}
	return;
}


void test(void){
	int count = 0, ans;
	double max = 0, tmp = 0;
	for (int i = 0; i < TestDataNum; i++){
		max = 0;
		for (int j = 0; j < KINDS; j++){
			tmp = 0;
			for (int k = 0; k < DIMENSION; k++){
				tmp += X_test_data[i][k] * weight[k][j] / TestDataNum;
			}
			if (tmp > max){
				max = tmp;
				ans = j;
			}
		}
		if (ans != Y_test_data[i])
			count++;
		printf("%d,%d\n", i, ans);
	}
	//fprintf(stderr, "wrong = %d\n", count);
	return;
}


void *thread_func(void *arg){
	int index = *(int *) arg;
	int from = index * DATANUM / thread_num;
	int to = (index + 1) * DATANUM / thread_num;
	double tmp = 0;
	for (int i = from; i < to; i++){
		for (int j = 0; j < KINDS; j++){
			tmp = 0;
			for (int k = 0; k < DIMENSION; k++){
				tmp += X_train_data[i][k] * weight[k][j] / DATANUM;
			}
			predict[i][j] = tmp;
		}
	}
	for (int i = from; i < to; i++){
		tmp = 0;
		for (int j = 0; j < KINDS; j++){
			tmp += exp(predict[i][j]);
		}
		for (int j = 0; j < KINDS; j++){
			predict[i][j] = exp(predict[i][j]) / tmp;
		}
	}
	pthread_exit((void *) 0);
}


int main(int argc, char *argv[]){
	time_t start, end;
	time(&start);
	int fd = open("result.csv", O_WRONLY | O_CREAT | O_TRUNC);
	dup2(fd, STDOUT_FILENO);
	printf("id,label\n");
	read_train_data(argv[1], argv[2]);
	read_test_data(argv[3]);
	thread_num = atoi(argv[4]);
	pthread_t tid[thread_num];
	int arg[thread_num];
	void *retval;
	for (int i = 0; i < thread_num; i++){
		arg[i] = i;
	}

	for (int i = 0; i < 4; i++){
		//get_predict();//1 and 2

		for (int j = 0; j < thread_num; j++){
			pthread_create(&(tid[j]), NULL, thread_func, &(arg[j]));
		}
		for (int j = 0; j < thread_num; j++){
			pthread_join(tid[j], &retval);
		}
		get_gradient();//4
		update(i + 1);//3
	}
	time(&end);
	test();
	//fprintf(stderr, "time = %d\n", end - start);
	return 0;
}