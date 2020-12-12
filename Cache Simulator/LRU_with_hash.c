#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <time.h>
#define	MAX_CACHE_SIZE		8192
#define	HASH_SIZE		((MAX_CACHE_SIZE*3/4)-1)
// 데이터 참조 스트림이 저장된 파일 
#define TRACE_FILE_NAME		"ref_stream.txt"

// 이중연결리스트 및 해시 테이블의 노드 타입
typedef struct _buffer {
	unsigned long blkno;
	struct _buffer* next, * prev;
    struct _buffer* hash_next, * hash_prev;
} buffer;

// 데이터를 저장할 공간을 한 번에 할당 받음
// 이후 추가적인 메모리 할당은 없음
// 할당 받은 노드들을 이용하여 LRU 리스트 또는 FIFO 리스트를 유지해야 함 
buffer cache_buffer[MAX_CACHE_SIZE];

buffer hash_table[HASH_SIZE];
buffer lrulist;
// 사용되지 않은 노드들을 별도의 리스트로 관리.
buffer free_node_list;

void init_list(buffer* head)
{
    head->prev = head;
    head->next = head;
}

void insert_on_list(buffer* before, buffer* to_insert)
{
    to_insert->next = before->next;
    to_insert->prev = before;

    before->next->prev = to_insert;
    before->next = to_insert;
}

buffer* delete_on_list(buffer* head, buffer* to_remove)
{
    if (to_remove == head)
        return NULL;
    
    to_remove->prev->next = to_remove->next;
    to_remove->next->prev = to_remove->prev;
    return to_remove;
}

int is_list_empty(buffer* head)
{
    return head->next == head;
}

// 연결리스트에서 blkno를 가지는 노드를 찾음. (O(n))
buffer* search_on_list(buffer* head, unsigned long blkno)
{
    buffer* p;

    for (p = head->next; p != head; p = p->next)
        if (p->blkno == blkno)
            return p;
    return NULL;
}

int get_hash(int key)
{
    return key % HASH_SIZE;
}

void init_ht(buffer* ht, int size)
{
    for (int i = 0; i < size; ++i)
    {
        ht[i].hash_prev = ht+i;
        ht[i].hash_next = ht+i;
    }
}

void insert_on_ht(buffer* ht, buffer* to_insert)
{
    int hash = get_hash(to_insert->blkno);

    to_insert->hash_next = ht[hash].hash_next;
    to_insert->hash_prev = ht + hash;

    ht[hash].hash_next->hash_prev = to_insert;
    ht[hash].hash_next = to_insert;
}

void delete_on_ht(buffer* ht, buffer* to_remove)
{
    to_remove->hash_prev->hash_next = to_remove->hash_next;
    to_remove->hash_next->hash_prev = to_remove->hash_prev;
}

// 해시 테이블의 버켓에서 blkno를 가지는 노드를 찾음. (O(1))
buffer* search_on_ht(buffer* ht, unsigned long blkno)
{
    int hash = get_hash(blkno);
    buffer* p;

    for (p = ht[hash].hash_next; p != ht+hash; p = p->hash_next)
        if (p->blkno == blkno)
            return p;
    return NULL;
}

int main(int argc, char* argv[])
{
	int	total_count = 0, hit_count = 0, i;
	unsigned long blkno;
    buffer* found;
    buffer* to_remove;
    clock_t start, end;
	FILE* fp = NULL;

	init_list(&lrulist);
    init_list(&free_node_list);

    // hash_table의 각 헤드들을 초기화
    init_ht(hash_table, HASH_SIZE);

    // cache_buffer의 각 노드들을 초기화
    for (i = 0; i < MAX_CACHE_SIZE; ++i)
	{
		cache_buffer[i].blkno = -1;
		cache_buffer[i].next = NULL;
		cache_buffer[i].prev = NULL;
        cache_buffer[i].hash_next = NULL;
		cache_buffer[i].hash_prev = NULL;
        insert_on_list(&free_node_list, cache_buffer + i);
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

    while (fscanf(fp, "%lu", &blkno) == 1)
    {
        ++total_count;
        found = search_on_ht(hash_table, blkno);

        // 캐시에서 히트되지 않았을 경우
        if (found == NULL)
        {
            // 캐시에서 히트되지 않았다면 캐시의 빈 노드에 값을 할당하거나,
            // 캐시에 더이상 공간이 없다면 LRU 교체 정책에 따라
            // 가장 오랫동안 사용하지 않은 값을 제거해야 함.

            // 캐시에 더이상 공간이 없는 경우,
            // 가장 오랫동안 사용하지 않은 값을 제거하고 그 자리에 지금의 값을 넣음.
            if (is_list_empty(&free_node_list))
            {
                // 연결리스트와 해시 테이블에서 노드를 제거함.
                to_remove = lrulist.prev;
                delete_on_list(&lrulist, to_remove);
                delete_on_ht(hash_table, to_remove);

                // 제거한 노드에 새로운 값을 할당하여 다시 연결리스트와 해시 테이블에 넣음.
                to_remove->blkno = blkno;
                insert_on_list(&lrulist, to_remove);
                insert_on_ht(hash_table, to_remove);
            }
            // 캐시에 빈 공간이 있는 경우,
            // 빈 공간 중 하나에 지금의 값을 넣음.
            else
            {
                to_remove = delete_on_list(&free_node_list, free_node_list.next);
                to_remove->blkno = blkno;
                insert_on_list(&lrulist, to_remove);
                insert_on_ht(hash_table, to_remove);
            }
        }
        // 캐시에서 히트된 경우
        else
		{
            ++hit_count;

			// 히트된 값을 리스트의 가장 처음으로 옮김.
			delete_on_list(&lrulist, found);
			insert_on_list(&lrulist, found);
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