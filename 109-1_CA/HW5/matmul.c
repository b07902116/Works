// See LICENSE for license details.

#include "dataset.h"
#include "util.h"
#include <stddef.h>

#pragma GCC optimize ("unroll-loops")

void matmul(const size_t coreid, const size_t ncores, const size_t lda,  const data_t A[], const data_t B[], data_t C[])
{
  size_t i, j, k;
  size_t block = lda / ncores;
  size_t start = block * coreid;
  
 	for (k = 0; k < lda / 4; k++){
		for (j = start; j < (start + block); j++){
			
			for (i = 0; i < lda; i++){
				data_t partial_sum = 0;
				partial_sum = A[j * lda + (k * 4 + 0)] * B[(k * 4 + 0) * lda + i] + A[j * lda + (k * 4 + 1)] * B[(k * 4 + 1) * lda + i]
							+ A[j * lda + (k * 4 + 2)] * B[(k * 4 + 2) * lda + i] + A[j * lda + (k * 4 + 3)] * B[(k * 4 + 3) * lda + i];
				C[j * lda + i] += partial_sum;
			}
		}
	}

}
