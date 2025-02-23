#define _XOPEN_SOURCE
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <locale.h>

int main(int argc, char *argv[]) {
    wchar_t *buf = NULL;
    size_t buf_size = 0;
    int status = 0;
    const char *lang = getenv("LANG");

    if (lang != NULL && setlocale(LC_ALL, lang) == NULL) {
        fprintf(stderr, "Error: setting locale to \"%s\": %s", lang, strerror(errno));
        return 1;
    }

    for (int argind = 1; argind < argc; ++ argind) {
        const char *arg = argv[argind];
        size_t len = mbstowcs(NULL, arg, 0);

        if (len == (size_t)-1) {
            fprintf(stderr, "[%d] Error: illegal multi-byte string\n", argind);
            status = 1;
            continue;
        }

        if (len >= buf_size) {
            if (len == SIZE_MAX || (len + 1) > SIZE_MAX / sizeof(wchar_t)) {
                fprintf(stderr, "[%d] Error: integer overflow in size calculation, len = %zu\n", argind, len);
                status = 1;
                continue;
            }

            wchar_t *new_buf = realloc(buf, sizeof(wchar_t) * (len + 1));
            if (new_buf == NULL) {
                fprintf(stderr, "[%d] Error: allocating buffer: %s\n", argind, strerror(errno));
                status = 1;
                continue;
            }

            buf = new_buf;
            buf_size = len + 1;
        }

        mbstowcs(buf, arg, buf_size);

        buf[len] = 0;

        int width = wcswidth(buf, len);
        if (width < 0) {
            fprintf(stderr, "[%d] Error: string contains non-printable or control characters\n", argind);
            status = 1;
            continue;
        }

        printf("%d\n", width);
    }

    free(buf);

    return status;
}
