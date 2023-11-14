//
// Created by JALOL on 14.11.2023.
//

#include "vector.h"

#include <inttypes.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

struct vector {
    size_t size;
    int64_t* elems;
    bool* stats;
};

struct vector* vector_allocate() {
    struct vector* v = malloc(sizeof(struct vector));
    if (v == NULL) {
        return NULL;
    }

    v->size = 0;
    v->elems = malloc(sizeof(int64_t));
    v->stats = malloc(sizeof(bool));

    if (v->elems == NULL || v->stats == NULL) {
        free(v->elems);
        free(v->stats);
        free(v);
        return NULL;
    }

    return v;
}

void vector_reallocate(struct vector* v, size_t newsize) {
    size_t pre = v->size;
    v->size = newsize;

    v->elems = realloc(v->elems, sizeof(int64_t) * newsize);
    v->stats = realloc(v->stats, sizeof(bool) * newsize);

    if (v->elems == NULL || v->stats == NULL) {
        v->size = pre;
        return;
    }

    for (size_t i = pre; i < newsize; i++) {
        v->stats[i] = false;
    }
}

void push(struct vector* v, int64_t num) {
    vector_reallocate(v, v->size + 1);
    v->elems[v->size - 1] = num;
    v->stats[v->size - 1] = true;
    // Увеличивать размер вектора не нужно, это делается в vector_reallocate
}

void add_to_vector(struct vector* v, int64_t num) {
    if (v->size == 0) {
        push(v, num);
        return;
    }
    for (size_t i = 0; i < v->size; i++) {
        if (!v->stats[i]) {
            v->elems[i] = num;
            v->stats[i] = true;
            return;
        }
    }
    push(v, num);
}

void vector_to_vector(struct vector* child, struct vector* parent) {
    for (size_t i = 0; i < child->size; i++) {
        add_to_vector(parent, child->elems[i]);
    }
}

void swap(struct vector* v, size_t a, size_t b) {
    int64_t temp = v->elems[a];
    v->elems[a] = v->elems[b];
    v->elems[b] = temp;
}

void remove_from_vector(struct vector* v, size_t i) {
    v->elems[i] = 0;
    for (size_t t = i + 1; t < v->size; t++) {
        swap(v, t, t - 1);
        if (v->stats[t] == false) {
            v->stats[t - 1] = false;
            return;
        }
    }
}

int64_t get_from_vector(struct vector* v, size_t i) {
    return v->elems[i];
}

void set_in_vector(struct vector* v, size_t i, int64_t val) {
    v->elems[i] = val;
}

size_t vector_size(struct vector* v) {
    return v->size;
}

size_t vector_memory_size(struct vector* v) {
    return sizeof(int64_t) * v->size;
}

void vector_free(struct vector* v) {
    free(v->elems);
    free(v->stats);
    v->size = 0;
}

void vector_print(struct vector* v, FILE* out) {
    printf("{ ");
    for (size_t i = 0; i < v->size; i++) {
        if (v->stats[i]) {
            fprintf(out, "%" PRId64 " ", v->elems[i]);
        }
    }
    printf("}");
    fprintf(out, "\n");
}
