#include <stdio.h>

#define STRBLD_IMPLEMENTATION
#include "../strbld.h"

int main() {
    Strbld* bld = strbld();
    strbld_add(bld, "Hllo, Wld! From string builder!!!");
    strbld_addc(bld, '\n');

    strbld_add(bld, "Version ");
    strbld_addf(bld, "%d.%d.%d", 2, 3, 1);
    strbld_addc(bld, '\n');

    strbld_add(bld, "Bit version: ");
    strbld_addbits(bld, 231, 8);

    strbld_insert(bld, 0, "|TEST|");

    printf("%s", bld->str);

    strbld_free(bld);
}

// OUTPUT:
