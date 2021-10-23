// This examples demonstrates multiple interface inheritance.

#include <interface99.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define Read_IFACE iMethod(size_t, read, void *self, char *dest, size_t bytes_to_read)
interface(Read);

#define Write_IFACE iMethod(size_t, write, void *self, const char *src, size_t bytes_to_write)
interface(Write);

typedef struct {
    FILE *fp;
} File;

size_t File_read(void *self, char *dest, size_t bytes_to_read) {
    File *this = (File *)self;
    return fread(dest, 1, bytes_to_read, this->fp);
}

impl(Read, File);

size_t File_write(void *self, const char *src, size_t bytes_to_write) {
    File *this = (File *)self;
    return fwrite(src, 1, bytes_to_write, this->fp);
}

impl(Write, File);

/*
 * Output:
 * We have read: 'hello world'
 */
int main(void) {
    FILE *fp = tmpfile();
    assert(fp);

    Write w = DYN(File, Write, &(File){fp});
    VCALL(w, write, "hello world", strlen("hello world"));
    rewind(fp);

    Read r = DYN(File, Read, &(File){fp});
    char hello_world[16] = {0};
    VCALL(r, read, hello_world, strlen("hello world"));

    printf("We have read: '%s'\n", hello_world);
    fclose(fp);

    return 0;
}
