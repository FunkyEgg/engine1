#ifndef ENGINE1_CORE_VECTOR_H
#define ENGINE1_CORE_VECTOR_H

#include <stdint.h>
#include <stdbool.h>

#define E1_VECTOR_INITAL_CAPACITY 16

typedef struct Vector {
    // This means length, but it's too late to change it now
    uint32_t size;
    uint32_t capacity;
    uint16_t elem_size;
    void* elems;
} Vector;

Vector vector_create_empty(uint16_t elem_size);
Vector vector_create_from_array(void* elems, uint32_t elems_length, uint16_t elem_size);
void vector_destroy(Vector* vector);
void vector_resize(Vector* vector);
void vector_resize_to(Vector* vector, uint32_t size);
void vector_push_back(Vector* vector, void* elem);
bool vector_type_is(const Vector* const vector, uint16_t type_size);
bool vector_null(const Vector* const vector); // Helper method because I'm lazy. Maybe macro?
bool vector_as_expected(const Vector* const vector, uint16_t type_size);

// TODO: Vector copy
//       Vector pop

#endif
