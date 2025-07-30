#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_SIZE 2097152 // about 2 MB

typedef struct {
    char key[16];               // 16 byte
    uint64_t timestamp;         // 8 byte
    uint32_t value_length;      // 4 byte
    uint8_t flags;              // 2 byte
    uint8_t value_data;         // 2 byte
} TimeRecord ;

typedef struct {
    uint32_t magic_number;
    uint32_t version;
    uint64_t start_time;

    uint64_t end_time;
    uint64_t file_size;
    uint64_t record_count;
    uint16_t num_entry_types;

    uint64_t index_offset;
    uint64_t data_offset;
    uint32_t compression_type;

    uint32_t header_checksum;
    uint32_t data_checksum;
    uint64_t creation_time;
    uint64_t last_modified;

    uint32_t series_count;
    uint32_t min_series_id;
    uint32_t max_series_id;

    uint8_t reserved[32];
} FileHeader ;







