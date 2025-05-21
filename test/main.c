#include <stdio.h>
#include <ujson/ujson.h>

int main() {
    JSON* json_value = ujson_parser("{\"name\": \"Jack\"}");
    JSON* json = json_value;

    while(json) {
        switch (json -> dataType) {
            case 0:
                printf("The key is %s\n", json -> key);
                printf("The value is %s\n", json -> strValue);
                break;
            default:
                break;
        }

        json = json -> next;
    }
}
