#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[]){
	int player_id = atoi(argv[1]);
	int winner_id, money = player_id * 100;
	char buffer[256];
	sprintf(buffer, "%d %d\n\0", player_id, money);
	write(STDOUT_FILENO, buffer, strlen(buffer));
	fsync(STDOUT_FILENO);
	for (int i = 0; i < 9; i++){
		scanf("%d", &winner_id);
		sprintf(buffer, "%d %d\n\0", player_id, money);
		write(STDOUT_FILENO, buffer, strlen(buffer));
		fsync(STDOUT_FILENO);
	}
	_exit(0);
}