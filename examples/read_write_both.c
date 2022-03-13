// This examples demonstrates dynamic objects of multiple interfaces.

#include <interface99.h>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define Reader_IFACE vfunc(size_t, read, VSelf, char *dest, size_t bytes_to_read)
interface(Reader);

#define Writer_IFACE vfunc(size_t, write, VSelf, const char *src, size_t bytes_to_write)
interface(Writer);

#define ReadWriter_IFACE
#define ReadWriter_EXTENDS (Reader, Writer)
interface(ReadWriter);

typedef struct {
    FILE *fp;
} File;

size_t File_read(VSelf, char *dest, size_t bytes_to_read) {
    VSELF(File);
    return fread(dest, 1, bytes_to_read, self->fp);
}

impl(Reader, File);

size_t File_write(VSelf, const char *src, size_t bytes_to_write) {
    VSELF(File);
    return fwrite(src, 1, bytes_to_write, self->fp);
}

impl(Writer, File);

// `Reader` and `Writer` are already implemented.
impl(ReadWriter, File);

/*
 * Output:
 * We have read: 'hello world'
 */
int main(void) {
    FILE *fp = tmpfile();
    assert(fp);

    ReadWriter rw = DYN(File, ReadWriter, &(File){fp});

    VCALL_SUPER(rw, Writer, write, "hello world", strlen("hello world"));
    rewind(fp);

    char hello_world[16] = {0};
    VCALL_SUPER(rw, Reader, read, hello_world, strlen("hello world"));

    printf("We have read: '%s'\n", hello_world);
    fclose(fp);

    return 0;
}
