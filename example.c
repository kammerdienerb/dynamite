// example.c
#include "dynamite.h"

#include <stdio.h>
#include <string.h>

// Must have EXACTLY ONE 'using' statement for each type that you wish to use
// with dynamite
use_dynamite(int);
// Any type whose name is not a simple identifier (float[], char*, etc.) must be
// used via a typedef
typedef char * str;
// You can provide a custom comparison function for the type so that
// dynamite_find will search the way you want it to
int str_eq(char * a, char * b) { return strcmp(a, b) == 0; }
// Comparison functions go into 'using' statements.
use_dynamite(str, str_eq);

void da_print_all_int(dynamite(int) * da) {
    for (int i = 0; i < dynamite_length(da); i++)
        printf("%d\t%p:\t%d\n", i, dynamite_begin(da) + i,
               *(dynamite_begin(da) + i));
}

void da_print_all_str(dynamite(str) * da) {
    for (int i = 0; i < dynamite_length(da); i++)
        printf("%d\t%p:\t%s\n", i, dynamite_begin(da) + i,
               *(dynamite_begin(da) + i));
}

int main(int argc, char ** argv) {
    // make_dynamite(T) returns a pointer to dynamite(T) for some type T
    dynamite(int) * ints = make_dynamite(int);

    for (int i = 0; i < 10; i++)
        dynamite_push(ints, i);

    printf("\n");
    da_print_all_int(ints);

    printf("\nPopping last element.\n\n");
    dynamite_pop(ints);
    da_print_all_int(ints);

    printf("\nErasing element 1.\n\n");
    dynamite_erase(ints, 1);
    da_print_all_int(ints);

    printf("\nErasing elements 6-7\n\n");
    dynamite_erase_range(ints, 6, 7);
    da_print_all_int(ints);

    printf("\nFinal length: %d\n\n", dynamite_length(ints));

    // Bounds checking -- will exit with an error message
    // dynamite_at(ints, 100);

    printf("\n");

    dynamite(str) * words = make_dynamite(str);

    dynamite_push(words, argv[0]);
    dynamite_push(words, "fire in the hole!");
    dynamite_push(words, "Easy-peasy lemon squeezy");
    dynamite_push(words, "orange");
    dynamite_push(words, "banana");
    dynamite_push(words, "apple");
    dynamite_push(words, "MANGO");
    printf("\nwords:\n\n");
    da_print_all_str(words);

    printf("\nSearching for 'plumb'\n\n");
    char ** plumb = dynamite_find(words, "plumb");
    if (plumb == dynamite_end(words))
        printf("\tDid not find 'plumb'.\n\n");

    printf("\nSearching for 'banana'\n\n");
    char ** banana = dynamite_find(words, "banana");
    if (banana != dynamite_end(words))
        printf("\tFound '%s' at %p!\n\n", *banana, banana);

    printf("\nErasing non-fruit elements (0-2)\n\n");
    dynamite_erase_range(words, 0, 2);
    da_print_all_str(words);

    // C++ish style begin/end loop
    printf("\nFruit name lengths\n\n");
    for (char ** it = dynamite_begin(words); it != dynamite_end(words); it++)
        printf("%-6s is %d characters long\n", *it, strlen(*it));

    // Calls to make_dynamite() should always be cleaned up with
    // destroy_dynamite()
    destroy_dynamite(words);
    destroy_dynamite(ints);

    return 0;
}
