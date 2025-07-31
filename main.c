#include "file.h"
#include <stdio.h>
#include <string.h>

int main() {
    TimeRecord t = {"hi", 123, 123, 123, 123};
    printf("%zu\n", sizeof(t));
    
    uint8_t buffer[MAX_FILE_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int err = set_file_header(buffer, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123, 123);
    switch (err) {
        case 1:
            printf("Failed to set header");
            break;
        default:
            break;
    }

    FILE *fptr;

    fptr = fopen("db.bin", "wb");
    if (fptr) {
        // Write sizeof(FileHeader) bytes, not the entire buffer
        fwrite(buffer, sizeof(FileHeader), 1, fptr);
        fclose(fptr);
    }

    return 0;
}