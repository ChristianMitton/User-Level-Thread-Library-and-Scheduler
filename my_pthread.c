#include "my_pthread.h"
#define STACK_SIZE SIGSTKSZ

void enqueueThreadToTCB(my_pthread_tcb **head, my_pthread_tcb *newEntry);
my_pthread_tcb *createEntry();
int containsRunnableEntries();

static uint thread_id = 1;

struct sigaction sa;
struct itimerval timer;

/* Scheduler State */
 // Fill in Here //
my_pthread_tcb *schedulerHead = NULL;
my_pthread_tcb *scheduleFunctionContext = NULL;

//---------------------------------------------------------------------------------------
//-----------------------------------schedule--------------------------------------------
//---------------------------------------------------------------------------------------

/* Scheduler Function
 * Pick the next runnable thread and swap contexts to start executing
 */

void schedule(int signum){
  // Implement Here

	printf("In schedule\n");
	
	//point to main
	my_pthread_tcb *main = schedulerHead;
	my_pthread_tcb *nextThread = schedulerHead->next;
	//swapcontext(&main->context, &nextThread->context);

	//setitimer (ITIMER_VIRTUAL, &timer, NULL);
	while(containsRunnableEntries() == 1){
		swapcontext(&main->context, &nextThread->context);
		//printf("1\n");
	}

	//setcontext(&nextThread->context);
	//setcontext(&main->context);
	printf("set\n");


}

void returnToMain(){
	printf("in returnToMain\n");
	return;
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

	//Create TCB entry for main thread if not created
	if(schedulerHead == NULL){
		my_pthread_tcb *mainThreadEntry = createEntry();
		mainThreadEntry->tid = 0;
		//After this thread is done it returns to main?
		mainThreadEntry->context.uc_link = NULL;

		getcontext(&mainThreadEntry->context);
		makecontext(&mainThreadEntry->context,(void (*) (void))returnToMain, 0);

		//make first entry to schedulerHead queue to be main
		enqueueThreadToTCB(&schedulerHead,mainThreadEntry);

		//create context to scheduleFunction
		scheduleFunctionContext = createEntry();

		getcontext(&scheduleFunctionContext->context);
		makecontext(&scheduleFunctionContext->context,(void (*) (void))schedule, 0);
		

	}

	//Create new TCB entry
	my_pthread_tcb *newEntry = createEntry();
	newEntry->tid = thread_id; thread_id += 1;

	//points to main thread when done?:
	//newEntry->context.uc_link = &schedulerHead->context;
	//newEntry->context.uc_link = &scheduleFunctionContext->context;

	//Gets current context, puts it as a temp context for newEntry and then...
	getcontext(&newEntry->context);

	//...replaces newEntry's temp context with the new context specified by function input..?
	makecontext(&newEntry->context,(void (*) (void))function, 0);

	enqueueThreadToTCB(&schedulerHead, newEntry);

	//schedule(1);
	
	/*----- 
	 TIMER
	-----*/
	

	/*struct sigaction sa;
	struct itimerval timer;*/

	 /* Install timer_handler as the signal handler for SIGVTALRM. */
	 //memset (&sa, 0, sizeof (sa));
	 sa.sa_handler = &schedule;
	 sigaction (SIGVTALRM, &sa, NULL);

	 /* Configure the timer to expire after 250 msec... */
	 timer.it_value.tv_sec = 0;
	 //timer.it_value.tv_usec = 250000;
	 timer.it_value.tv_usec = TIME_QUANTUM_MS;
	 /* ... and every 250 msec after that. */
	 timer.it_interval.tv_sec = 0;
	 //timer.it_interval.tv_usec = 250000;
	 timer.it_value.tv_usec = TIME_QUANTUM_MS;
	 /* Start a virtual timer. It counts down whenever this process is
	   executing. */
	 setitimer (ITIMER_VIRTUAL, &timer, NULL);
	 //setitimer (ITIMER_PROF, &timer, NULL);

}

my_pthread_tcb *createEntry(){
	my_pthread_tcb *newEntry = malloc(sizeof(my_pthread_t));

	newEntry->status = RUNNABLE;
	newEntry->context.uc_link = NULL;
	newEntry->context.uc_stack.ss_sp = malloc(SIGSTKSZ);
	newEntry->context.uc_stack.ss_size = SIGSTKSZ;
	newEntry->context.uc_stack.ss_flags = 0;

	return newEntry;
}

void enqueueThreadToTCB(my_pthread_tcb **head, my_pthread_tcb *newEntry){

	if(*head == NULL){
		(*head) = newEntry;
		(*head)->next = (*head);
		return;
	}

	my_pthread_tcb *ptr = (*head);
	while(ptr->next != *head){
		ptr = ptr->next;
	}

	ptr->next = newEntry;
	ptr->next->next = (*head);
}

int containsRunnableEntries(){

	my_pthread_tcb *ptr = schedulerHead;

	while(ptr->next != ptr){
		/*
		if(ptr->tid == 0){
			continue;
		}
		*/
		if(ptr->status == RUNNABLE){
			return 1;
		}
		
		ptr = ptr->next;
	}
	//account for last entry
	if((ptr->status == RUNNABLE) && (ptr->tid != 0)){
		return 1;
	}

	return 0;

}

void getNumProcessesInTCB(){
	if(schedulerHead == NULL){
		printf("There are 0 entries in TCB\n");
		return;
	}

	my_pthread_tcb *ptr = schedulerHead;

	int numProcesses = 0;

	while(ptr->next != schedulerHead){
		numProcesses++;
		ptr = ptr->next;
	}
	//Account for the last entry
	numProcesses++;

	printf("%d thread(s) in the TCB\n",numProcesses);

	return;	
}


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









	/*
	//loop through entries

	//Note, main entry isn't considered a runnable entry

	
	while(containsRunnableEntries()){

		//if at main thread (id == 0)continue
		
		
		//switch to next thread
	}
	*/

	//swap back to main thread
