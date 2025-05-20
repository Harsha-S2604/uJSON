#ifndef UJSON_H
#define UJSON_H

enum DataTypes {
    STRING,
    INTEGER,
    FLOAT,
    ARRAY_OF_STRINGS,
    ARRAY_OF_INTEGERS,
    ARRAY_OF_FLOATS,
};

typedef struct {
    enum DataTypes dataType;
    char* key;
    char* strValue;
} JSON;

JSON ujson_parser(char* s);
#endif
