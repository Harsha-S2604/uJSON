#ifndef UJSON_H
#define UJSON_H

#define UJSON_NULL 0
#define UJSON_TRUE 1
#define UJSON_FALSE 2
#define UJSON_STRING 3
#define UJSON_INTEGER 4
#define UJSON_FLOAT 5
#define UJSON_ARRAY 6
#define UJSON_OBJECT 7

typedef struct JSON {
    int dataType;

    unsigned char* key;
    unsigned char* strValue;
    
    size_t intValue;
    double floatValue;

    struct JSON* next;
} JSON;

JSON* ujson_parser(char* s);
#endif
