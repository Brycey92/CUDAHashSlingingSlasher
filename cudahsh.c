#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

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
    if(argc == 1) {
        printf("Usage: cudahsh.out <input file> <output file>\n");
        printf("Use a CUDA compatible GPU to crack the hashes in the input file and output the keys to the output file.\n");
        printf("Uses brute force and only works on keys made from letters and numbers.\n");
    }
    return 0;
}
