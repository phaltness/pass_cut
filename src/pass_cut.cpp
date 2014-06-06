//
//  main.c
//  wav lowpass IIR filter with fixed ariphmetic;
//  sampling F=44100 Hz; -3dB F=300 Hz;
//

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;
using std::string;
using std::fstream;

#define PASS	34

int main(int argc, const char * argv[]){

		char *inFilePath = (char *)"passwords.txt";
		char *outFilePath = (char *)"pass_34.txt";
		long long i = 0;
		int count = 0;
		char *in, *out, *list;
		char buff[16] = {0, 0};
		size_t szRead, szDataBuffer = 0x100;
		char *dataBuffer = (char *)malloc(szDataBuffer);
		FILE *inFile = fopen(inFilePath, "r+");

		cout << "start" << endl;
		if (inFile){
			cout << "file read opened" << endl;
			FILE *outFile = fopen(outFilePath, "w+");
			if (outFile){

				while (feof(inFile) == 0) {
					in = dataBuffer;												//set in to begin of buff
					out = dataBuffer + count;										//set out to last char in buff
//Read from file to buff
					szRead = fread(out, 1, (szDataBuffer - count), inFile);			//fill buff from out iterator
					out += szRead;													//move out to end of chars in buff
//					cout << i << "\t" << (out-in) << endl;
					i += szRead;													//total iterator

					count = 0;														//reset count of chars in word
//parse buff
 					while (in <= out) {												//until end of filled buff
//move last chars to begin of buff if it < 8 chars
						if (in == out){
							count = 0;
							break;
						}else if (out-in <= PASS) {									//check that leave <= 8 chars in buff
							while (in + count < out) {								//until end of chars in buff
								*(dataBuffer + count) = *(in + count);				//move last chars to begin of buff
								count++;
							}
							break;
						}else if (*(in+count) == (0x0a)){							//search of \n
							if (count == PASS) {										//check for pass length=8
								fwrite(in, 1, PASS+1, outFile);							//write last pass in outFile
							}
							in += count + 1;										//move in to last position of read / clear last pass
							count = 0;												//reset count of chars in word
 						}else count++;												//increment count to search pass 8 chars
 					}

				}
				free(dataBuffer);
				fclose(outFile);
			}
			fclose(inFile);
		}else cout << "stoped" << endl;
}
