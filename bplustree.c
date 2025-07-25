#include <bplustree.h>

typedef enum {
    VALUE_INT,
    VALUE_DOUBLE,
    VALUE_BOOL,
    VALUE_STRING
} ValueType;

typedef struct {
    ValueType valueType;
    union {
        int64_t i64;
        double f64;
        bool b;
        struct {
            char *str;
            uint64_t len;
        };
    };
} Value;

typedef struct {
    int key;
    Value value;
} Entry ;

typedef struct {

    Node* parent;
} Node ;