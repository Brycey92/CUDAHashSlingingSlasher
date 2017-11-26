#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_LENGTH

typedef enum {false, true} bool;

void getMD5(char* md5, const char* src) {
    MD5(src, strlen(src), md5);
}

void getSHA1(char* sha1, const char* src) {
	SHA1(src, strlen(src), sha1);
}

void getSHA256(char* sha256, const char* src) {
	SHA256(src, strlen(src), sha256);
}

int main(int argc, char** argv[]) {
    if(argc < 3) {
        printf("Usage: cudahsh.out <input file> <output file>\n");
        printf("Use a CUDA compatible GPU to crack the hashes in the input file and output the keys to the output file.\n");
        printf("Uses brute force and only works on keys made from letters and numbers.\n");
        return 1;
    }

    FILE* input = fopen(argv[1]);
    if(!input) {
    	printf("Error opening input file %s!\n", argv[1]);
    	return 1;
    }

    FILE* output = fopen(argv[2]);
    if(!output) {
    	printf("Error opening output file %s!\n", argv[2]);
    	fclose(input);
    	return 1;
    }

    char* buf = calloc(HASH_LENGTH, sizeof(char));

    for(int i; fgets(buf, HASH_LENGTH, input); i++) {
    	//brute force the hash from buf and print the result to a line in output
    }
    
    fclose(input);
    fclose(output);
    free(buf);
    return 0;
}
