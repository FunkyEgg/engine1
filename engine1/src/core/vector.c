#include <engine1/core/vector.h>

#include <stdio.h>
#include <stdlib.h>

Vector vector_create_empty(uint16_t elem_size) {
    return (Vector){
        .size = 0,
        .capacity = 0,
        .elem_size = elem_size,
        .elems = NULL
    };
}

void vector_destory(Vector* vector) {
    if (vector->elems == NULL) {
        return;
    }

    free(vector->elems);
    return;
}

void vector_resize(Vector* vector) {
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

void vector_push_back(Vector* vector, void* elem) {
    if (vector->size == vector->capacity) {
        vector_resize(vector);
    }

    if (vector->elems == NULL) {
        return;
    }

    vector->elems[vector->size] = elem;
    vector->size++;
}

void* vector_get(Vector* vector, uint32_t index) {
    if (index >= vector->size || vector->elems == NULL) {
        return NULL;
    }

    return vector->elems[index];
}

void* vector_get_last(Vector* vector) {
    if (vector->elems == NULL) {
        return NULL;
    }

    return vector->elems[vector->size - 1];
}
