#include <stdio.h>
#include <ujson/ujson.h>

void printJsonValue(JSON* json, int withComma);
void print_object(JSON* json);
void print_array(JSON* json);
void print_value(JSON* json);

void print_object(JSON* json) {
    printf("{\n");
    while (json) {
        printf("    %s: ", json -> key);
        print_value(json);
        json = json -> next;
    }
    printf("}\n");
}
void printJsonValue(JSON* json, int withComma) {
    switch (json->dataType) {
        case UJSON_NULL:
            printf("null");
            break;
        case UJSON_FALSE:
            printf("false");
            break;
        case UJSON_TRUE:
            printf("true");
            break;
        case UJSON_STRING:
            printf("%s", json->strValue);
            break;
        case UJSON_FLOAT:
            printf("%f", json->floatValue);
            break;
        case UJSON_INTEGER:
            printf("%zu", json->intValue);
            break;
        default:
            printf("Invalid data type\n");
            break;
    }

    if (withComma) {
        printf(", ");
    }
}

void print_array(JSON* json) {
    printf("[");
    while (json != NULL) {
        if (json -> next == NULL) {
            printJsonValue(json, 0);
        } else {
            printJsonValue(json, 1);
        }
        json = json -> next;
    }
    printf("]\n");
}

void print_value(JSON* json) {
        switch (json -> dataType) {
            case UJSON_NULL:
                printf("null\n");
                break;
            case UJSON_TRUE:
                printf("true\n");
                break;
            case UJSON_FALSE:
                printf("false\n");
                break;
            case UJSON_STRING:
                printf("%s\n", json -> strValue);
                break;
            case UJSON_INTEGER:
                printf("%zu\n", json -> intValue);
                break;
            case UJSON_FLOAT:
                printf("%f\n", json -> floatValue);
                break;
            case UJSON_ARRAY:
                print_array(json -> next);
                break;
            case UJSON_OBJECT:
                print_object(json -> next);
                break;
            default:
                break;
        }


}

int main() {
    // JSON* null_json_value = ujson_parser("null");
    // JSON* true_json_value = ujson_parser("true");
    // JSON* false_json_value = ujson_parser("false");
    // JSON* float_json_value = ujson_parser("0.25");
    // JSON* int_json_value = ujson_parser("25");
    // JSON* str_json_value = ujson_parser("\"Hello World\"");
    // JSON* str_es_json_value = ujson_parser("\"Hello W\\torld\"");
    // JSON* arr_json_value = ujson_parser("[\"hello\", \"hi\", 123, null, true, false]");
    // whitespace handling
    // JSON* null_ws_json_value = ujson_parser("   null  ");
    // JSON* num_ws_json_value = ujson_parser("   23");
    // JSON* true_ws_json_value = ujson_parser(" true");
    // JSON* false_ws_json_value = ujson_parser(" false ");

    // print_value(null_json_value);
    // print_value(true_json_value);
//     print_value(false_json_value);
//     print_value(float_json_value);
//     print_value(int_json_value);
//     print_value(str_json_value);
//     print_value(str_es_json_value);
// 
// 
//     print_value(null_ws_json_value);
//     print_value(num_ws_json_value);
// 
    JSON* obj_json_value = ujson_parser("{\"name\": \"John\", \"age\": 30, \"arr\": [1, 2, 3]}");
    // whitespace handling
    print_value(obj_json_value);
}
