#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int n;

int * pClass;
int * pStart;
int * pEnd;
int * pSize;

int i, j;


void printMemory(){
	for(i = 0; i < n; ++i){
		printf("pClass[%d] = %d \tpStart[%d] = %d \tpEnd[%d] = %d \tpSize[%d] = %d\n",i,pClass[i],i,pStart[i],i,pEnd[i],i,pSize[i]);	
	}
}


void firstRun(char str[]){
	char lastC = str[0];
	for(i = 0; i < n; ++i){	
		if(str[i] == lastC){
			lastC = str[i];
			pClass[i] = 0;
			if(pStart[0] == -1){
				pStart[0] = i;
			}
			pEnd[0] = i;
			pSize[0] += 1;
			
		}else {
			pClass[i] = 1;
			if(pStart[1] == -1){
				pStart[1] = i;
			}
			pEnd[1] = i;
			pSize[1] += 1;
		}
		
	}
	
	for(i = 0; i < n; ++i){
	}
	

}

void init(char str[]){	
	n = strlen(str);
	pClass = (int *) malloc(sizeof (int) * n);
	pStart = pClass + n;
	pEnd = pStart + n;
	pSize = pEnd + n;
	for(i = 0; i < n; ++i){
		pClass[i] = -1;
		pStart[i] = -1;
		pEnd[i] = -1;
	}
	
}

int main (int argc, char * argv[])
{
	printf("the string is: %s\n", argv[1]);
	init(argv[1]);
	firstRun(argv[1]);
	printMemory();
	
}