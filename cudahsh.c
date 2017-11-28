#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stringgen.h"

#define HASH_LENGTH 513
#define KEY_LENGTH 4 //number of characterse, not counting null (extra space is allocated for null)

typedef enum {false, true} bool;

//used to hash a test key - the vast majority of the hashes won't match the one we're looking for.
void getHash(char* hash, const char* src) {
    //get the hash using whichever algorithm we choose
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

    char* buf = calloc(HASH_LENGTH, sizeof(char));

    for(int i; fgets(buf, HASH_LENGTH, input); i++) {
    	char* key = calloc(KEY_LENGTH + 2, sizeof(char));

    	//brute force the hash from buf and print the result to a line in output
    	charlist_t* sequence;
	    sequence = new_charlist_element();

	    while(strlen(key) <= KEY_LENGTH)
	    {
	    	sprint_charlist(key, sequence);
	    	printf("%s\n", key);
	        next(sequence);
	    }

	    free_charlist(sequence);

    	free(key);
    }
    
    fclose(input);
    fclose(output);
    free(buf);
    return 0;
}
