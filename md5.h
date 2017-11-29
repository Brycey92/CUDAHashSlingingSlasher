#ifdef __cplusplus
extern "C"
#endif
char *md5_unpad(char *input);

#ifdef __cplusplus
extern "C"
#endif
char *md5_pad(char *input);

#ifdef __cplusplus
extern "C"
#endif
void md5_calculate(int blocks, int threads, char* hash, char* keyArr, char* key, bool* hash_found);
