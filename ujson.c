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

static JSON* ujson_parse();
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
    int is_integer = 1;
    const unsigned char* strNum = buffer_at_offset(input_buffer);
    
    size_t i_num = 0;
    double f_num = 0.0;
    size_t place = 10;

    while (*strNum == '.' || *strNum >= '0' && *strNum <= '9') {
        if (*strNum == '.') {
            f_num += i_num;
            is_integer = 0;
            place = 10;
            strNum++;
            input_buffer.idx++;
        }
        
        size_t num = *strNum - '0';
        if (is_integer) {
            i_num = ((place * i_num) + num); 
        } else {
            double dec_place = (double)num / place;
            f_num += dec_place;
            place *= 10;
        }

        strNum++;
        input_buffer.idx++;
    }
    
    if (is_integer) {
        json_value -> dataType = UJSON_INTEGER;
        json_value -> intValue = i_num;
    } else {
        json_value -> dataType = UJSON_FLOAT;
        json_value -> floatValue = f_num;
    }

    return 1;
}

static int parse_string(JSON* json_value) {
    const unsigned char* start_place = buffer_at_offset(input_buffer) + 1;
    const unsigned char* curr_place = buffer_at_offset(input_buffer) + 1;
    
    input_buffer.idx++;
    size_t skip_char = 0;
    while (*curr_place && *curr_place != '"') {
        if (curr_place[0] == '\\') {
            curr_place++;
            skip_char += 1;

            input_buffer.idx++;
        }
        curr_place++;
        input_buffer.idx++;
    }

    if (*curr_place != '"') {
        printf("[STRING PARSE ERROR]:: invalid string missing symbol \"\n");
        return 0;
    }
   
    size_t total_chars = curr_place - start_place;
    size_t total_memory_to_allocate = (curr_place - start_place) - skip_char;
    unsigned char* result_str = malloc(sizeof(char) * total_memory_to_allocate);
    if (result_str == NULL) {
        printf("[STR PARSE ERROR]:: Failed to allocate memory\n");
        return 0;
    }
    
    int idx = 0;
    while (start_place < curr_place) {
        char char_to_append;
        if (*start_place == '\\') {
          char escape_char = *(++start_place);

          switch (escape_char) {
            case 'b':
                char_to_append = '\b';
                break;
            case 't':
                char_to_append = '\t';
                break;
            case 'n':
                char_to_append = '\n';
                break;
            default:
                break;
          }
        } else {
            char_to_append = *start_place;
        }
    
        result_str[idx] = char_to_append;
        idx++;
        start_place++;
    }
    
    json_value -> dataType = UJSON_STRING;
    json_value -> strValue = (char*)result_str;
    
    input_buffer.idx++;
    return 1;
}

static int parse_array(JSON* json_value) {
    if (!json_value) {
        printf("[ARRAY PARSE ERROR]:: json object is null");
        return 0;
    }

    const char sq_open_bracket = buffer_at_idx(input_buffer);
    if (sq_open_bracket != '[') {
        printf("[ARRAY PARSE ERROR]:: syntax error missing [");
        return 0;
    }
    
    input_buffer.idx += 1;
    json_value -> dataType = UJSON_ARRAY;
    const unsigned char* character = buffer_at_offset(input_buffer);
    while (character && *character != ']') {
        JSON* json_item = ujson_parse();
        json_value -> next = json_item;
        json_value = json_value -> next;

        char end_char = buffer_at_idx(input_buffer);
        if (end_char != ']' && end_char != ',') {
            printf("[ARRAY PARSE ERROR]:: Invalid syntax\n");
            return 0;
        }
        
        if (end_char != ']') {
            input_buffer.idx += 1;
            ignore_whitespaces();
        }
        character = buffer_at_offset(input_buffer);

    }

    return 1;
}

static JSON* ujson_parse() {
    if (input_buffer.content == NULL) {
        return NULL;
    }

    JSON* json_value = create_new_item();
    if (json_value == NULL) {
        printf("[PARSING ERROR]:: Failed to allocate memory");
        return NULL;
    }

    ignore_whitespaces();
    const char* offset_buffer = (const char*)buffer_at_offset(input_buffer);

    // parsing null
    if (strncmp(offset_buffer, "null", 4) == 0) {
        json_value -> dataType = UJSON_NULL;
        input_buffer.idx += 4;
        return json_value;
    }

    // parsing true
    if (strncmp(offset_buffer, "true", 4) == 0) {
        json_value -> dataType = UJSON_TRUE;
        input_buffer.idx += 4;
        return json_value;
    }

    // parsing false
    if (strncmp(offset_buffer, "false", 5) == 0) {
        json_value -> dataType = UJSON_FALSE;
        input_buffer.idx += 5;
        return json_value;
    }

    // parsing number
    char char_at_idx = buffer_at_idx(input_buffer);
    if (char_at_idx >= '0' && char_at_idx <= '9') {
        parse_number(json_value);
        return json_value;
    }

    // parsing string
    if (char_at_idx == '"') {
        parse_string(json_value);
        return json_value;
    }

    // parsing array
    if (char_at_idx == '[') {
        parse_array(json_value);
        return json_value;
    }

    return NULL;
}

JSON* ujson_parser(char* json_string) {
    init_buffer(json_string);
        
    JSON* json_value = ujson_parse();
    if (!json_value) {
       printf("[PARSING ERROR]:: invalid syntax - expected value\n");
       return NULL;
    }

    return json_value;
}
