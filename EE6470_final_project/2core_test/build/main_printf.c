#include "stdio.h"
#include "string.h"
#include "math.h"
///////////////////////////
#include <stdlib.h>
#include <string.h>
#include "data.h"
///////////////////////////
///////////////////////////new2///////////////////////////
#include "crc.h"
#include "crc2.h"
#include "crc3.h"
///////////////////////////new2///////////////////////////
int sem_init(uint32_t *__sem, uint32_t count) __THROW
{
	*__sem = count;
	return 0;
}

int sem_wait(uint32_t *__sem) __THROW
{
	uint32_t value, success; //RV32A
	__asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     beqz %[value],L%=                   # if zero, try again\n\t\
     addi %[value],%[value],-1           # value --\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
						 : [value] "=r"(value), [success] "=r"(success)
						 : [__sem] "r"(__sem)
						 : "memory");
	return 0;
}

int sem_post(uint32_t *__sem) __THROW
{
	uint32_t value, success; //RV32A
	__asm__ __volatile__("\
L%=:\n\t\
     lr.w %[value],(%[__sem])            # load reserved\n\t\
     addi %[value],%[value], 1           # value ++\n\t\
     sc.w %[success],%[value],(%[__sem]) # store conditionally\n\t\
     bnez %[success], L%=                # if the store failed, try again\n\t\
"
						 : [value] "=r"(value), [success] "=r"(success)
						 : [__sem] "r"(__sem)
						 : "memory");
	return 0;
}

int barrier(uint32_t *__sem, uint32_t *__lock, uint32_t *counter, uint32_t thread_count)
{
	sem_wait(__lock);
	if (*counter == thread_count - 1)
	{ //all finished
		*counter = 0;
		sem_post(__lock);
		for (int j = 0; j < thread_count - 1; ++j)
			sem_post(__sem);
	}
	else
	{
		(*counter)++;
		sem_post(__lock);
		sem_wait(__sem);
	}
	return 0;
}

///////////////////////////new2///////////////////////////
/*
extern char print_buffer[64];

extern const int input_small_data_size;
extern const char *input_small_data[];

struct myStringStruct
{
	char qstring[32];
};

int compare(const void *elem1, const void *elem2)
{
	int result;

	result = strcmp((*((struct myStringStruct *)elem1)).qstring, (*((struct myStringStruct *)elem2)).qstring);
	//compare string(elem1,elem2)
	//elem1 > elem2 -> result > 0
	//elem1 < elem2 -> result < 0
	//elem1 = elem2 -> result = 0

	return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
	//result == 0 -> 0
	//result != 0 ->-1
	//result  > 0 -> 1
}
*/
///////////////////////////new2///////////////////////////

//Total number of cores
//static const int PROCESSORS = 2;
#define PROCESSORS 2
//the barrier synchronization objects
uint32_t barrier_counter = 0;
uint32_t barrier_lock;
uint32_t barrier_sem;
//the mutex object to control global summation
uint32_t lock;
//print synchronication semaphore (print in core order)
uint32_t print_sem[PROCESSORS];

int main(unsigned hart_id)
{

	/////////////////////////////
	// thread and barrier init //
	/////////////////////////////

	if (hart_id == 0)
	{
		// create a barrier object with a count of PROCESSORS
		sem_init(&barrier_lock, 1);
		sem_init(&barrier_sem, 0); //lock all cores initially
		for (int i = 0; i < PROCESSORS; ++i)
		{
			sem_init(&print_sem[i], 0); //lock printing initially
		}
		// Create mutex lock
		sem_init(&lock, 1);
	}
	///////////////////////////new2///////////////////////////
	/*
	const int sorting_buffer_size = 50;
	const int number_batch = 2;
	struct myStringStruct array[sorting_buffer_size];
	int round, count = 0;
*/
	///////////////////////////new2///////////////////////////
	////////////////////////////
	// barrier to synchronize //
	////////////////////////////
	//Wait for all threads to finish
	barrier(&barrier_sem, &barrier_lock, &barrier_counter, PROCESSORS);

	if (hart_id == 0)
	{ // Core 0 print first and then others
		///////////////////////////new2///////////////////////////
		/*
		printf("start core%d\n", hart_id);
		for (count = 0; count < sorting_buffer_size; count++)
		{
			strcpy(array[count].qstring, input_small_data[0 * sorting_buffer_size + count]);
			//把input_small_data裡面的東西丟到array裡
		}

		//siprintf(print_buffer, "\r\nRound %d Sorting %d elements:\r\n\0", round, count);
		printf("\r\nRound %d Sorting %d elements:\r\n\0", 0, count);
		qsort(array, count, sizeof(struct myStringStruct), compare);
		//比array裡面字串大小

		int i = 0;
		for (i = 0; i < count; i++)
		{
			printf("%s, \0", array[i].qstring);
		}
		printf("\nEnd core%d\n", hart_id);
		*/
		unsigned long qsort_crc = qsort_crc_64x32(0);
		printf("C00: qsort_crc=%04lX\r\n\0", qsort_crc);
		///////////////////////////new2///////////////////////////
		sem_post(&print_sem[1]); // Allow Core 1 to print
	}
	else
	{

		//printf("id1 = %d\n", hart_id);
		for (int i = 1; i < PROCESSORS; ++i)
		{
			sem_wait(&print_sem[i]);
			///////////////////////////new2///////////////////////////
			/*
			printf("\nstart core%d\n", hart_id);
			for (count = 0; count < sorting_buffer_size; count++)
			{
				strcpy(array[count].qstring, input_small_data[1 * sorting_buffer_size + count]);
				//把input_small_data裡面的東西丟到array裡
			}

			//siprintf(print_buffer, "\r\nRound %d Sorting %d elements:\r\n\0", round, count);
			printf("\r\nRound %d Sorting %d elements:\r\n\0", 1, count);
			qsort(array, count, sizeof(struct myStringStruct), compare);
			//比array裡面字串大小

			int i = 0;
			for (i = 0; i < count; i++)
			{
				printf("%s, \0", array[i].qstring);
			}
			printf("\nEnd core%d\n", hart_id);
			*/
			///////////////////////////new2///////////////////////////
			unsigned long qsort_crc = qsort_crc_64x32(1);
			printf("C01: qsort_crc=%04lX\r\n\0", qsort_crc);
			sem_post(&print_sem[i + 1]); // Allow next Core to print
		}
	}

	return 0;
}
