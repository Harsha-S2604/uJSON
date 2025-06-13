#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ujson.h"

typedef struct ujson_buffer {
    const unsigned char* content;
    size_t length;
    size_t idx;
} ujson_buffer;

static ujson_buffer input_buffer = {0, 0, 0};

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
}

static void ignore_whitespaces(char* json_string, size_t total_chars, size_t* idx) {
    while(*idx < total_chars && json_string[*idx] == ' ') {
        (*idx)++;
    }
}

static int braces_are_valid(char* json_string, size_t total_characters) {
    char opening_braces = json_string[0];
    char closing_braces = json_string[total_characters - 1];
    int valid_braces = opening_braces == '{' && closing_braces == '}';
    return valid_braces;
}

static char* parse_string(char* json_string, char char_to_find, size_t* index, size_t total_characters) {
    char* parsed_string = (char*)malloc(1);
    if (!parsed_string) {
        return NULL;
    }

    size_t length = 0, capacity = 2;
    int valid_parse = 0;
    while (*index < total_characters) {
        char c = json_string[*index];
        if (c == char_to_find) {
            valid_parse = 1;
            break;
        }

        parsed_string[length++] = c;
        (*index)++;

        parsed_string = realloc(parsed_string, length + 1);
        if (!parsed_string) {
            valid_parse = 0;
            break;
        }
    }
    
    if (!valid_parse) {
        free(parsed_string);
        parsed_string = NULL;
    }

    return parsed_string;
}

static UJSON_DATA_TYPES parse_data_type(char c) {
    UJSON_DATA_TYPES dataType;

    switch (c) {
        case '"':
            dataType = 0;
            break;
    }

    return dataType;
}

static void parse_value(char* json_string, char* key, size_t* start_idx, size_t total_chars, JSON* json) {

    // ignore whitespace if any
    ignore_whitespaces(json_string, total_chars, start_idx);

    UJSON_DATA_TYPES dataType = parse_data_type(json_string[*start_idx]);
 
    json -> dataType = dataType;
    json -> key = key;
    json -> next = (JSON*)malloc(sizeof(JSON));

    switch (dataType) {
        case 0: {
            char char_to_find = json_string[*start_idx];
            size_t char_start_idx = (*start_idx) + 1;
            char* value = parse_string(json_string, char_to_find, &char_start_idx, total_chars);
            json -> strValue = value;
            *start_idx = char_start_idx;
            break;
        }
        default:
            break;
    }

}

static int ujson_parse(JSON* json_value) {
    if (input_buffer.content == NULL) {
        return 0;
    }

    // parsing null
    if (strncmp((const char*)input_buffer.content, "null", 4) == 0) {
        json_value -> dataType = 0;
        input_buffer.idx += 4;
        return 1;
    }

    // parsing true
    if (strncmp((const char*)input_buffer.content, "true", 4) == 0) {
        json_value -> dataType = 1;
        input_buffer.idx += 4;
        return 1;
    }

    // parsing false
    if (strncmp((const char*)input_buffer.content, "false", 5) == 0) {
        json_value -> dataType = 2;
        input_buffer.idx += 5;
        return 1;
    }
    
    return 1;
}

/* static int ujson_parse_object(JSON** json_value) {
    JSON* json = *json_value;
    char* key = "";

    int str_idx = 1;
    int is_valid = 1;
    
    while (str_idx < total_chars - 1) {
        char c = json_string[str_idx];
        size_t start_idx = str_idx + 1;
        switch(c) {
            case ':':
                parse_value(json_string, key, &start_idx, total_chars, json);
                json = json -> next;
                
                ignore_whitespaces(json_string, total_chars, &start_idx);
                str_idx = start_idx + 1;
    
                if (json_string[str_idx] != ',' && str_idx != total_chars - 1) {
                    is_valid = 0;
                }
                break;
            case '"':
            case '\'':
                key = parse_string(json_string, c, &start_idx, total_chars);
                str_idx = start_idx;
                break;
        }

        if (!is_valid) {
            break;
        }

        str_idx++;
    }

    json -> next = NULL;

    return is_valid;
} */

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
