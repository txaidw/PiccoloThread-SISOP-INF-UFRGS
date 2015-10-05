

#include "../src/pidata.c"
#include "../src/pithread.c"

#include <stdlib.h>
#include <stdio.h>



int main(int argc, char const *argv[]) {
	printf("START PERSISTENCE TESTS\n");
	int tid = 1;

	// Insertion test on the ready_active queue with priority;
	TCB_t *um = (TCB_t*) malloc(sizeof(TCB_t));
	um->tid = tid++;
	um->credCreate = um->credReal = 1;

	ready_active_insert(um);

	TCB_t *dois = (TCB_t*) malloc(sizeof(TCB_t));
	dois->tid = tid++;
	dois->credCreate = dois->credReal = 5;
	ready_expired_insert(dois);

	TCB_t *tres = (TCB_t*) malloc(sizeof(TCB_t));
	tres->tid = tid++;
	tres->credCreate = tres->credReal = 1;
	ready_active_insert(tres);

	TCB_t *quatro = (TCB_t*) malloc(sizeof(TCB_t));
	quatro->tid = tid++;
	quatro->credCreate = quatro->credReal = 100;
	ready_active_insert(quatro);
	
	// First print to check the insertions;
	printAllQueues();
	
	// removing from the ready_active and inserting on the blocked_list;
	TCB_t *cinco_removed = ready_active_remove_and_return();
	blocked_list_insert(cinco_removed);

	TCB_t *seis_removed = ready_active_remove_and_return();
	blocked_list_insert(seis_removed);
	
	// Print to check the removal;
	printAllQueues();

	// Removal from the blocked_list;
	blocked_list_remove(seis_removed);
	// Final print to check the removal;
	printAllQueues();

	return 0;
}