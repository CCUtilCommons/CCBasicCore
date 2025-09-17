#ifndef CCCOREBASIC_STRING_H
#define CCCOREBASIC_STRING_H

#include <stddef.h>

/**
 * @brief When iterate words, use this to iterate the sessions
 *
 */
typedef void (*VisitStringIterator)(const int ch);

/**
 * @brief   When Iterate While Make Assignment, use this
 *          to process a String
 *
 */
typedef int (*AssignedStringIterator)(const int ch);

/**
 * @brief Simple Ascii String
 *
 */
typedef struct CCString {
	size_t size;
	size_t capacity;

#ifdef USE_STRING_SSO
#define STRING_SSO_SIZE (15)
#if STRING_SSO_SIZE < 0
#error "STRING_SSO_SIZE Can not be assigned negatives"
#endif
	union {
		char* buffer_ptr;
		char sso_buffer[STRING_SSO_SIZE + 1]; // + 1 for NULL Termination
	} buffer_ptr;
	unsigned char is_heap_flag;
#else
	char* buffer_ptr;
#endif
} CCString;

/**
 * @brief create an empty string
 *
 * @return CCString*
 */
CCString* CCCoreBasicString_CreateEmpty();

/**
 * @brief Create a String from a given C Raw String
 *
 * @param c
 * @return CCString*
 */
CCString* CCCoreBasicString_CreateFromPlainString(const char* c);

/**
 * @brief Release A CCString
 *
 * @param string string
 * @return int
 */
int CCCoreBasicString_FreeString(CCString* string);
/**
 * @brief Get a Const View Of A String
 *
 * @param string
 * @return const char*
 */
const char* CCoreBasicString_Get(const CCString* string);

/**
 * @brief Reserve Size with preallocations
 *
 * @param s
 * @param new_cap
 * @return int
 */
int CCoreBasicString_Reserve(CCString* s, size_t new_cap);

/**
 * @brief Readjust space for the strings
 *
 * @param s
 * @return int
 */
int CCoreBasicString_ShrinkToFit(CCString* s);

/**
 * @brief Append strings like a + b -> ab
 *
 * @param s
 * @param cstr
 * @return int
 */
int CCoreBasicString_Append(CCString* s, const char* cstr);

/**
 * @brief Assigned back a char into a string
 *
 * @param s
 * @param c
 * @return int
 */
int CCoreBasicString_PushBack(CCString* s, char c);

/**
 * @brief Insert a substring into CSString
 *
 * @param s
 * @param pos
 * @param cstr
 * @return int
 */
int CCoreBasicString_Insert(CCString* s, size_t pos, const char* cstr);

/**
 * @brief Erase Sub from given Position
 *
 * @param s
 * @param pos
 * @param len
 * @return int
 */
int CCoreBasicString_Erase(CCString* s, size_t pos, size_t len);

/**
 * @brief Get substring from basic string
 *
 * @param s
 * @param pos
 * @param len
 * @return CCString*
 */
CCString* CCoreBasicString_SubString(const CCString* s, size_t pos, size_t len);

/**
 * @brief Compare String
 *
 * @param a
 * @param b
 * @return int
 */
int CCoreBasicString_Compare(const CCString* a, const CCString* b);

/**
 * @brief Find String
 *
 * @param s
 * @param substr
 * @return size_t
 */
size_t CCoreBasicString_Find(const CCString* s, const char* substr);

/**
 * @brief Iterate A String with only visiting
 *
 * @param s
 * @param it
 * @return int
 */
int CCoreBasicString_Visit(CCString* s, VisitStringIterator it);

/**
 * @brief Process A String or we can say make a assignments
 *
 * @param s
 * @param it
 * @return int
 */
int CCoreBasicString_Process(CCString* s, AssignedStringIterator it);

/**
 * @brief Direct Clear for the sessions
 *
 * @param s
 */
void CCoreBasicString_Clear(CCString* s);

/**
 * @brief Get Size
 *
 * @param string
 * @return size_t
 */
static inline size_t CCoreBasicString_Size(CCString* string) {
	return string->size;
}

/**
 * @brief Get if empty
 *
 * @param string
 * @return int
 */
static inline int CCoreBasicString_Empty(CCString* string) {
	return string->size == 0;
}

#endif
