#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_FILENAME_LENGTH 255

size_t cw(char *arg, char *filename);

int main(int argc, char *argv[])
{
    if (argc < 2) {
        exit(EXIT_FAILURE);
    }
    size_t results[argc == 2 ? 3 : argc - 2]; // 1
    int fn_i = --argc; /* except filename */ // 2
    int r_size = sizeof(results) / sizeof(size_t); // 1

    if(r_size != (argc - 1)) { /* Default (-c, -l and -w options) */
        results[1] = cw("-l", argv[fn_i]);
        results[2] = cw("-w", argv[fn_i]);
        results[0] = cw("-c", argv[fn_i]);
    }else {
        int r_i = r_size;
        while(argc > 0 && r_i > 0) {
            results[--r_i] = cw(argv[--argc], argv[fn_i]);
        }
    }

    printf("\t");
    for(int i = 0; i < r_size; i++) {
        printf("%zu ", results[i]);
    }
    printf("%s\n", argv[fn_i]);

    exit(EXIT_SUCCESS);
}

size_t bytes_count(FILE *fp);
size_t lines_count(FILE *fp);
size_t words_count(FILE *fp);
size_t chars_count(FILE *fp);

size_t cw(char *arg, char *filename)
{
    size_t result = -1;
    FILE *fp = fopen(filename, "r");
    if (strcmp(arg, "-c") == 0) { // bytes
        result = bytes_count(fp);
    }else if (strcmp(arg, "-l") == 0) { // lines
        result = lines_count(fp);
    }else if (strcmp(arg, "-w") == 0) { // words
        result = words_count(fp);
    }else if (strcmp(arg, "-m") == 0) {
        result = chars_count(fp);
    }
    fclose(fp);
    return result;
}

size_t words_count(FILE *fp)
{
    size_t words = 0;
    unsigned c;

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while((c = getc(fp)) != EOF) {
        if(isspace(c) || !isalnum(c)) {
            long int pos;
            if((pos = ftell(fp)) != -1L && pos < len) {
                unsigned cn;
                if(!isspace((cn = getc(fp)))) {
                    words++;
                }
            }
        }
    }

    return words;
}

size_t char_bytes_count(unsigned char ch)
{
    if (ch < 128) {
        return 1;
    } else if (ch < 224) {
        return 2;
    } else if (ch < 240) {
        return 3;
    } else {
        return 4;
    }
}

size_t chars_count(FILE *fp)
{
    size_t chars = 0;
    int c, bytes = 0;
    while((c = getc(fp)) != EOF) {
        if(bytes == 0) {
            bytes = char_bytes_count((unsigned int)c) - 1;
        } else {
            bytes--;
            continue;
        }
        chars++;
    }
    return chars;
}

size_t lines_count(FILE *fp)
{
    size_t lines = 0;
    int c;
    while((c = getc(fp)) != EOF) {
        if(c == '\n') {
            lines++;
        }
    }
    return lines;
}

size_t bytes_count(FILE *fp)
{
    size_t bytes = 0;
    while (getc(fp) != EOF) {
        bytes++;
    }
    return bytes;
}
