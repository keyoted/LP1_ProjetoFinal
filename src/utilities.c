#include "utilities.h"

char* strdup (const char *s) {
    size_t len = strlen (s);
    void *new = malloc (len);
    if (new == NULL)
        return NULL;
    return (char *) memcpy (new, s, len);
}

int vecPrintItemPredicate (char* item, int* userdata) {
    printf("   %*d   |   %s\n", 8, ++(*userdata), item);
    return 0;
}
