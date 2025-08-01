#ifndef FILE_H
#define FILE_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define MAX_FILE_SIZE 2097152 // about 2 MB

typedef struct {
    char key[16];               // 16 byte
    time_t timestamp;         // 8 byte
    uint32_t value_length;      // 4 byte
    uint8_t flags;              // 2 byte
    uint8_t value_data;
} TimeRecord;

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
} FileHeader;

typedef struct {
   uint8_t* buffer;
   size_t buffer_size;
   size_t cursor;
   size_t dirty_start;
   size_t dirty_end;
   FILE* file_handle;
   bool autoflush;
}  FileContext ;

int set_magic_number(uint8_t* header_buffer, uint32_t magic_number);
int set_version(uint8_t* header_buffer, uint32_t version);
int set_start_time(uint8_t* header_buffer, time_t start_time);
int set_end_time(uint8_t* header_buffer, time_t end_time);
int set_file_size(uint8_t* header_buffer, uint64_t file_size);
int set_record_count(uint8_t* header_buffer, uint64_t record_count);
int set_num_entry_types(uint8_t* header_buffer, uint16_t num_entry_types);
int set_index_offset(uint8_t* header_buffer, uint64_t index_offset);
int set_data_offset(uint8_t* header_buffer, uint64_t data_offset);
int set_compression_type(uint8_t* header_buffer, uint32_t compression_type);
int set_header_checksum(uint8_t* header_buffer, uint32_t header_checksum);
int set_data_checksum(uint8_t* header_buffer, uint32_t data_checksum);
int set_creation_time(uint8_t* header_buffer, uint64_t creation_time);
int set_last_modified(uint8_t* header_buffer, uint64_t last_modified);
int set_series_count(uint8_t* header_buffer, uint32_t series_count);
int set_min_series_id(uint8_t* header_buffer, uint32_t min_series_id);
int set_max_series_id(uint8_t* header_buffer, uint32_t max_series_id);

int set_file_header(uint8_t* header_buffer, uint32_t magic_number, uint32_t version, 
                    time_t start_time, time_t end_time, uint64_t file_size, 
                    uint64_t record_count, uint16_t num_entry_types, uint64_t index_offset,
                    uint64_t data_offset, uint32_t compression_type, uint32_t header_checksum, 
                    uint32_t data_checksum, uint64_t creation_time, uint64_t last_modified, 
                    uint32_t series_count, uint64_t min_series_id, uint32_t max_series_id);

void advance_cursor(uint64_t amount);
void reset_cursor();
int insert_at_end(TimeRecord *record, FILE *f);

int write_record_buffered(FileContext* ctx, TimeRecord* record);
int batch_write_records_buffered(FileContext* ctx, TimeRecord* records, size_t count);
int flush_buffer(FileContext* ctx);

FileContext* create_file_context(size_t buffer_size);
void destroy_file_context(FileContext* ctx);

#endif // FILE_H