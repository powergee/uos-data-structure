#include <stdio.h>
#include <stdlib.h>
#define TRACE_FILE_NAME "ref_stream.txt"

int main()
{
    FILE* fp = NULL;
    unsigned long block;
    int total = 0;
    int interest = 0;

    printf("Starting...\n");
    if ((fp = fopen(TRACE_FILE_NAME, "r")) == NULL) 
    {
		printf("%s trace file open fail.\n", TRACE_FILE_NAME);
		return 0;
	}
    while (fscanf(fp, "%lu", &block) == 1)
    {
        ++total;
        if (block % 4 == 0)
            ++interest;
    }
    printf("Finished.\n");

    printf("%d/%d (%lf %%)\n", interest, total, (double)interest / total * 100);
}