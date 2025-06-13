#ifndef UJSON_H
#define UJSON_H

#define UJSON_NULL 0
#define UJSON_TRUE 1
#define UJSON_FALSE 2
#define UJSON_STRING 3
#define UJSON_INTEGER 4
#define UJSON_FLOAT 5
#define UJSON_ARRAY_OF_STRINGS 6
#define UJSON_ARRAY_OF_INTEGERS 7
#define UJSON_ARRAY_OF_FLOATS 8
#define UJSON_OBJECT 9

typedef struct JSON {
    int dataType;

    char* key;
    char* strValue;
    
    size_t intValue;
    double floatValue;

    struct JSON* next;
} JSON;

JSON* ujson_parser(char* s);
#endif
