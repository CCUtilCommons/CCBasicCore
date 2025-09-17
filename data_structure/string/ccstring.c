#include "ccstring.h"
#include "memory_allocation/memory_allocation.h"
#include "memory_allocation/memory_allocation_tools.h"
#include <alloca.h>
#include <stddef.h>
#include <string.h>
static inline char* _get_data(CCString* string) {
#ifdef USE_STRING_SSO
	return string->is_heap_flag ? string->buffer_ptr.buffer_ptr : string->buffer_ptr.sso_buffer;
#else
	return string->buffer_ptr;
#endif
}

static inline const char* _get_const_data(const CCString* string) {
#ifdef USE_STRING_SSO
	return string->is_heap_flag ? string->buffer_ptr.buffer_ptr : string->buffer_ptr.sso_buffer;
#else
	return string->buffer_ptr;
#endif
}

static inline void _init_datas(CCString* string) {
#ifdef USE_STRING_SSO
	string->buffer_ptr.sso_buffer[0] = '\0';
	string->capacity = STRING_SSO_SIZE;
	string->is_heap_flag = 0; // NOT IN Heap
#else
	string->buffer_ptr = allocate_memory(1);
	string->buffer_ptr[0] = '\0';
	string->capacity = 0;
#endif
}

#ifdef USE_STRING_SSO

static inline int _canBeInSSOBuffer(const int stringLen) {
	return stringLen <= STRING_SSO_SIZE;
}

static void _assigned_sso_strings(
    CCString* string, const char* str, const int strLen) {
	// Pass the check and we assigned in SSO Array
	if (string->is_heap_flag) {
		// OK, we need to release that
		free_memory(string->buffer_ptr.buffer_ptr);
	}
	string->is_heap_flag = 0; // Not in head
	string->capacity = STRING_SSO_SIZE;
	string->size = strLen;
	memcpy(string->buffer_ptr.sso_buffer, str, strLen + 1);
	string->buffer_ptr.sso_buffer[strLen] = '\0'; // Force NULL Terminate
}

static void __assigned_heap_with_sso_string(
    CCString* string, const char* str, const int strLen) {
	string->size = strLen;
	string->capacity = strLen;
	char* dataPtr = _get_data(string);
	if (string->is_heap_flag) {
		free_memory(dataPtr);
	} else {
		string->is_heap_flag = 1;
	}
	string->buffer_ptr.buffer_ptr = allocate_memory(string->capacity + 1);
	memcpy(string->buffer_ptr.buffer_ptr, str, strLen + 1);
}
#else
static void __assigned_heap_string(CCString* string, const char* str, const int strLen) {
	string->size = strLen;
	string->capacity = strLen;
	char* dataPtr = _get_data(string);
	if (dataPtr) {
		free_memory(dataPtr);
	}
	string->buffer_ptr = allocate_memory(string->capacity + 1);
	memcpy(string->buffer_ptr, str, strLen + 1);
}
#endif // USE_STRING_SSO
static void _reassigned_string(CCString* string, const char* str, const int strLen) {
#ifdef USE_STRING_SSO
	if (strLen < STRING_SSO_SIZE) {
		_assigned_sso_strings(string, str, strLen);
	} else {
		__assigned_heap_with_sso_string(string, str, strLen);
	}

#else
	__assigned_heap_string(string, str, strLen);
#endif
}

CCString* CCCoreBasicString_CreateEmpty() {
	CCString* string = allocate_one(CCString);
	_init_datas(string);
	string->size = 0;
	return string;
}

CCString* CCCoreBasicString_CreateFromPlainString(const char* c) {
	CCString* string = allocate_one(CCString);
	_init_datas(string);
	_reassigned_string(string, c, strlen(c));
	return string;
}

int CCCoreBasicString_FreeString(CCString* string) {
#ifdef USE_STRING_SSO
	if (string->is_heap_flag) {
		free_memory(string->buffer_ptr.buffer_ptr);
	}
#else
	free_memory(string->buffer_ptr);
#endif
	free_memory(string);
	return 0;
}

const char* CCoreBasicString_Get(const CCString* string) {
	return _get_const_data(string);
}

