#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define HEAP_BLOCKS 16
#define BLOCK_CAPACITY 1024

enum block_status { BLK_FREE = 0, BLK_ONE, BLK_FIRST, BLK_CONT, BLK_LAST };

struct heap {
  struct block {
    char contents[BLOCK_CAPACITY];
  } blocks[HEAP_BLOCKS];
  enum block_status status[HEAP_BLOCKS];
} global_heap = {0};

struct block_id {
  size_t       value;
  bool         valid;
  struct heap* heap;
};

struct block_id block_id_new(size_t value, struct heap* from) {
  return (struct block_id){.valid = true, .value = value, .heap = from};
}
struct block_id block_id_invalid() {
  return (struct block_id){.valid = false};
}

bool block_id_is_valid(struct block_id bid) {
  return bid.valid && bid.value < HEAP_BLOCKS;
}

/* Find block */

bool block_is_free(struct block_id bid) {
  if (!block_id_is_valid(bid))
    return false;
  return bid.heap->status[bid.value] == BLK_FREE;
}

/* Allocate */
struct block_id block_allocate(struct heap* heap, size_t size) {
  if (size > HEAP_BLOCKS) return block_id_invalid();
  for (size_t i = 0; i < HEAP_BLOCKS; i++) {
    if (heap->status[i] == BLK_FREE) {
      if (size == 1) {
        heap->status[i] = BLK_ONE;
        return block_id_new(i, heap);
      }
      if (HEAP_BLOCKS - i < size) return block_id_invalid();
      bool k = 0;
      for (size_t c = i; c < HEAP_BLOCKS; c++) {
        if (heap->status[c] != BLK_FREE) k = 1;
      }
      if (k) continue;
      heap->status[i] = BLK_FIRST;
      for (size_t f = 1; f < size - 1; f++) {
        heap->status[i + f] = BLK_CONT;
      }
      heap->status[i + size - 1] = BLK_LAST;
      return block_id_new(i, heap);
    }
  }
  return block_id_invalid();
}
/* Free */
void block_free(struct block_id bid) {
  if (block_id_is_valid(bid) && bid.heap->status[bid.value] != BLK_FREE) {
    if (bid.heap->status[bid.value] == BLK_ONE) bid.heap->status[bid.value] = BLK_FREE;
    else {
      while (bid.heap->status[bid.value] != BLK_LAST) {
        bid.heap->status[bid.value] = BLK_FREE;
        bid.value++;
      }
      bid.heap->status[bid.value] = BLK_FREE;
    }
  }
}
/* Printer */
const char* block_repr(struct block_id b) {
  static const char* const repr[] = {[BLK_FREE] = " .",
                                     [BLK_ONE] = " *",
                                     [BLK_FIRST] = "[=",
                                     [BLK_LAST] = "=]",
                                     [BLK_CONT] = " ="};
  if (b.valid)
    return repr[b.heap->status[b.value]];
  else
    return "INVALID";
}

void block_debug_info(struct block_id b, FILE* f) {
  fprintf(f, "%s", block_repr(b));
}

void block_foreach_printer(struct heap* h, size_t count,
                           void printer(struct block_id, FILE* f), FILE* f) {
  for (size_t c = 0; c < count; c++)
    printer(block_id_new(c, h), f);
}

void heap_debug_info(struct heap* h, FILE* f) {
  block_foreach_printer(h, HEAP_BLOCKS, block_debug_info, f);
  fprintf(f, "\n");
}
/*  -------- */

int main() {
  printf("Выделение трех блоков\n");
  struct block_id alloc_3 = block_allocate(&global_heap, 3);
  heap_debug_info(&global_heap, stdout);

  printf("\nОчистка блока\n");
  block_free(alloc_3);
  heap_debug_info(&global_heap, stdout);

  printf("\nВыделение трех блоков и одного блока и наоборот\n");
  alloc_3 = block_allocate(&global_heap, 3);
  struct block_id alloc_1 = block_allocate(&global_heap, 1);
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_1);
  block_free(alloc_3);
  alloc_1 = block_allocate(&global_heap, 1);
  alloc_3 = block_allocate(&global_heap, 3);
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_1);
  block_free(alloc_3);

  printf("\nВыделение всей кучи\n");
  struct block_id alloc_full = block_allocate(&global_heap, HEAP_BLOCKS);
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_full);

  printf("\nВыделение при нехватке памяти\n");
  alloc_3 = block_allocate(&global_heap, 3);
  alloc_full = block_allocate(&global_heap, HEAP_BLOCKS);
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_3);

  printf("\nОчистка инвалидного блока\n");
  block_free(alloc_full);
  heap_debug_info(&global_heap, stdout);

  printf("\nВыделение одинарного блока при полной куче\n");
  alloc_full = block_allocate(&global_heap, HEAP_BLOCKS);
  alloc_1 = block_allocate(&global_heap, 1);
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_1);
  block_free(alloc_full);

  alloc_1 = block_allocate(&global_heap, 1);
  struct block_id alloc_1_2 = block_allocate(&global_heap, 1);
  struct block_id alloc_1_3 = block_allocate(&global_heap, 1);
  struct block_id alloc_1_4 = block_allocate(&global_heap, 1);
  printf("\nОчистка из середины кучи\n");
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_1_2);
  heap_debug_info(&global_heap, stdout);

  printf("\nПопытка добавления большого блока в текущую кучу\n");
  alloc_full = block_allocate(&global_heap, HEAP_BLOCKS);
  heap_debug_info(&global_heap, stdout);

  printf("\nДобавление большого блока в кучу с дырами\n");
  alloc_full = block_allocate(&global_heap, 10);
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_1);
  block_free(alloc_1_3);
  block_free(alloc_1_4);
  block_free(alloc_full);

  printf("\nПопытка добавления слишком большого блока\n");
  alloc_full = block_allocate(&global_heap, 17);
  heap_debug_info(&global_heap, stdout);
  block_free(alloc_full);
}
