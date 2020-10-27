#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>
#define	MAX_CACHE_SIZE		8192
// 데이터 참조 스트림이 저장된 파일 
#define TRACE_FILE_NAME		"ref_stream.txt"

// 이중연결리스트의 노드 타입
typedef struct _buffer {
	unsigned long blkno;
	struct _buffer* next, * prev;
} buffer;

// 데이터를 저장할 공간을 한 번에 할당 받음
// 이후 추가적인 메모리 할당은 없음
// 할당 받은 노드들을 이용하여 LRU 리스트 또는 FIFO 리스트를 유지해야 함 
buffer cache_buffer[MAX_CACHE_SIZE];

// LRU 시뮬레이터 인 경우 lrulist 아니면 fifolist를 유지하기 위한 헤드 노드 선언
#if 1
buffer lrulist;
#else
buffer fifolist;
#endif

// 사용되지 않은 노드들을 별도의 리스트로 관리.
buffer free_node_list;

void init(buffer* head)
{
    head->prev = head;
    head->next = head;
}

void insert(buffer* before, buffer* to_insert)
{
    to_insert->next = before->next;
    to_insert->prev = before;

    before->next->prev = to_insert;
    before->next = to_insert;
}

buffer* delete(buffer* head, buffer* to_remove)
{
    if (to_remove == head)
        return NULL;
    
    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    return to_remove;
}

int is_empty(buffer* head)
{
    return head->next == head;
}

buffer* search(buffer* head, unsigned long blkno)
{
    buffer* p;

    for (p = head->next; p != head; p = p->next)
        if (p->blkno == blkno)
            return p;
    return NULL;
}

int main(int argc, char* argv[])
{
	int	total_count = 0, hit_count = 0, i;
	unsigned long blkno;
    buffer* found;
    buffer* empty_buffer;
    clock_t start, end;
	FILE* fp = NULL;

	init(&lrulist);
    init(&free_node_list);

    for (i = 0; i < MAX_CACHE_SIZE; ++i)
	{
		cache_buffer[i].blkno = -1;
		cache_buffer[i].next = NULL;
		cache_buffer[i].prev = NULL;
        insert(&free_node_list, cache_buffer + i);
	}

	if ((fp = fopen(TRACE_FILE_NAME, "r")) == NULL) 
    {
		printf("%s trace file open fail.\n", TRACE_FILE_NAME);
		return 0;
	}
	else 
		printf("start simulation!\n");

	////////////////////////////////////////////////
	// 시뮬레이션 시작 및 결과 출력을 위한 코드
	////////////////////////////////////////////////

	start = clock();

    while (!feof(fp))
    {
        ++total_count;
        fscanf(fp, "%lu", &blkno);
        found = search(&lrulist, blkno);

        // 캐시에서 히트되지 않았을 경우
        if (found == NULL)
        {
            // 캐시에서 히트되지 않았다면 캐시의 빈 노드에 값을 할당하거나,
            // 캐시에 더이상 공간이 없다면 LRU 교체 정책에 따라
            // 가장 오랫동안 사용하지 않은 값을 제거해야 함.

            // 캐시에 더이상 공간이 없는 경우,
            // 가장 오랫동안 사용하지 않은 값을 제거하고 그 자리에 지금의 값을 넣음.
            if (is_empty(&free_node_list))
            {
                empty_buffer = delete(&lrulist, lrulist.prev);
                empty_buffer->blkno = blkno;
                insert(&lrulist, empty_buffer);
            }
            // 캐시에 빈 공간이 있는 경우,
            // 빈 공간 중 하나에 지금의 값을 넣음.
            else
            {
                empty_buffer = delete(&free_node_list, free_node_list.next);
                empty_buffer->blkno = blkno;
                insert(&lrulist, empty_buffer);
            }
        }
        // 캐시에서 히트된 경우
        else
		{
            ++hit_count;

			// 히트된 값을 리스트의 가장 처음으로 옮김.
			delete(&lrulist, found);
			insert(&lrulist, found);
		}
    }

    end = clock();
    printf("Total count: %d\n", total_count);
    printf("Hit count: %d\n", hit_count);
    printf("Ratio: %lf %%\n", (double)hit_count / total_count * 100.0);
    printf("Elapsed: %lf sec\n", (double)(end-start) / CLOCKS_PER_SEC);
	fclose(fp);

	return 0;
}