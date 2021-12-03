#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <crypt.h>
#include <rpc/des_crypt.h>

int encry_and_check(unsigned char *block, unsigned char *key, unsigned char *LMhash){
	ecb_crypt(key, block, 8, DES_ENCRYPT);
	if (strncmp(block, LMhash, 8) == 0){
		ecb_crypt(key, block, 8, DES_DECRYPT);
		return 1;
	}
	ecb_crypt(key, block, 8, DES_DECRYPT);
	return 0;
}

void pwd_to_key(unsigned char *pwd, unsigned char *key){
	uint64_t keybit = 0;
	uint64_t pwdbit = 0;
	uint64_t tmp = 0;
	for (int i = 0; i < 7; i++){
		pwdbit += (((uint64_t) pwd[i]) << (8 * (6 - i)));
	}
	for (int i = 0; i < 8; i++){
		keybit <<= 8;
		tmp = (((pwdbit >> (7 * (7 - i))) & 127) << 1);
		keybit += tmp;
	}
	for (int i = 0; i < 8; i++){
		key[i] = (unsigned char) (keybit >> (8 * (7 - i)));
	}
	return;
}

int brute_search(unsigned char *block, unsigned char *LMhash, unsigned char *pwd, unsigned char *key, int count){
	if (count < 0){
		pwd[6] = '}';
		pwd_to_key(pwd, key);
		return encry_and_check(block, key, LMhash);
	}
	for (unsigned char i = 0; i < 128; i++){
		if (isprint(i) == 0 || islower(i) != 0)
			continue;
		pwd[count] = i;
		if (count == 5){
			printf("%c\n", i);
		}
		//printf("%d\n", count);
		//printf("%02x, %02x, %02x, %02x, %02x, %02x, %02x\n", pwd[0], pwd[1], pwd[2], pwd[3], pwd[4], pwd[5], pwd[6]);       
		if (brute_search(block, LMhash, pwd, key, count - 1)){
			printf("%02x, %02x, %02x, %02x, %02x, %02x, %02x\n", pwd[0], pwd[1], pwd[2], pwd[3], pwd[4], pwd[5], pwd[6]);       
			printf("find key: ");
			for (int i = 0; i < 7; i++)
				printf("%c", *(pwd + i));
			printf("\n");
			return 1;
		}
	}
	return 0;
}



int main(void){
	unsigned char *LMhash = "cc06a61efcb3394b8a3b0fdfa7972d03";
	unsigned char front[8], back[8];
	for (int i = 0; i < 8; i++){
		sscanf((LMhash + 2 * i), "%02hhx", &(front[i]));
		sscanf((LMhash + 2 * (8 + i)), "%02hhx", &(back[i]));
	}
	unsigned char pwd[7] = {0}, key[8] = {0};
	unsigned char block[8] = {'K', 'G', 'S', '!', '@', '#', '$', '%'};
	//brute_search(block, front, pwd, key, 6);
	printf("===\n");
	brute_search(block, back, pwd, key, 5);

	return 0;
}
