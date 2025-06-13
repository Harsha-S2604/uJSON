#include <stdio.h>
#include <ujson/ujson.h>

void print_value(JSON* json) {
    while(json != NULL) {
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
            default:
                break;
        }

        json = json -> next;
    }

}
int main() {
    JSON* null_json_value = ujson_parser("null");
    JSON* true_json_value = ujson_parser("true");
    JSON* false_json_value = ujson_parser("false");
    
    print_value(null_json_value);
    print_value(true_json_value);
    print_value(false_json_value);
}
