#include <stdio.h>
#include <stdlib.h>
#include "my_pthread.h"

void thread_run(){
int count = 0;
  //while(count < 10){
  while(1){
    printf("Thread Running\n", my_pthread_self());
	count++;
  }
	//getNumProcessesInTCB();

}

int main(){

	printf("in main\n");
	getNumProcessesInTCB();
  my_pthread_t thread;

  my_pthread_create(&thread, (void*) thread_run, (void*) NULL);

  while(1){
    printf("Main Thread Running\n");
  }

}
