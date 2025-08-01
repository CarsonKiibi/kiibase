#include "file.h"

int set_magic_number(uint8_t* header_buffer, uint32_t magic_number) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->magic_number = magic_number;
}

int set_version(uint8_t* header_buffer, uint32_t version) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->version = version;
}

int set_start_time(uint8_t* header_buffer, time_t start_time) {
   FileHeader* header = (FileHeader*)header_buffer;
   header->start_time = start_time;
}

int set_end_time(uint8_t* header_buffer, time_t end_time) {
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
                    time_t start_time, time_t end_time, uint64_t file_size, 
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

// static size_t file_cursor = 0;      // static keyword keeps this variable only available in this file
// inside a function, it will keep its value inbetween invocations
// not thread safe and can be hard to understand (for usage in functions)
// use for naive implementation

// naive insert that just appends to the end of the file 
int insert_at_end(TimeRecord *record, FILE *f) {
   if (!f) {
      return -1;
   }
   fseek(f, 0, SEEK_END);
   fwrite(record, sizeof(*record), 1, f);
   return 0;
}

int write_record_buffered(FileContext* ctx, TimeRecord* record) {
   if (!ctx || !record) return -1;

   if (ctx->cursor + sizeof(TimeRecord) > ctx->buffer_size) {
      if (ctx->autoflush) {
         flush_buffer(ctx);
      } else {
         return -2; // buffer full
      }
   }

   // buffer is a pointer so we are writing to that memory address + the cursor position!
   memcpy(ctx->buffer + ctx->cursor, record, sizeof(TimeRecord));

   if (ctx->dirty_start == SIZE_MAX) {
      ctx->dirty_start = ctx->cursor;
   }

   ctx->dirty_end = ctx->cursor + sizeof(TimeRecord);
   ctx-> cursor += sizeof(TimeRecord);

   return 0;
}

int batch_write_records_buffered(FileContext* ctx, TimeRecord* records, size_t count) {
   size_t total_size = count * sizeof(TimeRecord);

   if (ctx->cursor + total_size > ctx->buffer_size) {
      if (ctx->autoflush) {
         flush_buffer(ctx);
      } else {
         return -2; // buffer full
      }
   }

   memcpy(ctx->buffer + ctx->cursor, records, total_size);

   if (ctx->dirty_start == SIZE_MAX) {
      ctx->dirty_start = ctx->cursor;
   }

   ctx->dirty_end = ctx->cursor + total_size;
   ctx->cursor += total_size;

   return 0;
}

int flush_buffer(FileContext* ctx) {
   if (!ctx || !ctx->file_handle || !ctx->dirty_start == SIZE_MAX) {
      return 0; // nothing to flush
   }

   size_t dirty_size = ctx->dirty_end - ctx->dirty_start;

   fseek(ctx->file_handle, ctx->dirty_start, SEEK_SET);  // move file pointer dirty_start bytes from beginning of file_handle

   size_t written = fwrite(ctx->buffer + ctx->dirty_start, dirty_size, 1, ctx->file_handle);

   if (written != dirty_size) {
      return -1;  // failed to write whole dirty contents
   }

   ctx->dirty_start = SIZE_MAX;
   ctx->dirty_end = 0;

   return 0;
}

FileContext* create_file_context(size_t buffer_size) {
    FileContext* ctx = malloc(sizeof(FileContext));
    if (!ctx) return NULL;
    
    ctx->buffer = malloc(buffer_size);
    if (!ctx->buffer) {
        free(ctx);
        return NULL;
    }
    
    ctx->buffer_size = buffer_size;
    ctx->cursor = 0;
    ctx->file_handle = NULL;
    return ctx;
}

void destroy_file_context(FileContext* ctx) {
    if (ctx) {
        free(ctx->buffer);
        if (ctx->file_handle) {
            fclose(ctx->file_handle);
        }
        free(ctx);
    }
}











