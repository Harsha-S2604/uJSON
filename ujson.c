#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ujson.h"

int braces_are_valid(char* json_string, size_t total_characters) {
    char opening_braces = json_string[0];
    char closing_braces = json_string[total_characters - 1];
    int valid_braces = opening_braces == '{' && closing_braces == '}';
    return valid_braces;
}

char* parse_string(char* json_string, char char_to_find, size_t* index, size_t total_characters) {
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

UJSON_DATA_TYPES parse_data_type(char c) {
    UJSON_DATA_TYPES dataType;

    switch (c) {
        case '"':
            dataType = 0;
            break;
    }

    return dataType;
}

void parse_value(char* json_string, char* key, size_t* start_idx, size_t total_chars, JSON* json) {

    // ignore whitespace if any
    while(json_string[*start_idx] == ' ') {
        (*start_idx)++;
    }

    UJSON_DATA_TYPES dataType = parse_data_type(json_string[*start_idx]);
 
    json -> dataType = dataType;
    json -> key = key;

    switch (dataType) {
        case 0: {
            char char_to_find = json_string[*start_idx];
            size_t char_start_idx = (*start_idx) + 1;
            char* value = parse_string(json_string, char_to_find, &char_start_idx, total_chars);
            json -> strValue = value;
            break;
        }
        default:
            break;
    }

}

void ujson_parse(char* json_string, size_t total_chars, JSON** json_value) {
    JSON* json = *json_value;
    char* key = "";

    int str_idx = 1;
    
    while (str_idx < total_chars - 1) {
        char c = json_string[str_idx];
        size_t start_idx = str_idx + 1;
        switch(c) {
            case ':':
                parse_value(json_string, key, &start_idx, total_chars, json);
                json = json -> next;
                break;
            case '"':
            case '\'':
                key = parse_string(json_string, c, &start_idx, total_chars);
                str_idx = start_idx;
                break;
        }

        str_idx++;
    }
}

JSON* ujson_parser(char* json_string) {
    JSON* json_value = malloc(sizeof(JSON));

    size_t total_characters = strlen(json_string);

    int valid_braces = braces_are_valid(json_string, total_characters);
    if (!valid_braces) {
        printf("[PARSING FAILED]:: invalid syntax\n");
    } else {
        ujson_parse(json_string, total_characters, &json_value);
    }

    return json_value;
}
