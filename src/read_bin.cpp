#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "read_bin.h"
#include "VM.h"
#include "codedump.h"
#include "codector.h"
#include <assert.h>

typedef struct stat Stat;

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

    CODE_DUMP(codearr);

    close(fd);

    return codearr;
}

static ssize_t fsize(char *filename) {
    assert(filename);

    Stat st = {};

    if (stat(filename, &st) == -1) return -1;

    return st.st_size;
}
