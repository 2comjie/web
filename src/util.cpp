#include "util.h"

#include <stdio.h>
#include <stdlib.h>

void errif(bool condition, const char* msg) {
    if (condition) {
        printf("%s", msg);
        exit(1);
    }
}