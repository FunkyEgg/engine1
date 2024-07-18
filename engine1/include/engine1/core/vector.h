#ifndef ENGINE1_CORE_VECTOR_H
#define ENGINE1_CORE_VECTOR_H

#include <stdint.h>

#define E1_VECTOR_INITAL_CAPACITY 16

typedef struct Vector {
    uint32_t size;
    uint32_t capacity;
    uint16_t elem_size;
    void** elems;
} Vector;

Vector vector_create_empty(uint16_t elem_size);
void vector_destoy(Vector* vector);
void vector_resize(Vector* vector);
void vector_push_back(Vector* vector, void* elem);
void* vector_get(Vector* vector, uint32_t index);
void* vector_get_last(Vector* vector);

#endif
