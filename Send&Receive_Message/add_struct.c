#include <stdio.h>

char calcul_sum(char* data, int len){
	int i;
	unsigned char result = data[0];
	for(i = 1; i < len; i++){
		result ^= data[i];
		//printf("%c\n", data[i]);
	}
	return result;
}