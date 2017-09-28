# Dynamite

_Dynamite_ is a header-only generic dynamic array implementation in C. It utilizes (abuses?) the C preprocessor to be provide a level of generality near C++ templates like the STL’s std::vector.

## Quick Start ##
### Using ###
To use _Dynamite_, you must first `#include "dynamite.h"` in your source code. For every type that you wish to hold in a dynamite array, a EXACTLY ONE 'using' statement is required:

```
// main.c
#include "dynamite.h"

use_dynamite(int);

...
```
If the type to be contained is referenced with anything but a bare identifier (e.g. `const char *`, `float[10]`), it must first be wrapped in a typedef:

```
// main.c
#include "dynamite.h"

typedef const char * str;

use_dynamite(str);

...
```

You may also specify an equality function to be used in `dynamite_find`:

```
// main.c
#include "dynamite.h"
#include <string.h>

typedef const char * str;
int str_eq(str a, str b) { return strcmp(a, b) == 0; }

use_dynamite(str, str_eq);

...
```


## Download ##
You can clone the project with [Git](http://git-scm.com "Git") by running:

    $ git clone git://github.com/kammerdienerb/dynamite
