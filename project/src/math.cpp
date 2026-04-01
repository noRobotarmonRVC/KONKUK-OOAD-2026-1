#include "my_math.h"
#include <cstdlib>

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

// clang-tidy 경고 테스트용 코드
int* makeArray(int size) {
    int* arr = (int*)malloc(sizeof(int) * size); // C-style cast 경고
    int x = NULL;                                // modernize-use-nullptr 경고
    return arr;
}