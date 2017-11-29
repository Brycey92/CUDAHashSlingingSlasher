#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "stringgen.h"

#include <crypto++/md5.h>
#include <crypto++/hex.h>

#include <omp.h>

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
//compile with
//g++ -o openMPCrack.out stringgen.c hashCrackopenMP.cpp -fopenmp -lcrypto++

using namespace std;
 
string hashString(const string source);

void get_walltime(double* wcTime) {
     struct timeval tp;
     gettimeofday(&tp, NULL);
     *wcTime = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}
 
int main( int argc, char** argv) {
	double startTime, stopTime;

	string input;
	string inputHash;

	cout << "Enter the string to hash with no spaces: ";
	cin >> input;
	inputHash = hashString(input);
	cout << "Hash to check against: " << inputHash << endl;	

	get_walltime(&startTime);


	int threads = omp_get_num_threads();
	
	//We "cheat" and set the maximum character size to 1 more than the input size.
	//In a real scenario we would not know this but we could just use variable sizes
	//or an aribtrarily long character string
	string inputStrings[threads];
		
	charlist_t* sequence = new_charlist_element();
	
	bool stopAll = false;

    while(!stopAll)
    {
    	for (int i = 0; i < threads; ++i) {
   			char* inputChars = new char [input.length() + 1];
	    	sprint_charlist(inputChars, sequence);
	    	inputStrings[i] = string(inputChars);   	
	        next(sequence);
    	}

	    #pragma omp parallel for
       	for (int i = 0; i < threads; ++i) {	
			string hash = hashString(inputStrings[omp_get_thread_num()]);
			
			if (hash.compare(inputHash) == 0) {
				cout << inputStrings[omp_get_thread_num()] << " is equal to the input string of " << input << endl;
				cout << "Hash1: " << inputHash << endl << "Hash2: " << hash << endl;
				stopAll = true;
			}
		}
    	
    }

    free_charlist(sequence);
	get_walltime(&stopTime);

	cout << "Total Time Taken: " << (stopTime - startTime) << endl;

	return 0;
}
 
string hashString(const string source)
{
	string hash = "";
	CryptoPP::MD5 md5;

	CryptoPP::StringSource(source, true, new CryptoPP::HashFilter(md5, new CryptoPP::HexEncoder(new CryptoPP::StringSink(hash))));
	return hash;
}
