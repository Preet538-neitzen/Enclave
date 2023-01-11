#include <stdio.h>
#include <stdlib.h>
// #include <stdio.h>
// #include <stdlib.h>
#include <time.h>
#include "guessing_game_t.h"

int guessVal=-1;
int maxiVal = -1; 
int counter = 0; 

int seed(){
	return 11*time(NULL)+7; 
}

int randInt(){
	srand(seed()); 
	return rand()%256; 
}

void findMaxi(int res){
	 maxiVal = res; 
	 return; 
}

void checkStatus(int a, int* res){
	counter++; 
	if (guessVal==-1){
		guessVal=randInt();

	}
	if (maxiVal<counter){
		 fprintf(stderr, "Locked %d(max value)<%d(counter)\n", maxiVal, counter);
		oe_abort(); 
	}
	*res = a==guessVal;
	return; 
}