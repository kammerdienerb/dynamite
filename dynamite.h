// Brandon Kammerdiener

#ifndef _DYNAMITE_H_
#define _DYNAMITE_H_

/*
 MUST COMPILE IN C99 MODE!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define dynamite_at(da_ptr, ...) ((da_ptr)->at((da_ptr), __VA_ARGS__))
#define dynamite_begin(da_ptr) ((da_ptr)->begin((da_ptr)))
#define dynamite_end(da_ptr) ((da_ptr)->end((da_ptr)))
#define dynamite_length(da_ptr) ((da_ptr)->length((da_ptr)))
#define dynamite_push(da_ptr, ...) ((da_ptr)->push((da_ptr), __VA_ARGS__))
#define dynamite_erase(da_ptr, ...) ((da_ptr)->erase((da_ptr), __VA_ARGS__))
#define dynamite_erase_range(da_ptr, ...)                                      \
    ((da_ptr)->erase_range((da_ptr), __VA_ARGS__))
#define dynamite_pop(da_ptr) ((da_ptr)->pop((da_ptr)))
#define dynamite_find(da_ptr, ...) ((da_ptr)->find((da_ptr), __VA_ARGS__))
#define dynamite_free(da_ptr) ((da_ptr)->free(da_ptr))

#define make_dynamite(type) (CAT3(da_, type, _make)())
#define destroy_dynamite(da_ptr) (dynamite_free(da_ptr), free((da_ptr)))

#define BACK(x) x

#define STR(x) _STR(x)
#define _STR(x) #x

#define CAT2(x, y) _CAT2(x, y)
#define _CAT2(x, y) x##y

#define CAT3(x, y, z) _CAT3(x, y, z)
#define _CAT3(x, y, z) x##y##z

#define CAT4(a, b, c, d) _CAT4(a, b, c, d)
#define _CAT4(a, b, c, d) a##b##c##d

#define dynamite(type) CAT3(dynamic_, type, _array)
#define dynamite_pretty_name(type) ("dynamite(" STR(type) ")")

#define get_use(_1, _2, NAME, ...) NAME

#define use_dynamite(...)                                                      \
    get_use(__VA_ARGS__, use_dynamite_2, use_dynamite_1, )(__VA_ARGS__)

#define get_init(_1, _2, NAME, ...) NAME

#define init_dynamite(...)                                                     \
    get_init(__VA_ARGS__, init_dynamite_2, init_dynamite_1, )(__VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////

#define use_dynamite_1(type)                                                   \
    typedef struct {                                                           \
        unsigned int chunk_size;                                               \
        unsigned int size;                                                     \
        unsigned int used;                                                     \
        type * data;                                                           \
                                                                               \
        type (*at)(void * da, unsigned int pos);                               \
        type * (*begin)(void * da);                                            \
        type * (*end)(void * da);                                              \
        unsigned int (*length)(void * da);                                     \
        type * (*push)(void * da, type val);                                   \
        void (*erase)(void * da, unsigned int elem);                           \
        void (*erase_range)(void * da, unsigned int first_elem,                \
                            unsigned int last_elem);                           \
        type * (*pop)(void * da);                                              \
        type * (*find)(void * da, type val);                                   \
        void (*free)(void * da);                                               \
                                                                               \
    } dynamite(type);                                                          \
                                                                               \
    type CAT3(da_, type, _at)(void * da, unsigned int pos) {                   \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (!(pos > array->used - 1 || pos < 0))                               \
            return array->data[pos];                                           \
                                                                               \
        fprintf(stderr,                                                        \
                "dynamite: element %d in %s is out of bounds! Aborting.\n",    \
                pos, dynamite_pretty_name(type));                              \
                                                                               \
        array->free(array);                                                    \
                                                                               \
        exit(EXIT_FAILURE);                                                    \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _begin)(void * da) {                                \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        return array->data;                                                    \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _end)(void * da) {                                  \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        return &array->data[array->used];                                      \
    }                                                                          \
                                                                               \
    unsigned int CAT3(da_, type, _length)(void * da) {                         \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        return array->used;                                                    \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _push)(void * da, type val) {                       \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (array->used == array->size) {                                      \
            array->size *= 2;                                                  \
            array->data =                                                      \
                (type *)realloc(array->data, array->size * sizeof(type));      \
        }                                                                      \
        array->data[array->used++] = val;                                      \
                                                                               \
        return &array->data[array->used - 1];                                  \
    }                                                                          \
                                                                               \
    void CAT3(da_, type, _erase)(void * da, unsigned int elem) {               \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (!array->used || elem > array->used - 1) {                          \
            fprintf(                                                           \
                stderr,                                                        \
                "dynamite: element %d in %s is out of bounds! Aborting.\n",    \
                elem, dynamite_pretty_name(type));                             \
                                                                               \
            array->free(array);                                                \
            exit(EXIT_FAILURE);                                                \
        } else {                                                               \
            if (elem == array->used - 1) {                                     \
                array->pop(array);                                             \
            } else {                                                           \
                for (int i = elem + 1; i <= array->used; i++)                  \
                    array->data[i - 1] = array->data[i];                       \
                                                                               \
                array->used--;                                                 \
                                                                               \
                if (!array->used)                                              \
                    return;                                                    \
                else if (array->used == array->size / 2 &&                     \
                         array->size > array->chunk_size) {                    \
                    array->size /= 2;                                          \
                    array->data = (type *)realloc(array->data,                 \
                                                  array->size * sizeof(type)); \
                }                                                              \
            }                                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    void CAT3(da_, type, _erase_range)(void * da, unsigned int first_elem,     \
                                       unsigned int last_elem) {               \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        unsigned int range_len = last_elem - first_elem + 1;                   \
                                                                               \
        if (!array->used || first_elem > array->used - 1 ||                    \
            last_elem > array->used - 1) {                                     \
            fprintf(stderr,                                                    \
                    "dynamite: element range %d-%d in %s is out of bounds! "   \
                    "Aborting.\n",                                             \
                    first_elem, last_elem, dynamite_pretty_name(type));        \
                                                                               \
            array->free(array);                                                \
            exit(EXIT_FAILURE);                                                \
        } else if (first_elem >= last_elem) {                                  \
            fprintf(                                                           \
                stderr,                                                        \
                "dynamite: element range %d-%d in %s is invalid! Aborting.\n", \
                first_elem, last_elem, dynamite_pretty_name(type));            \
                                                                               \
            array->free(array);                                                \
            exit(EXIT_FAILURE);                                                \
        } else if (last_elem != array->used - 1) {                             \
            for (int i = first_elem + range_len; i <= array->used; i++)        \
                array->data[i - range_len] = array->data[i];                   \
        }                                                                      \
                                                                               \
        array->used -= range_len;                                              \
                                                                               \
        if (!array->used)                                                      \
            return;                                                            \
        else if (array->used < array->size / 2 &&                              \
                 array->size > array->chunk_size) {                            \
            array->size /= 2;                                                  \
            array->data =                                                      \
                (type *)realloc(array->data, array->size * sizeof(type));      \
        }                                                                      \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _pop)(void * da) {                                  \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (!array->used)                                                      \
            return array->data;                                                \
        else {                                                                 \
            array->used--;                                                     \
                                                                               \
            if (array->used < array->size / 2 &&                               \
                array->size > array->chunk_size) {                             \
                array->size /= 2;                                              \
                array->data =                                                  \
                    (type *)realloc(array->data, array->size * sizeof(type));  \
            }                                                                  \
                                                                               \
            return &array->data[array->used - 1];                              \
        }                                                                      \
    }                                                                          \
                                                                               \
    /*	NOTE: doesn't need equality function.. uses '=='	*/                 \
    type * CAT3(da_, type, _find)(void * da, type val) {                       \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        for (int i = 0; i < array->used; i++)                                  \
            if (array->data[i] == val)                                         \
                return &array->data[i];                                        \
                                                                               \
        return &array->data[array->used];                                      \
    }                                                                          \
                                                                               \
    void CAT3(da_, type, _free)(void * da) {                                   \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        free(array->data);                                                     \
        array->data = NULL;                                                    \
        array->used = array->size = 0;                                         \
    }                                                                          \
                                                                               \
    dynamite(type) * CAT3(da_, type, _make)() {                                \
        dynamite(type) * new = malloc(sizeof(dynamite(type)));                 \
        *new = init_dynamite(type);                                            \
        return new;                                                            \
    }

