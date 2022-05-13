#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ABS(a)    ((a) < (0)) ? -(a) : (a)
#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) < (b)) ? (b) : (a)

#define MAX_ID 1000000
#define MAX_CABIN 20001
#define IN 1
#define OUT 2
#define SPAN_CNT 25001
#define NOT_POSSIBLE_LENGTH 100000
#define NOT_EXISTING_ID 1000000

unsigned cabins;
unsigned id;
unsigned cases;  //what where
unsigned inout;
unsigned int result;
int id_to_cab[MAX_ID];

struct span {
    unsigned int start;
    unsigned int length;
};

struct span spans[SPAN_CNT];
struct span root;
unsigned int heap_size;
unsigned int l, r, parent;

#define PARENT(i) ((i - 1) / 2)
#define LEFT(i)   (2 * i + 1)
#define RIGHT(i)  (2 * i + 2)

#define SWAP(a, b) do {  \
    struct span tmp;     \
    tmp = spans[a];      \
    spans[a] = spans[b]; \
    spans[b] = tmp;      \
} while(0)

static unsigned st_a = 0, st_b = 0;
static unsigned len_a = 0, len_b = 0, len_t;

static int left_greater(unsigned a, unsigned b)
{
    st_a = spans[a].start;
    len_a = spans[a].length;
    st_b = spans[b].start;
    len_b = spans[b].length;

/*  //looks like bad decision.
    if (st_a == 0 || (st_a + len_a > cabins)) {
        len_a = len_a / 2 - 1;
    } else {
        len_a = len_a / 2 + len_a % 2;
    }
    if (st_b == 0 || (st_b + len_b > cabins)) {
        len_b = len_b / 2 - 1;
    } else {
        len_b = len_b / 2 + len_b % 2;
    }*/
    //Maybe it is better.
    len_a = len_a / 2 + len_a % 2;
    len_b = len_b / 2 + len_b % 2;

    if (len_a > len_b) {
        return 1;
    } else if (len_b > len_a) {
        return 0;
    } else if (st_a < st_b) {
        return 1;
    } else {
        return 0;
    }


}

void max_heapify(unsigned int i)
{
    unsigned int max = i;
    l = LEFT(i);
    r = RIGHT(i);

    if (l < heap_size && left_greater(l, i)) {
        max = l;
    }
    if (r < heap_size && left_greater(r, max)) {
        max = r;
    }
    if (max != i) {
        SWAP(i, max);
        max_heapify(max);
    }
}

//struct span *extract_max(void)
struct span extract_max(void)
{
/*  if (heap_size == 0) {
        printf("Could not exrtact element from heap, its size = 0\n");
        root.length = NOT_POSSIBLE_LENGTH;
        return root;
    }*/
    if (heap_size == 1) {
        heap_size--;
        return spans[0];
    }

    root = spans[0];
    spans[0] = spans[heap_size - 1];
    heap_size--;
    max_heapify(0);

    return root;
}

struct span *get_max(void)
//struct span get_max(void)
{
    if (heap_size == 0) {
        printf("Could not take max element - heap is empty\n");
//        root.length = NOT_POSSIBLE_LENGTH;
//        return root;
    }
    return &spans[0];
}

void increase_item(unsigned int i, struct span span)
{
    spans[i] = span;
    while (i != 0 && left_greater(i, PARENT(i))) {
        SWAP(i, PARENT(i));
        i = PARENT(i);
    }
}

void delete_item(unsigned int i)
{
    struct span max;
    max.length = NOT_POSSIBLE_LENGTH;

    increase_item(i, max);
    extract_max();
}

void insert_item(struct span item, unsigned num)
{
    register int i;

    if (heap_size == SPAN_CNT) {
        printf("Could not add new spans in heap, i = %u\n", num);
        return;
    }
    heap_size++;
    i = heap_size - 1;
    spans[i] = item;
    //printf("insert: item.start = %u, item.length = %u, heap_size = %u\n", item.start, item.length, heap_size);

    while (i != 0 && left_greater(i, PARENT(i))) {
        SWAP(i, PARENT(i));
        i = PARENT(i);
    }
}

