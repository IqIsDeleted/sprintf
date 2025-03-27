#include "my_string.h"

int main() {
    char arr[100];
    my_sprintf(arr, "digit %d", 100);
    printf("%s", arr);
    return 0;
}