////////////////////////////////////////////////////////////////////////////////

#define use_dynamite_2(type, func)                                             \
    typedef struct {                                                           \
        unsigned int chunk_size;                                               \
        unsigned int size;                                                     \
        unsigned int used;                                                     \
        type * data;                                                           \
                                                                               \
        type (*at)(void * da, unsigned int pos);                               \
        type * (*begin)(void * da);                                            \
        type * (*end)(void * da);                                              \
        unsigned int (*length)(void * da);                                     \
        type * (*push)(void * da, type val);                                   \
        void (*erase)(void * da, unsigned int elem);                           \
        void (*erase_range)(void * da, unsigned int first_elem,                \
                            unsigned int last_elem);                           \
        type * (*pop)(void * da);                                              \
        type * (*find)(void * da, type val);                                   \
        void (*free)(void * da);                                               \
                                                                               \
    } dynamite(type);                                                          \
                                                                               \
    type CAT3(da_, type, _at)(void * da, unsigned int pos) {                   \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (!(pos > array->used - 1 || pos < 0))                               \
            return array->data[pos];                                           \
                                                                               \
        fprintf(stderr,                                                        \
                "dynamite: element %d in %s is out of bounds! Aborting.\n",    \
                pos, dynamite_pretty_name(type));                              \
                                                                               \
        array->free(array);                                                    \
                                                                               \
        exit(EXIT_FAILURE);                                                    \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _begin)(void * da) {                                \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        return array->data;                                                    \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _end)(void * da) {                                  \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        return &array->data[array->used];                                      \
    }                                                                          \
                                                                               \
    unsigned int CAT3(da_, type, _length)(void * da) {                         \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        return array->used;                                                    \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _push)(void * da, type val) {                       \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (array->used == array->size) {                                      \
            array->size *= 2;                                                  \
            array->data =                                                      \
                (type *)realloc(array->data, array->size * sizeof(type));      \
        }                                                                      \
        array->data[array->used++] = val;                                      \
                                                                               \
        return &array->data[array->used - 1];                                  \
    }                                                                          \
                                                                               \
    void CAT3(da_, type, _erase)(void * da, unsigned int elem) {               \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (!array->used || elem > array->used - 1) {                          \
            fprintf(                                                           \
                stderr,                                                        \
                "dynamite: element %d in %s is out of bounds! Aborting.\n",    \
                elem, dynamite_pretty_name(type));                             \
                                                                               \
            array->free(array);                                                \
            exit(EXIT_FAILURE);                                                \
        } else {                                                               \
            if (elem == array->used - 1) {                                     \
                array->pop(array);                                             \
            } else {                                                           \
                for (int i = elem + 1; i <= array->used; i++)                  \
                    array->data[i - 1] = array->data[i];                       \
                                                                               \
                array->used--;                                                 \
                                                                               \
                if (!array->used)                                              \
                    return;                                                    \
                else if (array->used == array->size / 2 &&                     \
                         array->size > array->chunk_size) {                    \
                    array->size /= 2;                                          \
                    array->data = (type *)realloc(array->data,                 \
                                                  array->size * sizeof(type)); \
                }                                                              \
            }                                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    void CAT3(da_, type, _erase_range)(void * da, unsigned int first_elem,     \
                                       unsigned int last_elem) {               \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        unsigned int range_len = last_elem - first_elem + 1;                   \
                                                                               \
        if (!array->used || first_elem > array->used ||                        \
            last_elem > array->used) {                                         \
            fprintf(stderr,                                                    \
                    "dynamite: element range %d-%d in %s is out of bounds! "   \
                    "Aborting.\n",                                             \
                    first_elem, last_elem, dynamite_pretty_name(type));        \
                                                                               \
            array->free(array);                                                \
            exit(EXIT_FAILURE);                                                \
        } else if (first_elem >= last_elem) {                                  \
            fprintf(                                                           \
                stderr,                                                        \
                "dynamite: element range %d-%d in %s is invalid! Aborting.\n", \
                first_elem, last_elem, dynamite_pretty_name(type));            \
                                                                               \
            array->free(array);                                                \
            exit(EXIT_FAILURE);                                                \
        } else if (last_elem != array->used - 1) {                             \
            for (int i = first_elem + range_len; i <= array->used; i++)        \
                array->data[i - range_len] = array->data[i];                   \
        }                                                                      \
                                                                               \
        array->used -= range_len;                                              \
                                                                               \
        if (!array->used)                                                      \
            return;                                                            \
        else if (array->used < array->size / 2 &&                              \
                 array->size > array->chunk_size) {                            \
            array->size /= 2;                                                  \
            array->data =                                                      \
                (type *)realloc(array->data, array->size * sizeof(type));      \
        }                                                                      \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _pop)(void * da) {                                  \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        if (!array->used)                                                      \
            return array->data;                                                \
        else {                                                                 \
            array->used--;                                                     \
                                                                               \
            if (array->used == array->size / 2 &&                              \
                array->size > array->chunk_size) {                             \
                array->size /= 2;                                              \
                array->data =                                                  \
                    (type *)realloc(array->data, array->size * sizeof(type));  \
            }                                                                  \
                                                                               \
            return &array->data[array->used - 1];                              \
        }                                                                      \
    }                                                                          \
                                                                               \
    type * CAT3(da_, type, _find)(void * da, type val) {                       \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        for (int i = 0; i < array->used; i++)                                  \
            if (func(array->data[i], val))                                     \
                return &array->data[i];                                        \
                                                                               \
        return &array->data[array->used];                                      \
    }                                                                          \
                                                                               \
    void CAT3(da_, type, _free)(void * da) {                                   \
        dynamite(type) * array = (dynamite(type) *)da;                         \
                                                                               \
        free(array->data);                                                     \
        array->data = NULL;                                                    \
        array->used = array->size = 0;                                         \
    }                                                                          \
                                                                               \
    dynamite(type) * CAT3(da_, type, _make)() {                                \
        dynamite(type) * new = malloc(sizeof(dynamite(type)));                 \
        *new = init_dynamite(type);                                            \
        return new;                                                            \
    }

////////////////////////////////////////////////////////////////////////////////

#define init_dynamite_1(type)                                                  \
    ((dynamite(type)){                                                         \
        8,                                                                     \
        8,                                                                     \
        0,                                                                     \
        (type *)malloc(8 * sizeof(type)),                                      \
                                                                               \
        &CAT3(da_, type, _at),                                                 \
        &CAT3(da_, type, _begin),                                              \
        &CAT3(da_, type, _end),                                                \
        &CAT3(da_, type, _length),                                             \
        &CAT3(da_, type, _push),                                               \
        &CAT3(da_, type, _erase),                                              \
        &CAT3(da_, type, _erase_range),                                        \
        &CAT3(da_, type, _pop),                                                \
        &CAT3(da_, type, _find),                                               \
        &CAT3(da_, type, _free),                                               \
    })

#define init_dynamite_2(type, c_size)                                          \
    ((dynamite(type)){                                                         \
        c_size,                                                                \
        c_size,                                                                \
        0,                                                                     \
        (type *)malloc(c_size * sizeof(type)),                                 \
                                                                               \
        &CAT3(da_, type, _at),                                                 \
        &CAT3(da_, type, _begin),                                              \
        &CAT3(da_, type, _end),                                                \
        &CAT3(da_, type, _length),                                             \
        &CAT3(da_, type, _push),                                               \
        &CAT3(da_, type, _erase),                                              \
        &CAT3(da_, type, _erase_range),                                        \
        &CAT3(da_, type, _pop),                                                \
        &CAT3(da_, type, _find),                                               \
        &CAT3(da_, type, _free),                                               \
    })

#endif
