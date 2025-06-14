#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ujson.h"

typedef struct ujson_buffer {
    const unsigned char* content;
    size_t length;
    size_t idx;
} ujson_buffer;

#define buffer_at_idx(buffer) ((buffer).content[(buffer).idx]) 
#define buffer_at_offset(buffer) (buffer.content + buffer.idx)

ujson_buffer input_buffer = {0, 0, 0};

static JSON* create_new_item() {
    JSON* item = malloc(sizeof(JSON));
    if (item) {
        memset(item, '\0', sizeof(JSON));
    }

    return item;
}

static void init_buffer(char* content) {
    size_t total_characters = strlen(content);

    input_buffer.content = (const unsigned char*)content;
    input_buffer.length = total_characters;
    input_buffer.idx = 0;
}

static void ignore_whitespaces() {
    char character = buffer_at_idx(input_buffer);
    while(character == ' ') {
       input_buffer.idx += 1;
       character = buffer_at_idx(input_buffer);
    }
}

static int braces_are_valid(char* json_string, size_t total_characters) {
    char opening_braces = json_string[0];
    char closing_braces = json_string[total_characters - 1];
    int valid_braces = opening_braces == '{' && closing_braces == '}';
    return valid_braces;
}

static int get_data_type(char c) {
    int dataType;

    switch (c) {
        case '"':
            dataType = UJSON_STRING;
            break;
        case '{':
            dataType = UJSON_OBJECT;
            break;
        default:
            if (c >= '0' && c <= '9') {
                dataType = UJSON_INTEGER;
            }
            break;
    }

    return dataType;
}

static int parse_number(JSON* json_value) {
    int integerValue = 1;
    char number = buffer_at_idx(input_buffer);

    while (number != '\0') {
        if (number == '.') {
            integerValue = 0;
        } else if (!(number >= '0' && number <= '9')) {
            return 0;
        }
        
        number = buffer_at_idx(input_buffer);
        input_buffer.idx += 1;
    }

    if (integerValue) {
        size_t intValue = atof((const char*)input_buffer.content);
        json_value -> dataType = UJSON_INTEGER;
        json_value -> intValue = intValue;
    } else {
        double floatValue = atof((const char*)input_buffer.content);
        json_value -> dataType = UJSON_FLOAT;
        json_value -> floatValue = floatValue;
    }

    return 1;
}

static int parse_string(JSON* json_value) {
    printf("%s PARSE STRING\n", input_buffer.content);
    return 1;
}

static int ujson_parse(JSON* json_value) {
    if (input_buffer.content == NULL) {
        return 0;
    }

    ignore_whitespaces();
    const char* offset_buffer = (const char*)buffer_at_offset(input_buffer);

    // parsing null
    if (strncmp(offset_buffer, "null", 4) == 0) {
        json_value -> dataType = UJSON_NULL;
        input_buffer.idx += 4;
        return 1;
    }

    // parsing true
    if (strncmp(offset_buffer, "true", 4) == 0) {
        json_value -> dataType = UJSON_TRUE;
        input_buffer.idx += 4;
        return 1;
    }

    // parsing false
    if (strncmp(offset_buffer, "false", 5) == 0) {
        json_value -> dataType = UJSON_FALSE;
        input_buffer.idx += 5;
        return 1;
    }

    // parsing number
    char char_at_idx = buffer_at_idx(input_buffer);
    if (char_at_idx >= '0' && char_at_idx <= '9') {
        parse_number(json_value);
        return 1;
    }

    // parsing string
    if (char_at_idx == '"') {
        parse_string(json_value);
        return 1;
    }

    return 0;
}

JSON* ujson_parser(char* json_string) {
    JSON* json_value = create_new_item();
    if (json_value == NULL) {
        printf("[PARSING ERROR]:: Failed to allocate memory");
        return NULL;
    }

    init_buffer(json_string);
        
    int valid_json = ujson_parse(json_value);
    if (!valid_json) {
       printf("[PARSING ERROR]:: invalid syntax - expected value\n");
       return NULL;
    }

    return json_value;
}
