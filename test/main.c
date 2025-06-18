#include <stdio.h>
#include <ujson/ujson.h>

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
            case 0:
                printf("null\n");
                break;
            case 1:
                printf("true\n");
                break;
            case 2:
                printf("false\n");
                break;
            case 3:
                printf("%s\n", json -> strValue);
                break;
            case 4:
                printf("%zu\n", json -> intValue);
                break;
            case 5:
                printf("%f\n", json -> floatValue);
                break;
            case 6:
                print_array(json -> next);
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
    JSON* arr_json_value = ujson_parser("[\"hello\", \"hi\", 123, null, true, false]");
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
    print_value(arr_json_value);
}
