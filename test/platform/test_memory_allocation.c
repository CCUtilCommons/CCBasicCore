#include "memory_allocation.h"
#include <stdio.h>
#include <string.h>

int main() {
    printf("=== Memory Allocation Test Start ===\n");

    // 1. 测试 allocate_memory
    size_t size1 = 16;
    char* buffer = (char*)allocate_memory(size1);
    if (!buffer) {
        printf("allocate_memory failed for size %zu\n", size1);
        return 1;
    }
    printf("allocate_memory success: %p (size %zu)\n", (void*)buffer, size1);

    // 初始化 buffer 内容
    for (size_t i = 0; i < size1; i++) {
        buffer[i] = (char)(i + 1);
    }

    printf("Buffer content after initialization: ");
    for (size_t i = 0; i < size1; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // 2. 测试 reallocate_memory
    size_t new_size = 32;
    buffer = (char*)reallocate_memory(buffer, new_size);
    if (!buffer) {
        printf("reallocate_memory failed for new size %zu\n", new_size);
        return 1;
    }
    printf("reallocate_memory success: %p (new size %zu)\n", (void*)buffer, new_size);

    // 检查原来的数据是否还在
    printf("Buffer content after reallocation (old data check): ");
    for (size_t i = 0; i < size1; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // 填充新空间
    for (size_t i = size1; i < new_size; i++) {
        buffer[i] = (char)(i + 1);
    }

    printf("Buffer content after filling new space: ");
    for (size_t i = 0; i < new_size; i++) {
        printf("%d ", buffer[i]);
    }
    printf("\n");

    // 3. 测试 free_memory
    int ret = free_memory(buffer);
    if (ret) {
        printf("free_memory success\n");
    } else {
        printf("free_memory failed\n");
    }

    printf("=== Memory Allocation Test End ===\n");
    return 0;
}


