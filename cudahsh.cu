#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <cuda.h>
#include "stringgen.h"
#include "cudahsh.h"
#include "md5.h"

static struct timeval timer() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp;
}

static double toSeconds(struct timeval tp) {
   return ((double) (tp.tv_sec) + 1e-6 * tp.tv_usec);
}

char* removeNewline(char* str) {
   if(str != NULL && !strcmp(str + (strlen(str) - 1), "\n")) {
      str[(strlen(str) - 1)] = 0;
   }
   
   return str;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Usage: cudahsh.out <input file> <output file>\n");
        printf("Use a CUDA compatible GPU to crack the hashes in the input file and output the keys to the output file.\n");
        printf("Uses brute force and only works on keys made from letters and numbers.\n");
        return 1;
    }

    FILE* input = fopen(argv[1], "r");
    if(!input) {
    	printf("Error opening input file %s!\n", argv[1]);
    	return 1;
    }

    FILE* output = fopen(argv[2], "w");
    if(!output) {
    	printf("Error opening output file %s!\n", argv[2]);
    	fclose(input);
    	return 1;
    }
    
    double elt = 0.0;
    struct timeval start;
    start = timer();
	
    char* key = (char*) calloc(KEY_LENGTH + 2, sizeof(char));
    char* hash = (char*) calloc(HASH_LENGTH + 1, sizeof(char));
    char* keyArr = (char*) calloc(N * (KEY_LENGTH + 2), sizeof(char));
    char* gpuKey;
    cudaMalloc(&gpuKey, (KEY_LENGTH + 2) * sizeof(char));
	char* gpuHash;
    cudaMalloc(&gpuHash, (HASH_LENGTH + 1) * sizeof(char));
    char* gpuKeyArr;
    cudaMalloc(&gpuKeyArr, N * (KEY_LENGTH + 2) * sizeof(char));
    bool stopAll;
    bool* gpuStopAll;
    cudaMalloc(&gpuStopAll, sizeof(bool));
    
    int blocksPerGrid = ceil(N / 512.0);
    int threadsPerBlock = N / blocksPerGrid;
    //printf("%d\n", threadsPerBlock);

    for(int i; fgets(hash, HASH_LENGTH, input); i++) {
    	removeNewline(hash);

    	//brute force the hash from buf and print the result to a line in output
    	charlist_t* sequence;
	    sequence = new_charlist_element();

	    stopAll = false;
	    cudaMemset(gpuStopAll, false, sizeof(bool));

	    while(!stopAll && strlen(key) <= KEY_LENGTH)
	    {
	    	//generate an array of keys to hash
	    	for(int curKey = 0; curKey < N && strlen(key) <= KEY_LENGTH; curKey++)
		    {
		    	//printf("%d\n", curKey);
		    	sprint_charlist(key, sequence);
		    	memcpy(&keyArr[curKey * (KEY_LENGTH + 2)], md5_pad(key), (KEY_LENGTH + 2) * sizeof(char));
		    	//printf("%s %d\n", &keyArr[curKey * (KEY_LENGTH + 2)], curKey);
		    	
		        next(sequence);
		    }
		    
		    cudaMemcpy(gpuKeyArr, keyArr, N * (KEY_LENGTH + 2) * sizeof(char), cudaMemcpyHostToDevice);
   		    cudaMemcpy(gpuHash, hash, N * (KEY_LENGTH + 2) * sizeof(char), cudaMemcpyHostToDevice);

		    //hash the keys and check the hashes
	    	//getHash<<<blocksPerGrid, threadsPerBlock>>>(hash, key);
			md5_calculate(blocksPerGrid, threadsPerBlock, gpuHash, gpuKeyArr, gpuKey, gpuStopAll);
	    	
	    	cudaMemcpy(&stopAll, gpuStopAll, sizeof(bool), cudaMemcpyDeviceToHost);
	    }

		cudaMemcpy(key, gpuKey, (KEY_LENGTH + 2) * sizeof(char), cudaMemcpyDeviceToHost);
		memcpy(key, md5_unpad(key), KEY_LENGTH + 2);
		key[KEY_LENGTH] = 0;
		free_charlist(sequence);
		
		struct timeval end = timer();
        elt = toSeconds(end) - toSeconds(start);
		
		fprintf(output, "%s\n", key);
		printf("Time taken: %3.3lf s.\n", elt);

	    
    }
    
    fclose(input);
    fclose(output);
    free(key);
    free(hash);
    cudaFree(gpuKeyArr);
    return 0;
}
