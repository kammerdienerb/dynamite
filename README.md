# Dynamite

_Dynamite_ is a header-only generic dynamic array implementation in C. It utilizes (abuses?) the C preprocessor to be provide a level of generality near C++ templates like the STL’s std::vector.

## Using ##
To use _Dynamite_, you must first `#include "dynamite.h"` in your source code. For every type that you wish to hold in a dynamite array, EXACTLY ONE 'using' statement is required:

```C
// main.c
#include "dynamite.h"

use_dynamite(int);

...
```
If the type to be contained is referenced with anything but a bare identifier (e.g. `char *`, `float[10]`), it must first be wrapped in a typedef:

```C
// main.c
#include "dynamite.h"

typedef char * str;

use_dynamite(str);

...
```

You may also specify an equality function to be used in `dynamite_find()`:

```C
// main.c
#include "dynamite.h"
#include <string.h>

typedef char * str;
int str_eq(str a, str b) { return strcmp(a, b) == 0; }

use_dynamite(str, str_eq);

...
```

After that, you may use any of the following macro-like functions to create, destroy, and operate on dynamite arrays.
`make_dynamite(type)`                                           - Allocates and initializes a dynamite array. Returns a pointer to the new array.

`destroy_dynamite(da_ptr)`                                      - free()s the array's internal storage and `da_ptr`

`dynamite_at(da_ptr, int index)`                                - Bounds checked access. Returns the element at index `index`

`dynamite_begin(da_ptr)`                                        - Returns a pointer to the first element.

`dynamite_end(da_ptr)`                                          - Returns 1 + the pointer to the last element.

`dynamite_length(da_ptr)`                                       - Returns the number of elements in the array.

`dynamite_push(da_ptr, type item)`                              - Adds an element to the back of the array.

`dynamite_erase(da_ptr, int index)`                             - Removes the element at index `index`

`dynamite_erase_range(da_ptr, int first_index, int last_index)` - Removes all elements in the range `first_index` to 
`last_index`

`dynamite_pop(da_ptr)`                                          - Removes that last element in the array and returns a pointer to the new last element.

`dynamite_find(da_ptr, type val)`                               - Searches for an element equal to `val`. If found, returns a pointer to the element. Otherwise returns `dynamite_end()`

`dynamite_free(da_ptr)`                                         - free()s the array's internal storage and resets the array.


## Download ##
You can clone the project with [Git](http://git-scm.com "Git") by running:

    $ git clone git://github.com/kammerdienerb/dynamite
