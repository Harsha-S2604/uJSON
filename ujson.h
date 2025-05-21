#ifndef UJSON_H
#define UJSON_H

typedef enum {
    STRING,
    INTEGER,
    FLOAT,
    ARRAY_OF_STRINGS,
    ARRAY_OF_INTEGERS,
    ARRAY_OF_FLOATS,
} UJSON_DATA_TYPES;

typedef struct JSON {
    UJSON_DATA_TYPES dataType;

    char* key;
    char* strValue;

    struct JSON* next;
} JSON;

JSON* ujson_parser(char* s);
#endif
