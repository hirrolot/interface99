// This examples demonstrates dynamic objects of multiple interfaces.

#include <interface99.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define Read_INTERFACE(FN, CTX) FN(CTX, size_t, read, void *self, char *dest, size_t bytes_to_read)
interface(Read);

#define Write_INTERFACE(FN, CTX)                                                                   \
    FN(CTX, size_t, write, void *self, const char *src, size_t bytes_to_write)
interface(Write);

#define ReadWrite_INTERFACE
#define ReadWrite_EXTENDS (Read, Write)
interface(ReadWrite);

typedef struct {
    FILE *fp;
} File;

size_t File_Read_read(void *self, char *dest, size_t bytes_to_read) {
    File *this = (File *)self;
    return fread(dest, 1, bytes_to_read, this->fp);
}

impl(Read, File);

size_t File_Write_write(void *self, const char *src, size_t bytes_to_write) {
    File *this = (File *)self;
    return fwrite(src, 1, bytes_to_write, this->fp);
}

impl(Write, File);

// `Read` and `Write` are already implemented.
impl(ReadWrite, File);

/*
 * Output:
 * We have read: 'hello world'
 */
int main(void) {
    FILE *fp = tmpfile();
    assert(fp);

    ReadWrite rw = DYN(File, ReadWrite, &(File){fp});

    rw.vptr->Write->write(rw.self, "hello world", strlen("hello world"));
    rewind(fp);

    char hello_world[16] = {0};
    rw.vptr->Read->read(rw.self, hello_world, strlen("hello world"));

    printf("We have read: '%s'\n", hello_world);
    fclose(fp);

    return 0;
}
