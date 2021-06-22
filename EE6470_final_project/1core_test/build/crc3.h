#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned long crc32buf(char *buf, size_t len);

extern char print_buffer[64];

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

extern const int input_small_data_size;
extern const char *input_small_data[];

unsigned long qsort_crc_64x32(const int core_id)
{
    const int sorting_buffer_size = 64;
    const int number_batch = 32;
    struct myStringStruct array[sorting_buffer_size];
    int round, count = 0;

    unsigned long qsort_crc_array[number_batch];
    for (round = 0; round < number_batch; round++)
    {

        memset((void *)array, 0, sorting_buffer_size * 32);
        //copy data to-be-sorted to array
        for (count = 0; count < sorting_buffer_size; count++)
        {
            strcpy(array[count].qstring, input_small_data[round * sorting_buffer_size + count]);
        }

        qsort(array, count, sizeof(struct myStringStruct), compare);
        qsort_crc_array[round] = crc32buf((char *)array, sizeof(struct myStringStruct) * sorting_buffer_size);
    }
    unsigned long crc = crc32buf((char *)qsort_crc_array, sizeof(qsort_crc_array));
    return crc;
}
