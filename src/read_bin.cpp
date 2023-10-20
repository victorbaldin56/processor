#include "../include/read_bin.h"
#include "../include/VM.h"
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct stat Stat;

#ifdef DEBUG
static void print_Code(const Code *code_array);
#endif

static ssize_t fsize(char *filename);

Code *read_bin(char *filename, Code *code_array) {
    assert(filename);

	code_array->size = fsize(filename) / (ssize_t)sizeof(double);

	if (code_array->size == -1) {
		return NULL;
    }

    code_array->code = (double *)calloc((size_t)code_array->size, sizeof(double));

    if (!code_array->code) {
        return NULL;
    }

	int fd = open(filename, O_RDONLY, 0);

    for (ssize_t i = 0; i < code_array->size; i++) {
        read(fd, (char *)(code_array->code + i), sizeof(double));
    }

    ON_DEBUG(printf("read_bin: parsed\n"));
    ON_DEBUG(print_Code(code_array));

    close(fd);

    return code_array;
}

static ssize_t fsize(char *filename) {
    assert(filename);

    Stat st = {};
    if (stat(filename, &st) == -1) {
		return -1;
    }

    return st.st_size;
}

#ifdef DEBUG
static void print_Code(const Code *code_array) {
    assert(code_array);
    assert(code_array->code);

    printf("size = %zd\n", code_array->size);

    for (ssize_t i = 0; i < code_array->size; i++) {
        for (size_t j = 0; j < sizeof(double); j++) {
            printf("%hhx ", ((char *)(code_array->code + i))[j]);
        }

        putchar('\n');
    }

    putchar('\n');
}
#endif