int CCoreBasicString_Reserve(CCString* s, size_t new_cap) {
#ifdef USE_STRING_SSO

	if (STRING_SSO_SIZE >= new_cap && !s->is_heap_flag) {
		// OK, we already in SSO and no need to switch to heap
		return 0; // Terminate the request
	}

	if (s->is_heap_flag) {
		if (new_cap <= s->capacity) {
			// Should Consider Thrink To Size
			return 0;
		}

		char* new_buffer = (char*)reallocate_memory(
		    s->buffer_ptr.buffer_ptr, new_cap + 1);
		if (!new_buffer)
			return -1;

		s->buffer_ptr.buffer_ptr = new_buffer;
		s->capacity = new_cap;
	} else {
		char temp_placed[STRING_SSO_SIZE + 1];
		memcpy(temp_placed, s->buffer_ptr.sso_buffer, s->size + 1);

		char* new_buffer = (char*)allocate_memory(s->size + 1);
		if (!new_buffer)
			return -1;
		memcpy(new_buffer, temp_placed, s->size + 1);
		s->is_heap_flag = 1;
		s->buffer_ptr.buffer_ptr = new_buffer;
		s->capacity = s->size;
	}
#else
	if (new_cap <= s->capacity) {
		// Should Consider Thrink To Size
		return 0;
	}
	char* new_buffer = (char*)reallocate_memory(
	    s->buffer_ptr, new_cap + 1);
	if (!new_buffer)
		return -1;
	s->buffer_ptr = new_buffer;
	s->capacity = new_cap;
#endif

	return 1;
}

int CCoreBasicString_ShrinkToFit(CCString* s) {
#ifdef USE_STRING_SSO
	if (!s->is_heap_flag) {
		return 0; // SSO No need to Process Fits
	}
	if (s->size <= STRING_SSO_SIZE) {
		char tmp_buffer[STRING_SSO_SIZE + 1];
		memcpy(tmp_buffer, s->buffer_ptr.buffer_ptr, s->size + 1);
		free_memory(s->buffer_ptr.buffer_ptr);
		s->is_heap_flag = 0;
		s->capacity = STRING_SSO_SIZE;
		memcpy(s->buffer_ptr.sso_buffer, tmp_buffer, s->size + 1);
		return 0;
	}
	char* p = (char*)reallocate_memory(
	    s->buffer_ptr.buffer_ptr, s->size + 1);
	if (!p)
		return -1;
	s->buffer_ptr.buffer_ptr = p;
	s->capacity = s->size;
	return 0;
#else
	char* p = (char*)reallocate_memory(
	    s->buffer_ptr.buffer_ptr, s->size + 1);
	if (!p)
		return -1;
	s->buffer_ptr.buffer_ptr = p;
	s->capacity = s->size;
	return 0;
#endif
}

int CCoreBasicString_Append(CCString* s, const char* cstr) {
	size_t src_len = strlen(cstr);
	size_t new_size = s->size + src_len;

#ifdef USE_STRING_SSO
	if (new_size < STRING_SSO_SIZE && !s->is_heap_flag) {
		memcpy(s->buffer_ptr.sso_buffer + s->size, cstr, src_len + 1);
		s->size = new_size;
		return 0;
	}

	if (!s->is_heap_flag) {
		// OK, we need to switch to heap mode
		size_t new_cap = (new_size > STRING_SSO_SIZE * 2 ? new_size : STRING_SSO_SIZE * 2);
		char* p = (char*)allocate_memory(new_cap + 1);
		if (!p)
			return -1;
		memcpy(p, s->buffer_ptr.sso_buffer, s->size);
		memcpy(p + s->size, cstr, src_len + 1);
		s->buffer_ptr.buffer_ptr = p;
		s->size = new_size;
		s->capacity = new_cap;
		s->is_heap_flag = 1;
		return 0;
	}
	char* op_ptr = s->buffer_ptr.buffer_ptr;
#else
	char* op_ptr = s->buffer_ptr;
#endif
	if (new_size > s->capacity) {
		size_t new_cap = s->capacity * 2;
		if (new_cap < new_size)
			new_cap = new_size;
		char* p = (char*)reallocate_memory(op_ptr, new_cap + 1);
		if (!p)
			return -1;
		op_ptr = p;
		s->capacity = new_cap;
	}
	memcpy(op_ptr + s->size, cstr, src_len + 1);
	s->size = new_size;
#ifdef USE_STRING_SSO
	s->buffer_ptr.buffer_ptr = op_ptr;
#else
	s->buffer_ptr = op_ptr;
#endif
	return 0;
}

int CCoreBasicString_PushBack(CCString* s, char c) {
	char tmp_buffer[2];
	tmp_buffer[0] = c;
	tmp_buffer[1] = '\0';
	return CCoreBasicString_Append(s, tmp_buffer);
}

