//
// Created by JALOL on 14.11.2023.
//
#pragma once
#include <stdint.h>
#include <stdio.h>
#ifndef VECTOR_H
#define VECTOR_H
#define VECTOR struct vector*

    struct vector;
    struct vector* vector_allocate();
    void add_to_vector(struct vector*, int64_t);
    void remove_from_vector(struct vector*, size_t);
    int64_t get_from_vector(struct vector*, size_t);
    void set_in_vector(struct vector*, size_t, int64_t);
    size_t vector_size(struct vector*);
    void vector_reallocate(struct vector*, size_t);
    void vector_print(struct vector* v, FILE* out);
    void vector_free(struct vector* v);
    void vector_to_vector(struct vector* child, struct vector* parent);

#endif//VECTOR_H