static void solve(void)
{
    unsigned it1 = 0;
    unsigned del_start;
    unsigned del_length;
    struct span span, new_span;

    heap_size = 0;
    span.start = 0;
    span.length = cabins * 2;
    insert_item(span, 0);

    for (register unsigned int i = 1; i <= cases; i++) {
        scanf("%u %u", &inout, &id);
        //printf("%u %u\n", inout, id);
        if (inout == IN) {
            //Perform in
            //first
            span = extract_max();
            //printf("inout = %u, id = %u, heap_size = %d, span.start = %d, span.length = %d\n", inout, id, heap_size, span.start, span.length);
            if (span.start == 0 && span.start + span.length > cabins) { //going to first
                // 2 conditions 1 and mre than 2
                span.start = 1;
                if (heap_size == 0) {
                    span.length = span.length - 2;
                } else {
                    span.length = span.length / 2 - 1;
                }
                insert_item(span, i);
                id_to_cab[id] = 1;
                new_span.start = 0;
                new_span.length = 0;
                insert_item(new_span, i);
                result += 1;
//                printf("result1 = %d\n", result);
            } else if (span.start > 0 && span.start + span.length > cabins) { //going to the last
                span.length = span.length / 2 - 1;
                insert_item(span, i);
                span.length = 0;
                span.start = cabins;
                insert_item(span, i);
                id_to_cab[id] = cabins;
                result += cabins;
                //printf("result2 = %d\n", result);
            } else {
                if (span.start == 0) {
                    new_span.start = 1;
                    new_span.length = span.length / 2 - 1;
                    // try next
                    result += 1;
                    id_to_cab[id] = 1;
                    insert_item(new_span, i);
                    span.length = 0;
                    insert_item(span, i);
//                    printf("result3 = %u\n", result);
                } else {
                    len_t = span.length / 2 + span.length % 2;
                    new_span.start = span.start + len_t;
                    new_span.length = span.length - len_t;
                    id_to_cab[id] = new_span.start;
                    result += new_span.start;
                    insert_item(new_span, i);
                    //try based on len_t
                    span.length = span.length - new_span.length - 1;
                    insert_item(span, i);
//                    printf("result4 = %u\n", result);
                }
            }
        } else if (inout == OUT) {
            //search deleting interval
            del_start = id_to_cab[id];
            for (register unsigned int i = 0; i < heap_size; i++) {
                if (spans[i].start == del_start) {
                    it1 = i;
                    break;
                }
            }
            del_start = spans[it1].start;
            del_length = spans[it1].length;
            delete_item(it1);
            //search previous interval
            for (register unsigned int i = 0; i < heap_size; i++) {
                if (spans[i].start + spans[i].length + 1 == del_start) {
                    it1 = i;
                    break;
                }
            }
            span = spans[it1];
            delete_item(it1);
            if (del_start == 1 && del_start + del_length > cabins) {
                span.start = 0;
                span.length = cabins * 2;
            } else if (del_start == 1 && del_start + del_length <= cabins) {
                span.start = 0;
                span.length = (del_length + 1) * 2;
            } else if (del_start == cabins && span.start == 0) {
                //span.start = 0;
                span.length = cabins * 2;
            } else if (del_start == cabins && span.start != 0) {
                span.length = (span.length + 1) * 2;
            } else if (span.start != 0 && del_start + del_length > cabins) {
                span.length = del_length + 2;
            } else if (span.start != 0 && del_start + del_length < cabins) {
                span.length = del_length + span.length + 1;
            }
            insert_item(span, i);
        }
    }
    while (heap_size != 0) {
        span = extract_max();
        printf("span.start = %u, span.length = %u\n", span.start, span.length);
    }

}

int main(void)
{
	unsigned tests_num;

    scanf("%u", &tests_num);
    tests_num++;
    //printf("tests_num = %u\n", tests_num);

    for (register unsigned i = 1; i < tests_num; i++) {
        scanf("%u %u", &cabins, &cases);
        //printf("cabins = %u, cases = %u\n", cabins, cases);
        result = 0;
        solve();
        printf("#%u %u\n", i, result);
	}
/*    printf("heap_size = %u\n", heap_size);
    while (heap_size != 0) {
        printf("%u \n", extract_max());
    }*/

	return 0;
}
