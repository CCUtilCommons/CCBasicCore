#ifndef CCTHREAD_DEFS_H
#define CCTHREAD_DEFS_H
#include <stddef.h>
#include <stdint.h>

#define CCThread_NameBufferLen (20)
typedef void* CCThread_t;
typedef void* CCThreadArg_t;
typedef size_t CCThreadID_t;
typedef void* (*CCThreadTask)(CCThreadArg_t args);

typedef struct CCThreadSettings {
	char name_buffer[CCThread_NameBufferLen];
	int priority;
	size_t stack_size;
} CCThreadSettings;

#endif