#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>
#include "stringgen.h"

#define HASH_LENGTH 64 //number of characters, not counting null (extra space is allocated for null)
#define KEY_LENGTH 4 //number of characters, not counting null (extra space is allocated for null)

typedef enum {false, true} bool;

char* removeNewline(char* str) {
   if(str != NULL && !strcmp(str + (strlen(str) - 1), "\n")) {
      str[(strlen(str) - 1)] = 0;
   }
   
   return str;
}

//used to hash a test key - the vast majority of the hashes won't match the one we're looking for.
__global__ void getHash(char* hash, const char* src) {
    //get the hash using whichever algorithm we choose
    memcpy(hash, src, HASH_LENGTH + 1);
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

    char* buf = (char*) calloc(HASH_LENGTH + 1, sizeof(char));
    char* key = (char*) calloc(KEY_LENGTH + 2, sizeof(char));
    char* hash = (char*) calloc(HASH_LENGTH + 1, sizeof(char));

    for(int i; fgets(buf, HASH_LENGTH, input); i++) {
    	removeNewline(buf);

    	//brute force the hash from buf and print the result to a line in output
    	charlist_t* sequence;
	    sequence = new_charlist_element();

	    bool stopAll = false;

	    while(!stopAll && strlen(key) <= KEY_LENGTH)
	    {
	    	//generate an array of keys to hash
	    	while(strlen(key) <= KEY_LENGTH && )
		    {
		    	sprint_charlist(key, sequence);
		    	//printf("%s\n", key);
		    	getHash(hash, key);
		    	if(!strcmp(hash, buf)) {
		    		printf("%s\n", hash);
		    		stopAll = true;
		    	}

		        next(sequence);
		    }

		    //hash the keys and check the hashes
	    	sprint_charlist(key, sequence);
	    	//printf("%s\n", key);
	    	getHash<<1, 1>>(hash, key);
	    	if(!strcmp(hash, buf)) {
	    		printf("%s\n", hash);
	    		stopAll = true;
	    	}

	        next(sequence);
	    }

	    free_charlist(sequence);
    }
    
    fclose(input);
    fclose(output);
    free(buf);
    free(key);
    free(hash);
    return 0;
}
