#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "read_bin.h"
#include "VM.h"
#include "codector.h"

typedef struct stat Stat;

#ifdef DEBUG
static void print_Code(const Code *codearr);
#endif

static ssize_t fsize(char *filename);

Code *read_bin(char *filename, Code *codearr) {
    assert(codearr);
    assert(filename);

	ssize_t code_size = fsize(filename);

	if (code_size == -1) return NULL;

    CodeCtor(codearr, (size_t)code_size);

    if (!codearr->code) return NULL;

	int fd = open(filename, O_RDONLY, 0);

    read(fd, codearr->code, (size_t)code_size);

    ON_DEBUG(printf("read_bin: parsed\n"));
    ON_DEBUG(print_Code(codearr));

    close(fd);

    return codearr;
}

static ssize_t fsize(char *filename) {
    assert(filename);

    Stat st = {};

    if (stat(filename, &st) == -1) return -1;

    return st.st_size;
}

#ifdef DEBUG
static void print_Code(const Code *codearr) {
    assert(codearr);
    assert(codearr->code);

    printf("size = %zu\n", codearr->size);

    for (size_t i = 0; i < codearr->size; i++) {
        for (size_t j = 0; j < sizeof(double); j++) {
            printf("%hhx ", ((char *)(codearr->code + i))[j]);
        }

        putchar('\n');
    }

    putchar('\n');
}
#endif
