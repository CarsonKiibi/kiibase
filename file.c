#include "file.h"

int set_magic_number(uint8_t* header_buffer, uint32_t magic_number) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->magic_number = magic_number;
}

int set_version(uint8_t* header_buffer, uint32_t version) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->version = version;
}

int set_start_time(uint8_t* header_buffer, uint64_t start_time) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->start_time = start_time;
}

int set_end_time(uint8_t* header_buffer, uint64_t end_time) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->end_time = end_time;
}

int set_file_size(uint8_t* header_buffer, uint64_t file_size) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->file_size = file_size;
}

int set_record_count(uint8_t* header_buffer, uint64_t record_count) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->record_count = record_count;
}

int set_num_entry_types(uint8_t* header_buffer, uint16_t num_entry_types) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->num_entry_types = num_entry_types;
}

int set_index_offset(uint8_t* header_buffer, uint64_t index_offset) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->index_offset = index_offset;
}

int set_data_offset(uint8_t* header_buffer, uint64_t data_offset) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->data_offset = data_offset;
}

int set_compression_type(uint8_t* header_buffer, uint32_t compression_type) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->compression_type = compression_type;
}

int set_header_checksum(uint8_t* header_buffer, uint32_t header_checksum) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->header_checksum = header_checksum;
}

int set_data_checksum(uint8_t* header_buffer, uint32_t data_checksum) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->data_checksum = data_checksum;
}

int set_creation_time(uint8_t* header_buffer, uint64_t creation_time) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->creation_time = creation_time;
}

int set_last_modified(uint8_t* header_buffer, uint64_t last_modified) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->last_modified = last_modified;
}

int set_series_count(uint8_t* header_buffer, uint32_t series_count) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->series_count = series_count;
}

int set_min_series_id(uint8_t* header_buffer, uint32_t min_series_id) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->min_series_id = min_series_id;
}

int set_max_series_id(uint8_t* header_buffer, uint32_t max_series_id) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->max_series_id = max_series_id;
}

// bulk setter (disguisting)
int set_file_header(uint8_t* header_buffer, uint32_t magic_number, uint32_t version, 
                    uint64_t start_time, uint64_t end_time, uint64_t file_size, 
                    uint64_t record_count, uint16_t num_entry_types, uint64_t index_offset,
                    uint64_t data_offset, uint32_t compression_type, uint32_t header_checksum, 
                    uint32_t data_checksum, uint64_t creation_time, uint64_t last_modified, 
                    uint32_t series_count, uint64_t min_series_id, uint32_t max_series_id) {
    
    FileHeader* header = (FileHeader*)header_buffer;
   
    header->magic_number = magic_number;
    header->version = version;
    header->start_time = start_time;
    header->end_time = end_time;
    header->file_size = file_size;
    header->record_count = record_count;
    header->num_entry_types = num_entry_types;
    header->index_offset = index_offset;
    header->data_offset = data_offset;
    header->compression_type = compression_type;
    header->header_checksum = header_checksum;
    header->data_checksum = data_checksum;
    header->creation_time = creation_time;
    header->last_modified = last_modified;
    header->series_count = series_count;
    header->min_series_id = min_series_id;
    header->max_series_id = max_series_id;
   
    memset(header->reserved, 0, 32);
}









