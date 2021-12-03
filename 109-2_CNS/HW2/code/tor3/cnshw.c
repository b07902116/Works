#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* #define ED25519_DLL */
#include "ed25519.h"

#include "ge.h"
#include "sc.h"


int main() {
    unsigned char public_key[32], private_key[64], seed[32];
	unsigned char CNSHW[4] = {19, 100, 123, 0}; 

    /* create a random seed, and a keypair out of that seed */
	while (1){
		ed25519_create_seed(seed);
	    ed25519_create_keypair(public_key, private_key, seed);
		if (public_key[0] == CNSHW[0]){
			if (public_key[1] == CNSHW[1]){
				if (public_key[2] == CNSHW[2]){
					if (public_key[3] < 128){
						break;
					}
				}
			}
		}
 	} 

	printf("\npublic_key: ");
	for (int i = 0; i < 32; i++){
		printf("%02x", public_key[i]);
	}
	printf("\nprivate_key: ");
	for (int i = 0; i < 64; i++){
		printf("%02x", private_key[i]);
	}
	printf("\n");	



	return 0;
}
