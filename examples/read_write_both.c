// This examples demonstrates dynamic objects of multiple interfaces.

#include <interface99.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define Read_IFACE vfunc(size_t, read, VSelf, char *dest, size_t bytes_to_read)
interface(Read);

#define Write_IFACE vfunc(size_t, write, VSelf, const char *src, size_t bytes_to_write)
interface(Write);

#define ReadWrite_IFACE
#define ReadWrite_EXTENDS (Read, Write)
interface(ReadWrite);

typedef struct {
    FILE *fp;
} File;

size_t File_read(VSelf, char *dest, size_t bytes_to_read) {
    VSELF(File);
    return fread(dest, 1, bytes_to_read, self->fp);
}

impl(Read, File);

size_t File_write(VSelf, const char *src, size_t bytes_to_write) {
    VSELF(File);
    return fwrite(src, 1, bytes_to_write, self->fp);
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

    VCALL_SUPER(rw, Write, write, "hello world", strlen("hello world"));
    rewind(fp);

    char hello_world[16] = {0};
    VCALL_SUPER(rw, Read, read, hello_world, strlen("hello world"));

    printf("We have read: '%s'\n", hello_world);
    fclose(fp);

    return 0;
}
