#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ABS(a)    ((a) < (0)) ? -(a) : (a)
#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#define MAX(a, b) ((a) < (b)) ? (b) : (a)

#define MAX_ID 1000000
#define MAX_CABIN 20001
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

static int diff_a, diff_b, place_a, place_b;

static int left_greater(unsigned a, unsigned b)
{
/*    place_a = spans[a].start + spans[a].length / 2 + spans[a].length % 2;
    place_b = spans[b].start + spans[b].length / 2 + spans[b].length % 2;
    diff_a = place_a - spans[a].start - 1;
    diff_b = place_b - spans[b].start - 1;
*/
    diff_a = spans[a].length / 2 + spans[a].length % 2 - 1;
    diff_b = spans[b].length / 2 + spans[b].length % 2 - 1;
    place_a = diff_a + 1 + spans[a].start;
    place_b = diff_b + 1 + spans[b].start;

    if (diff_a > diff_b) {
        return 1;
    } else if (diff_a < diff_b) {
        return 0;
    } else {
//    if (spans[a].start + spans[a].length > cabins) {
        if (spans[a].start == 0) {
            place_a = 1;
        } else if (spans[a].start + spans[a].length > cabins) {
            place_a = cabins;
        }
 //   }
//    if (spans[b].start + spans[b].length > cabins) {
        if (spans[b].start == 0) {
            place_b = 1;
        } else if (spans[b].start + spans[b].length > cabins) {
            place_b = cabins;
        }
//    }


        return place_a < place_b;
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
//        printf("extract: item.start = %u, item.length = %u, heap_size = %u\n", spans[0].start, spans[0].length, heap_size);
        return spans[0];
    }
    root = spans[0];
 //   printf("extract: item.start = %u, item.length = %u, heap_size = %u\n", spans[0].start, spans[0].length, heap_size);
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
//    struct span extr;
//    unsigned length = spans[i].length;
    max.length = NOT_POSSIBLE_LENGTH;

//    printf("i = %u\n", i);
    increase_item(i, max);
    extract_max();
//    extr = extract_max();
//    printf("i = %u, extr.start = %u, extr.length = %u\n", i, extr.start, length);
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

__attribute__((unused)) static void print_heap(void)
{
    struct span span;
    for (unsigned int i = 0; i < heap_size; i++) {
        span = spans[i];
        printf("heap: span.start = %u, span.length = %u\n", span.start, span.length);
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
    span.length = cabins;
    insert_item(span, 0);

    for (register unsigned int i = 1; i <= cases; i++) {
        scanf("%u %u", &inout, &id);
        //printf("%u %u\n", inout, id);
        if (inout == 1) {
            //Perform in
            //first
            span = extract_max();
            //printf("heap_size = %d, span.start = %d, span.length = %d\n", heap_size, span.start, span.length);
            if (heap_size == 0 && span.start == 0) { //cabins empty going to first
                span.start = 1;
                span.length = (span.length - 1) * 2 - 1;
                insert_item(span, i);
                id_to_cab[id] = 1;
                new_span.start = 0;
                new_span.length = 0;
                insert_item(new_span, i);
                result += 1;
                //printf("result1 = %d\n", result);
            } else if (heap_size == 1 && span.start == 1) { //going to the last
                span.length = cabins - 2;
                insert_item(span, i);
                new_span.start = cabins;
                new_span.length = 0;
                insert_item(new_span, i);
                id_to_cab[id] = cabins;
                result += cabins;
                //printf("result2 = %d\n", result);
            } else if (span.start == 0) {
                new_span.start = 1;
                new_span.length = span.length / 2;
                result += 1;
                insert_item(new_span, i);
                id_to_cab[id] = 1;
                span.length = 0;
                insert_item(span, i);
                //printf("result3 = %d\n", result);
            } else if (span.start + span.length > cabins) {
                new_span.start = cabins;
                new_span.length = 0;
                id_to_cab[id] = cabins;
                insert_item(new_span, i);
                result += cabins;
                span.length = span.length / 2;
                insert_item(span, i);
                //printf("result4 = %d\n", result);
            } else {
                new_span.start = span.start + span.length / 2 + span.length % 2;
                new_span.length = span.length / 2;
                insert_item(new_span, i);
                id_to_cab[id] = new_span.start;
                result += new_span.start;
                span.length = new_span.start - span.start - 1;
                insert_item(span, i);
                //printf("result5 = %d\n", result);
            }
        } else if (inout == 2) {
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
            printf("del_start = %u, del_length = %u\n", del_start, del_length);
            //print_heap();
            //search previous interval
            for (register unsigned int i = 0; i < heap_size; i++) {
                if (spans[i].start == 0 && (spans[i].start + spans[i].length / 2 + spans[i].length % 2) == del_start) {
                    it1 = i;
                    printf("Found1!\n");
                    break;
                }
                if (spans[i].start + spans[i].length + 1 == del_start) {
                    it1 = i;
                    printf("Found2!\n");
                    break;
                }
            }
            span = spans[it1];
            delete_item(it1);
            printf("span.start = %u\n", span.start);
            if (span.start == 0) {
                span.length = (span.length / 2 + span.length % 2 + del_length + 1) * 2 - 1;
            } else if (span.start == 1) {
                span.length = (span.length / 2 + span.length % 2 + del_length + 1) * 2 - 1;
            }else if (del_start + del_length >= cabins) {
                //printf("1\n");
                span.length = (cabins - span.start) * 2 - 1;
                //printf("Fuuuuck! del_start = %u, del_length = %u\n", del_start, del_length);
            //} else if (span.start == 0) {
                //printf("2\n");
                //span.length = (span.length / 2 + span.length % 2 + del_length + 1) * 2 - 1;
            } else {
                //printf("3\n");
                span.length = span.length + del_length + 1;
            }
            insert_item(span, i);
            //print_heap();
        }
            //optimize!!
    }
    print_heap();
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
