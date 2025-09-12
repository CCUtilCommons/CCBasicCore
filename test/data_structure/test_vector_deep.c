#include <string.h>
#include <stdlib.h>
#include "compares.h"
#include "utils_def.h"
#include "result_pack.h"
#include "vector.h"
#include <stdio.h>

void* string_copier(const void* elem) {
    const char* src = *(const char**)elem; // elem 是 char* 的地址
    return strdup(src); // 返回新分配的字符串
}

int string_freer(void* elem) {
    char* str = (char*)elem;
    free(str);
    return 1;
}

CompareResult string_compare(const void* a, const void* b) {
    const char* sa = (const char*)a;
    const char* sb = (const char*)b;
    return strcmp(sa, sb);
}

CCSpecialDefinedPack string_pack = {
    .copier = string_copier,
    .freer = string_freer,
    .compared = string_compare
};

void stress_test_strings() {
    CCSpecialDefinedPack string_pack = {
        .copier = string_copier,
        .freer = string_freer,
        .compared = string_compare
    };

    CCVector* v = CCBasicCoreVector_CreateDefinedVector(sizeof(char*), &string_pack);

    size_t N = 1 * 1000 * 1000; // 100 万
    char buf[32];
    for(size_t i = 0; i < N; i++) {
        sprintf(buf, "hello_%zu", i);
        char* tmp = buf;
        __CCBasicCoreVector_PushBack(v, &tmp, sizeof(char*));
    }

    printf("string push_back finished, size = %zu\n", v->current_size);

    // 缩小，触发批量 free
    CCBasicCoreVector_ResizeVector(v, N / 2);

    printf("string resize finished, size = %zu\n", v->current_size);

    CCBasicCoreVector_FreeVector(v);
}


int main() {
    // 创建一个 vector，存放 char*
    CCVector* v = CCBasicCoreVector_CreateDefinedVector(sizeof(char*), &string_pack);

    char* s1 = "hello";
    char* s2 = "world";

    __CCBasicCoreVector_PushBack(v, &s1, sizeof(char*));
    __CCBasicCoreVector_PushBack(v, &s2, sizeof(char*));

    // 取出数据
    for(size_t i = 0; i < v->current_size; i++) {
        struct ResultPack* r = CCBasicCoreVector_Get(v, i);
        char* stored = *(char**)r->result;  // data 是 char*
        printf("%s\n", stored);
    }

    // 缩小，触发 freer
    CCBasicCoreVector_ResizeVector(v, 1);

    // 销毁，触发 freer
    CCBasicCoreVector_FreeVector(v);

    printf("=========== Runs Stress Tests ===========\n");

    stress_test_strings();

    printf("=========== Runs Stress Tests OK ===========\n");
}
