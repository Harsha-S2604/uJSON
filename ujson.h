#ifndef UJSON_H
#define UJSON_H

typedef enum {
    UJSON_NULL,
    UJSON_TRUE,
    UJSON_FALSE,
    UJSON_STRING,
    UJSON_INTEGER,
    UJSON_FLOAT,
    UJSON_ARRAY_OF_STRINGS,
    UJSON_ARRAY_OF_INTEGERS,
    UJSON_ARRAY_OF_FLOATS,
} UJSON_DATA_TYPES;

typedef struct JSON {
    UJSON_DATA_TYPES dataType;

    char* key;
    char* strValue;

    struct JSON* next;
} JSON;

JSON* ujson_parser(char* s);
#endif
