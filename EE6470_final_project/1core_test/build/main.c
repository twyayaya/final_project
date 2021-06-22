#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "data.h"
#include "crc.h"
#include "crc2.h"
#include "crc3.h"

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

int main()
{
    /*
    const int sorting_buffer_size = 50;
    const int number_batch = 2;
    struct myStringStruct array[sorting_buffer_size];
    int round, count = 0;

    //uint8_t start_qsort[] = {"\r\n\r\nStart qsort50x2()!\r\n\0"};

    for (round = 0; round < number_batch; round++)
    {
        for (count = 0; count < sorting_buffer_size; count++)
        {
            strcpy(array[count].qstring, input_small_data[round * sorting_buffer_size + count]);
            //把input_small_data裡面的東西丟到array裡
        }

        //siprintf(print_buffer, "\r\nRound %d Sorting %d elements:\r\n\0", round, count);
        printf("\r\nRound %d Sorting %d elements:\r\n\0", round, count);
        qsort(array, count, sizeof(struct myStringStruct), compare);
        //比array裡面字串大小

        int i = 0;
        for (i = 0; i < count; i++)
        {
            //siprintf(print_buffer, "%s, \0", array[i].qstring);
            printf("%s, \0", array[i].qstring);
            //UART_polled_tx_string(&g_uart, (const uint8_t *)&print_buffer);
        }
        //UART_polled_tx_string(&g_uart, (const uint8_t *)"\r\n\0");
    }
    */
    int i = 0;
    for (i; i < 2; i++)
    {
        unsigned long qsort_crc = qsort_crc_64x32(i);
        printf("C0%d: qsort_crc=%04lX\r\n\0", i, qsort_crc);
    }

    return 0;
}
