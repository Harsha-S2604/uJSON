#include <stdio.h>
#include <string.h>
#include "ujson.h"

int braces_are_valid(char* json_string, int total_characters) {
    char opening_braces = json_string[0];
    char closing_braces = json_string[total_characters - 1];
    int valid_braces = opening_braces == '{' && closing_braces == '}';
    return valid_braces;
}

JSON ujson_parser(char* json_string) {
    JSON json_value = {};

    int total_characters = strlen(json_string);

    int valid_braces = braces_are_valid(json_string, total_characters);
    if (!valid_braces) {
        printf("[PARSING FAILED]:: invalid syntax\n");
    } else {
        printf("Parse the json\n");
    }
    return json_value;
}
