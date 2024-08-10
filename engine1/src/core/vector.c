#include <engine1/core/vector.h>
#include <engine1/math/math.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vector vector_create_empty(uint16_t elem_size) {
    return (Vector){
        .size = 0,
        .capacity = 0,
        .elem_size = elem_size,
        .elems = NULL
    };
}

Vector vector_create_from_array(void* elems, uint32_t elems_length, uint16_t elem_size) {
    Vector vector = vector_create_empty(elem_size);

    if (elems == NULL || elems_length == 0 || elem_size == 0) { return vector; }

    vector_resize_to(&vector, nearest_containing_power_of_2(elems_length));

    memcpy((char*)vector.elems, elems, elem_size * elems_length);
    vector.size = elems_length;

    return vector;
}

void vector_destroy(Vector* vector) {
    if (vector == NULL || vector->elems == NULL) {
        return;
    }

    free(vector->elems);
    return;
}

void vector_resize(Vector* vector) {
    if (vector == NULL) {  return; }

    if (vector->elems == NULL) {
        vector->capacity = E1_VECTOR_INITAL_CAPACITY;
        vector->elems = malloc(vector->capacity * vector->elem_size);
    }
    else {
        vector->capacity *= 2;
        vector->elems = realloc(vector->elems, vector->capacity * vector->elem_size);
    }

    if (vector->elems == NULL) {
        // FIXME: Do some error thing here
        // For now we die
        fprintf(stderr, "Vector realloc of %u capacity failed\n", vector->capacity);
        return;
    }
}

void vector_resize_to(Vector* vector, uint32_t size) {
    if (vector == NULL) { return; }

    vector->capacity = size;
    if (vector->elems == NULL) {
        vector->elems = malloc(vector->capacity * vector->elem_size);
    }
    else {
        vector->elems = realloc(vector->elems, vector->capacity * vector->elem_size);
    }

    if (vector->elems == NULL) {
        // FIXME: Do some error thing here
        // For now we die
        fprintf(stderr, "Vector realloc to of %u capacity failed\n", vector->capacity);
        return;
    }
}

void vector_push_back(Vector* vector, void* elem) {
    if (vector == NULL) { return; }

    if (vector->size == vector->capacity) {
        vector_resize(vector);
    }

    memcpy((char*)vector->elems + vector->size * vector->elem_size, elem, vector->elem_size);

    vector->size++;
}

bool vector_type_is(const Vector* const vector, uint16_t type_size) {
    return vector->elem_size == type_size;
}

bool vector_null(const Vector* const vector) {
    return (
        vector == NULL ||
        vector->elems == NULL ||
        vector->size == 0 ||
        vector->elem_size == 0
    );
}

bool vector_as_expected(const Vector* const vector, uint16_t type_size) {
    return !vector_null(vector) && vector_type_is(vector, type_size);
}
