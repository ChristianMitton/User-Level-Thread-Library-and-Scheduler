#include "my_pthread.h"
#define STACK_SIZE SIGSTKSZ

/* Scheduler State */
 // Fill in Here //
my_pthread_tcb *blockHead = NULL;

//---------------------------------------------------------------------------------------
//-----------------------------------schedule--------------------------------------------
//---------------------------------------------------------------------------------------

/* Scheduler Function
 * Pick the next runnable thread and swap contexts to start executing
 */
void schedule(int signum){

  // Implement Here

}

//---------------------------------------------------------------------------------------
//-------------------------------my_pthread_create()---------------------------------------
//---------------------------------------------------------------------------------------

/* Create a new TCB for a new thread execution context and add it to the queue
 * of runnable threads. If this is the first time a thread is created, also
 * create a TCB for the main thread as well as initalize any scheduler state.
 */
void my_pthread_create(my_pthread_t *thread, void*(*function)(void*), void *arg){
  // Implement Here

	my_pthread_tcb *newEntry = malloc(sizeof(my_pthread_tcb));
	//newEntry->tid = id of new thread
	newEntry->status = RUNNABLE;
	newEntry->context.uc_link = NULL;
	newEntry->context.uc_stack.ss_sp = malloc(SIGSTKSZ);
	newEntry->context.uc_stack.ss_size = SIGSTKSZ;
	//newEntry->context.uc_stack.ss_flags = 0;
	//newEntry->next = NULL;
	
	getcontext(&newEntry->context);
	/*if (getcontext(&newEntry->context) < 0) {
		perror("getcontext");
		exit(1);
	}*/

	//makecontext(&newEntry->context,(void *)&function, 0);
	makecontext(&newEntry->context,(void (*) (void))function, 0);
	printf("1\n");
	setcontext(&newEntry->context);
	printf("2\n");
	//swapcontext(&nctx,&newEntry);

	/*
	ucontext_t cctx,ncctx;
	
	if (getcontext(&cctx) <0) {
		printf("Error getting context. ");
		perror("getcontext");
		exit(1);
	}

	void *stack=malloc(STACK_SIZE);
    
      	if (stack==NULL) {
        	perror("Failed to allocate stack");
        	exit(1);
	}

    
      	cctx.uc_link=NULL;
    
      	cctx.uc_stack.ss_sp=stack;
    
      	cctx.uc_stack.ss_size=STACK_SIZE;
    
     	cctx.uc_stack.ss_flags=0;

	puts(" about to call make  context");
    	//makecontext(&cctx,(void *)&simplef,0);
	makecontext(&cctx,(void *)&function,0);
   	puts("Successfully modified context");
	
    
    	setcontext(&cctx);
	*/

	printf("You tried to create a new thread\n");
	//return *function;

}

void enqueueThreadToTCB(my_pthread_t *newThread){
	my_pthread_tcb *newEntry = malloc(sizeof(my_pthread_tcb));
	//newEntry->tid = id of new thread
	newEntry->status = RUNNABLE;
	//newEntry->context = u context
	newEntry->next = NULL;

	if(blockHead == NULL) {
		blockHead = newEntry;
		blockHead->next = blockHead;
		return;
	}

	my_pthread_tcb *ptr = blockHead;
	
	while(ptr->next != blockHead){
		ptr = ptr->next;
	}

	ptr->next = newEntry;
	newEntry->next = blockHead;

}

void getNumProcessesInTCB(){
	my_pthread_tcb *ptr = blockHead;
	if(ptr == NULL){
		printf("There are 0 processes in the TCB\n");
		return;	
	}
	if(ptr->next = ptr){
		printf("There is 1 process in the TCB\n");
		return;	
	}	
	int numProcesses = 1;
	while(ptr->next != ptr){
		numProcesses++;
		ptr = ptr->next;
	}
	printf("There are %d processes in the TCB\n",numProcesses);
	return;	
}
/*
void addThreadToEndOfTCB(my_pthread_t *newThread){
	my_pthread_tcb *newEntry = malloc(sizeof(my_pthread_tcb));
	//newEntry->tid = id of new thread
	newEntry->status = RUNNABLE;
	//newEntry->context = u context
	newEntry->next = NULL;

	if (*block == NULL) {		
		(*block) = newEntry;
		return;
	}

	my_pthread_tcb *ptr = (*block);
	while(ptr->next != NULL){
		ptr = ptr->next;
	}
	ptr->next = newEntry;
}
*/

//---------------------------------------------------------------------------------------
//-------------------------------my_pthread_yield()--------------------------------------
//---------------------------------------------------------------------------------------

/* Give up the CPU and allow the next thread to run.
 */
void my_pthread_yield(){

  // Implement Here

}

//---------------------------------------------------------------------------------------
//-------------------------------my_pthread_join()---------------------------------------
//---------------------------------------------------------------------------------------

/* The calling thread will not continue until the thread with tid thread
 * has finished executing.
 */
void my_pthread_join(my_pthread_t thread){

  // Implement Here //

}

//---------------------------------------------------------------------------------------
//-------------------------------my_pthread_self()---------------------------------------
//---------------------------------------------------------------------------------------

/* Returns the thread id of the currently running thread
 */
my_pthread_t my_pthread_self(){

  // Implement Here //

  return 0; // temporary return, replace this

}

//---------------------------------------------------------------------------------------
//-------------------------------my_pthread_exit()---------------------------------------
//---------------------------------------------------------------------------------------

/* Thread exits, setting the state to finished and allowing the next thread
 * to run.
 */
void my_pthread_exit(){

  // Implement Here //

}