int CCoreBasicString_Insert(CCString* s, size_t pos, const char* cstr) {
	if (s->size < pos) {
		return -1;
	}

	size_t src_len = strlen(cstr);
	size_t new_size = s->size + src_len;

#ifdef USE_STRING_SSO
	if (new_size < STRING_SSO_SIZE && !s->is_heap_flag) {
		memmove(s->buffer_ptr.sso_buffer + pos + src_len,
		        s->buffer_ptr.sso_buffer + pos,
		        s->size - pos + 1); /* +1 含 '\0' */
		memcpy(s->buffer_ptr.sso_buffer + pos, cstr, src_len);
		s->size = new_size;
		return 0;
		memcpy(s->buffer_ptr.sso_buffer + s->size, cstr, src_len);
		s->size = new_size;
		return 0;
	}

	if (!s->is_heap_flag) {
		/* 切换到 heap */
		size_t new_cap = (new_size > STRING_SSO_SIZE * 2 ? new_size : STRING_SSO_SIZE * 2);
		char* p = (char*)allocate_memory(new_cap + 1);
		if (!p)
			return -1;
		memcpy(p, s->buffer_ptr.sso_buffer, pos);
		memcpy(p + pos, cstr, src_len);
		strcpy(p + pos + src_len, s->buffer_ptr.sso_buffer + pos);
		s->buffer_ptr.buffer_ptr = p;
		s->size = new_size;
		s->capacity = new_cap;
		s->is_heap_flag = 1;
		return 0;
	}
	char* op_ptr = s->buffer_ptr.buffer_ptr;
#else
	char* op_ptr = s->buffer_ptr;
#endif
	if (new_size > s->capacity) {
		size_t new_cap = s->capacity * 2;
		if (new_cap < new_size)
			new_cap = new_size;
		char* p2 = (char*)reallocate_memory(op_ptr, new_cap + 1);
		if (!p2)
			return -1;
		op_ptr = p2;
		s->capacity = new_cap;
	}
	memmove(op_ptr + pos + src_len,
	        op_ptr + pos,
	        s->size - pos + 1);
	memcpy(op_ptr + pos, cstr, src_len);
	s->size = new_size;
#ifdef USE_STRING_SSO
	s->buffer_ptr.buffer_ptr = op_ptr;
#else
	s->buffer_ptr = op_ptr;
#endif
	return 0;
}

int CCoreBasicString_Erase(CCString* s, size_t pos, size_t len) {
	if (pos > s->size)
		return -1;
	if (len == 0)
		return 0;
#ifdef USE_STRING_SSO
	if (pos + len >= s->size) {
		if (s->is_heap_flag) {
			s->buffer_ptr.buffer_ptr[pos] = '\0';
		} else {
			s->buffer_ptr.sso_buffer[pos] = '\0';
		}
		s->size = pos;
	} else {
		if (s->is_heap_flag) {
			memmove(s->buffer_ptr.buffer_ptr + pos,
			        s->buffer_ptr.buffer_ptr + pos + len,
			        s->size - (pos + len) + 1);
		} else {
			memmove(s->buffer_ptr.sso_buffer + pos,
			        s->buffer_ptr.sso_buffer + pos + len,
			        s->size - (pos + len) + 1);
		}
		s->size -= len;
	}
#else
	memmove(s->buffer_ptr + pos,
	        s->buffer_ptr + pos + len,
	        s->size - (pos + len) + 1);
	s->size -= len;
#endif

	return 0;
}

CCString* CCoreBasicString_SubString(const CCString* s, size_t pos, size_t len) {

	if (pos > s->size)
		return NULL;
	if (pos + len > s->size)
		len = s->size - pos;

#ifdef USE_STRING_SSO
	if (len <= STRING_SSO_SIZE) {
		char temp[STRING_SSO_SIZE];
		memcpy(temp, s->buffer_ptr.sso_buffer + pos, len);
		temp[len] = '\0';
		return CCCoreBasicString_CreateFromPlainString(temp);
	}

	char* op_str = NULL;
	op_str = s->buffer_ptr.buffer_ptr;
#else
	op_ptr = s->buffer_ptr;
#endif
	char* string_buffer = allocate_memory(len + 1);
	if (!string_buffer)
		return NULL;
	memcpy(string_buffer, op_str, len);
	CCString* result = CCCoreBasicString_CreateFromPlainString(string_buffer);
	free_memory(string_buffer);
	return result;
}

int CCoreBasicString_Compare(const CCString* a, const CCString* b) {
	return strcmp(CCoreBasicString_Get(a), CCoreBasicString_Get(b));
}

size_t CCoreBasicString_Find(const CCString* s, const char* substr) {
	const char* buffer = CCoreBasicString_Get(s);
	const char* result = strstr(buffer, substr);
	if (!result) {
		return (size_t)-1;
	}
	return (size_t)(result - buffer);
}

int CCoreBasicString_Visit(CCString* s, VisitStringIterator it) {
	char* buffer = _get_data(s);
	for (size_t i = 0; i < s->size; i++) {
		it(buffer[i]);
	}
	return 0;
}

int CCoreBasicString_Process(CCString* s, AssignedStringIterator it) {
	char* buffer = _get_data(s);
	for (size_t i = 0; i < s->size; i++) {
		buffer[i] = it(buffer[i]);
	}
	return 0;
}

void CCoreBasicString_Clear(CCString* s) {
	char* buffer = _get_data(s);
	buffer[0] = '\0';
	s->size = 0;
}
