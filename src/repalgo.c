#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int n;

int * pClass;
int * pStart;
int * pGap;
int * pEnd;
int * pSize;

int i, j;


void printMemory(){
	for(i = 0; i < n; ++i){
		printf("pClass[%d] = %d \tpStart[%d] = %d \tpEnd[%d] = %d \tpSize[%d] = %d \tpGap[%d] = %d\n",i,pClass[i],i,pStart[i],i,pEnd[i],i,pSize[i],i,pGap[i]);	
	}
}

void rep(char str[]){
	char curr;
	int prevA = 0;
	int prevB = 1;
	for(i = 1; i < n; ++i){
		curr = str[i];
		switch(curr){
			case 'a':
				pGap[i] = i - prevA; 
				prevA = i;
				pClass[i] = 0;
				if(pStart[i] == -1){
					pStart[0] = i;
				}
				pEnd[0] = i;
				pSize[0] += 1;	
				break;
			case 'b':
			    pGap[i] = i - prevB; 
				prevB = i;	
				pClass[i] = 1;
				if(pStart[1] == -1){
					pStart[1] = i;
				}
				pEnd[1] = i;
				pSize[1] += 1;
				break;
			default:
				//to handle
				break;
		}		
	}
	

}

void init(char str[]){	
	n = strlen(str);
	pClass = (int *) malloc(sizeof (int) * n);
	pStart = pClass + n;
	pEnd = pStart + n;
	pSize = pEnd + n;
	pGap = pSize + n;
	for(i = 0; i < n; ++i){
		pClass[i] = -1;
		pStart[i] = -1;
		pEnd[i] = -1;
	}
	
}

int main (int argc, char * argv[])
{
	init(argv[1]);
	rep(argv[1]);
	printMemory();
	
}