#include "ccstring.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 简单断言宏
#define ASSERT_TRUE(expr, msg)                                                                     \
	do {                                                                                           \
		if (!(expr)) {                                                                             \
			fprintf(stderr, "Assertion failed: %s (%s:%d): %s\n", #expr, __FILE__, __LINE__, msg); \
			exit(1);                                                                               \
		}                                                                                          \
	} while (0)

#define ASSERT_EQ_SIZE(a, b)                                                                                                                  \
	do {                                                                                                                                      \
		if ((a) != (b)) {                                                                                                                     \
			fprintf(stderr, "Assertion failed: %s == %s (%s:%d): got %zu, want %zu\n", #a, #b, __FILE__, __LINE__, (size_t)(a), (size_t)(b)); \
			exit(1);                                                                                                                          \
		}                                                                                                                                     \
	} while (0)

#define ASSERT_STREQ(a, b)                                                                                                          \
	do {                                                                                                                            \
		if (strcmp((a), (b)) != 0) {                                                                                                \
			fprintf(stderr, "Assertion failed: %s == %s (%s:%d): got \"%s\", want \"%s\"\n", #a, #b, __FILE__, __LINE__, (a), (b)); \
			exit(1);                                                                                                                \
		}                                                                                                                           \
	} while (0)

// 测试创建 & 基本操作
void test_create_and_basic(void) {
	CCString* s = CCCoreBasicString_CreateEmpty();
	ASSERT_TRUE(s != NULL, "CreateEmpty returned NULL");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), 0);
	ASSERT_TRUE(CCoreBasicString_Empty(s), "Empty should be true for new string");
	const char* cs = CCoreBasicString_Get(s);
	ASSERT_TRUE(cs != NULL, "Get returned NULL on empty string");
	ASSERT_STREQ(cs, "");
	CCCoreBasicString_FreeString(s);
}

// 测试从普通 C 字符串创建
void test_create_from_plain(void) {
	const char* src = "hello world";
	CCString* s = CCCoreBasicString_CreateFromPlainString(src);
	ASSERT_TRUE(s != NULL, "CreateFromPlainString returned NULL");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), strlen(src));
	ASSERT_STREQ(CCoreBasicString_Get(s), src);
	CCCoreBasicString_FreeString(s);
}

// 测试 Append + PushBack
void test_append_and_pushback(void) {
	CCString* s = CCCoreBasicString_CreateEmpty();
	int r = CCoreBasicString_Append(s, "abc");
	ASSERT_TRUE(r == 0, "Append abc failed");
	ASSERT_STREQ(CCoreBasicString_Get(s), "abc");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), 3);

	r = CCoreBasicString_PushBack(s, 'X');
	ASSERT_TRUE(r == 0, "PushBack failed");
	ASSERT_STREQ(CCoreBasicString_Get(s), "abcX");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), 4);

	// 追加空串
	r = CCoreBasicString_Append(s, "");
	ASSERT_TRUE(r == 0, "Append empty failed");
	ASSERT_STREQ(CCoreBasicString_Get(s), "abcX");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), 4);

	CCCoreBasicString_FreeString(s);
}

// 测试 Insert
void test_insert(void) {
	CCString* s = CCCoreBasicString_CreateFromPlainString("hello");
	int r = CCoreBasicString_Insert(s, 0, "Start-");
	ASSERT_TRUE(r == 0, "Insert at beginning failed");
	ASSERT_STREQ(CCoreBasicString_Get(s), "Start-hello");
	r = CCoreBasicString_Insert(s, 6, "MID-");
	ASSERT_TRUE(r == 0, "Insert in middle failed");
	ASSERT_STREQ(CCoreBasicString_Get(s), "Start-MID-hello");
	r = CCoreBasicString_Insert(s, CCoreBasicString_Size(s), "-End");
	ASSERT_TRUE(r == 0, "Insert at end failed");
	ASSERT_STREQ(CCoreBasicString_Get(s), "Start-MID-hello-End");

	// 越界
	r = CCoreBasicString_Insert(s, CCoreBasicString_Size(s) + 1, "BAD");
	ASSERT_TRUE(r != 0, "Insert beyond end should fail");

	CCCoreBasicString_FreeString(s);
}

// 测试 Erase
void test_erase(void) {
	CCString* s = CCCoreBasicString_CreateFromPlainString("abcdefghijklmnopqrstuvwxyz");
	// erase middle
	int r = CCoreBasicString_Erase(s, 5, 10); // remove from f (index5) 10 chars
	ASSERT_TRUE(r == 0, "Erase middle failed");
	// we expect: abcde + (after f +10 characters, skip those) + rest
	// positions: 0-4 kept, 5 removed 10 chars (positions 5..14), so next start at pos 15
	const char* expected = "abcde"
	                       "pqrstuvwxyz";
	ASSERT_STREQ(CCoreBasicString_Get(s), expected);
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), strlen(expected));

	// erase to end
	r = CCoreBasicString_Erase(s, 5, 1000);
	ASSERT_TRUE(r == 0, "Erase to end failed");
	ASSERT_STREQ(CCoreBasicString_Get(s), "abcde");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), 5);

	// pos beyond end
	r = CCoreBasicString_Erase(s, 10, 1);
	ASSERT_TRUE(r != 0, "Erase beyond end should fail");

	CCCoreBasicString_FreeString(s);
}

// 测试 SubString
void test_substring(void) {
	CCString* s = CCCoreBasicString_CreateFromPlainString("0123456789");
	CCString* sub = CCoreBasicString_SubString(s, 2, 5);
	ASSERT_TRUE(sub != NULL, "SubString returned NULL");
	ASSERT_STREQ(CCoreBasicString_Get(sub), "23456");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(sub), 5);

	// len超过剩余
	CCString* sub2 = CCoreBasicString_SubString(s, 7, 100);
	ASSERT_TRUE(sub2 != NULL, "SubString with len too large should still produce valid substring");
	ASSERT_STREQ(CCoreBasicString_Get(sub2), "789");
	ASSERT_EQ_SIZE(CCoreBasicString_Size(sub2), 3);

	// pos 超界
	CCString* bad = CCoreBasicString_SubString(s, 11, 3);
	ASSERT_TRUE(bad == NULL, "SubString with pos beyond size should return NULL or error");

	CCCoreBasicString_FreeString(sub);
	CCCoreBasicString_FreeString(sub2);
	CCCoreBasicString_FreeString(s);
}

// 测试 Compare / Find
void test_compare_and_find(void) {
	CCString* a = CCCoreBasicString_CreateFromPlainString("apple");
	CCString* b = CCCoreBasicString_CreateFromPlainString("banana");
	ASSERT_TRUE(CCoreBasicString_Compare(a, b) < 0, "apple < banana");
	ASSERT_TRUE(CCoreBasicString_Compare(b, a) > 0, "banana > apple");
	ASSERT_TRUE(CCoreBasicString_Compare(a, a) == 0, "apple == apple");

	CCString* c = CCCoreBasicString_CreateFromPlainString("something something");
	size_t pos = CCoreBasicString_Find(c, "thing");
	ASSERT_TRUE(pos != (size_t)(-1), "Find should find substring");
	// check exact position
	// "some" (4) + "thing" starts at index 4
	ASSERT_EQ_SIZE(pos, 4);

	size_t pos2 = CCoreBasicString_Find(c, "xyz");
	ASSERT_TRUE(pos2 == (size_t)(-1), "Find unknown substring should return -1");

	CCCoreBasicString_FreeString(a);
	CCCoreBasicString_FreeString(b);
	CCCoreBasicString_FreeString(c);
}

// 测试 Clear
void test_clear(void) {
	CCString* s = CCCoreBasicString_CreateFromPlainString("not empty");
	CCoreBasicString_Clear(s);
	ASSERT_EQ_SIZE(CCoreBasicString_Size(s), 0);
	ASSERT_STREQ(CCoreBasicString_Get(s), "");
	CCCoreBasicString_FreeString(s);
}

void benchmark_append(int n, const char* chunk) {
	CCString* s = CCCoreBasicString_CreateEmpty();
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		int r = CCoreBasicString_Append(s, chunk);
		if (r != 0) {
			fprintf(stderr, "Append failed during benchmark\n");
			break;
		}
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("benchmark_append: n=%d, chunk_len=%zu, time=%.6f sec, final size=%zu\n",
	       n, strlen(chunk), elapsed, CCoreBasicString_Size(s));
	CCCoreBasicString_FreeString(s);
}

void benchmark_insert(int n, const char* chunk) {
	CCString* s = CCCoreBasicString_CreateFromPlainString("base");
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		int r = CCoreBasicString_Insert(s, CCoreBasicString_Size(s) / 2, chunk);
		if (r != 0) {
			fprintf(stderr, "Insert failed during benchmark\n");
			break;
		}
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("benchmark_insert: n=%d, chunk_len=%zu, time=%.6f sec, final size=%zu\n",
	       n, strlen(chunk), elapsed, CCoreBasicString_Size(s));
	CCCoreBasicString_FreeString(s);
}

// 测试 Reserve 接口
void benchmark_reserve(size_t cap_sizes[], int count) {
	printf("benchmark_reserve:\n");
	for (int i = 0; i < count; i++) {
		size_t cap = cap_sizes[i];
		CCString* s = CCCoreBasicString_CreateEmpty();
		clock_t start = clock();
		int r = CCoreBasicString_Reserve(s, cap);
		clock_t end = clock();
		double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
		printf("  Reserve(cap=%zu): time=%.6f sec, size=%zu, capacity>=%zu, ret=%d\n",
		       cap, elapsed, CCoreBasicString_Size(s), cap, r);
		CCCoreBasicString_FreeString(s);
	}
}

// 测试 ShrinkToFit 接口
void benchmark_shrink_to_fit(size_t initial_cap, size_t shrink_after, const char* chunk) {
	printf("benchmark_shrink_to_fit:\n");
	CCString* s = CCCoreBasicString_CreateEmpty();
	// 先 Reserve 初始容量
	CCoreBasicString_Reserve(s, initial_cap);

	// Append若干 chunk，使 size 到 shrink_after
	while (CCoreBasicString_Size(s) < shrink_after) {
		int r = CCoreBasicString_Append(s, chunk);
		if (r != 0) {
			fprintf(stderr, "Append failed during shrink_to_fit setup\n");
			break;
		}
	}

	printf("  Before shrink: size=%zu\n", CCoreBasicString_Size(s));
	clock_t start = clock();
	int r = CCoreBasicString_ShrinkToFit(s);
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  ShrinkToFit result ret=%d, time=%.6f sec, capacity approx size=%zu, size=%zu\n",
	       r, elapsed, CCoreBasicString_Size(s), CCoreBasicString_Size(s));

	CCCoreBasicString_FreeString(s);
}

// 测试 PushBack 接口
void benchmark_push_back(int n) {
	printf("benchmark_push_back: n=%d\n", n);
	CCString* s = CCCoreBasicString_CreateEmpty();
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		int r = CCoreBasicString_PushBack(s, 'a' + (i % 26));
		if (r != 0) {
			fprintf(stderr, "PushBack failed at iteration %d\n", i);
			break;
		}
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  time=%.6f sec, final size=%zu\n", elapsed, CCoreBasicString_Size(s));
	CCCoreBasicString_FreeString(s);
}

// 测试 Erase 接口
void benchmark_erase(size_t initial_size, size_t erase_len, int iterations) {
	printf("benchmark_erase: initial_size=%zu, erase_len=%zu, iterations=%d\n",
	       initial_size, erase_len, iterations);
	CCString* s = CCCoreBasicString_CreateEmpty();
	// 填满
	while (CCoreBasicString_Size(s) < initial_size) {
		CCoreBasicString_Append(s, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
	}
	printf("  setup done, size=%zu\n", CCoreBasicString_Size(s));
	clock_t start = clock();
	for (int i = 0; i < iterations; i++) {
		size_t pos = CCoreBasicString_Size(s) / 4; // 中间位置
		int r = CCoreBasicString_Erase(s, pos, erase_len);
		if (r != 0) {
			fprintf(stderr, "Erase failed at iteration %d\n", i);
			break;
		}
		// 可选：每次再添加回来那部分，以维持 size 稳定，避免 size 越来越小导致测试失衡
		CCoreBasicString_Insert(s, pos, "X"); // 插入一个字符，以维持测试结构
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  time=%.6f sec after %d erases+inserts, size=%zu\n",
	       elapsed, iterations, CCoreBasicString_Size(s));
	CCCoreBasicString_FreeString(s);
}

// 测试 SubString 接口
void benchmark_substring(const char* base, int n, size_t substr_len) {
	printf("benchmark_substring: base_len≈%zu, n=%d, substr_len=%zu\n",
	       strlen(base), n, substr_len);
	CCString* s = CCCoreBasicString_CreateFromPlainString(base);
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		size_t pos = (i * 7) % (CCoreBasicString_Size(s) + 1); // 分布 pos
		CCString* sub = CCoreBasicString_SubString(s, pos, substr_len);
		if (!sub) {
			fprintf(stderr, "SubString returned NULL at iteration %d\n", i);
			break;
		}
		// 使用子串，防止被优化掉
		// 比如读第一个字符
		const char* cs = CCoreBasicString_Get(sub);
		if (cs && cs[0] == '\0') {
			// do nothing
		}
		CCCoreBasicString_FreeString(sub);
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  time=%.6f sec for %d substrings, base size=%zu\n",
	       elapsed, n, CCoreBasicString_Size(s));
	CCCoreBasicString_FreeString(s);
}

// 测试 Compare 接口
void benchmark_compare(int n, const char* a_base, const char* b_base) {
	printf("benchmark_compare: n=%d, base lens: %zu vs %zu\n", n, strlen(a_base), strlen(b_base));
	CCString* a = CCCoreBasicString_CreateFromPlainString(a_base);
	CCString* b = CCCoreBasicString_CreateFromPlainString(b_base);
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		int r = CCoreBasicString_Compare(a, b);
		// perhaps alternate swap to detect different paths
		// ignore result
		(void)r;
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  time=%.6f sec for %d compares\n", elapsed, n);
	CCCoreBasicString_FreeString(a);
	CCCoreBasicString_FreeString(b);
}

// 测试 Find 接口
void benchmark_find(int n, const char* base, const char* substr) {
	printf("benchmark_find: n=%d, base_len=%zu, substr_len=%zu\n",
	       n, strlen(base), strlen(substr));
	CCString* s = CCCoreBasicString_CreateFromPlainString(base);
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		size_t pos = CCoreBasicString_Find(s, substr);
		(void)pos;
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  time=%.6f sec for %d finds\n", elapsed, n);
	CCCoreBasicString_FreeString(s);
}

// 测试 Visit 接口
void benchmark_visit(int n, const char* base, VisitStringIterator it) {
	printf("benchmark_visit: n=%d, base_len=%zu\n", n, strlen(base));
	CCString* s = CCCoreBasicString_CreateFromPlainString(base);
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		CCoreBasicString_Visit(s, it);
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  time=%.6f sec for %d visits\n", elapsed, n);
	CCCoreBasicString_FreeString(s);
}

// 测试 Process 接口
void benchmark_process(int n, const char* base, AssignedStringIterator it) {
	printf("benchmark_process: n=%d, base_len=%zu\n", n, strlen(base));
	CCString* s = CCCoreBasicString_CreateFromPlainString(base);
	clock_t start = clock();
	for (int i = 0; i < n; i++) {
		int r = CCoreBasicString_Process(s, it);
		(void)r;
	}
	clock_t end = clock();
	double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
	printf("  time=%.6f sec for %d processes\n", elapsed, n);
	CCCoreBasicString_FreeString(s);
}

// 一个 sample iterator 用于 Visit
void sample_visit_iterator(const int ch) {
	// 简单操作，比如 nop 或计数
	(void)ch;
}

// 一个 sample iterator 用于 Process
int sample_process_iterator(const int ch) {
	// 比如把字符加 1，如果是字母
	return (ch >= 'a' && ch <= 'z') ? (ch - 'a' + 'A') : ch;
}

// 扩展的 run_performance_tests
void run_performance_tests(void) {
	printf("Starting performance tests (extended)...\n");

	// 之前的 append/insert
	benchmark_append(1000, "x");
	benchmark_append(10000, "abcdef");
	benchmark_append(100000, "long substring to append");
	benchmark_insert(1000, "y");
	benchmark_insert(10000, "middle");
	benchmark_insert(100000, "mid-substring test");

	// 新增接口测试
	size_t reserve_caps[] = { 0, 10, 1000, 1000000 };
	benchmark_reserve(reserve_caps, sizeof(reserve_caps) / sizeof(reserve_caps[0]));

	benchmark_shrink_to_fit(1000000, 500000, "abc");

	benchmark_push_back(1000000);

	benchmark_erase(1000000, 50000, 1000);

	// 生成一个很长基字符串用于 substring / compare / find / visit / process
	char* base = malloc(2000001);
	if (!base) {
		fprintf(stderr, "malloc base failed\n");
	} else {
		for (size_t i = 0; i < 2000000; i++) {
			base[i] = 'a' + (i % 26);
		}
		base[2000000] = '\0';

		benchmark_substring(base, 10000, 50);
		benchmark_compare(100000, base, base); // same strings
		// slight modification for difference
		base[1000] = 'Z';
		benchmark_compare(100000, base, base);

		benchmark_find(100000, base, "abcdef");
		benchmark_find(100000, base, "notexistsubstring");

		benchmark_visit(10000, base, sample_visit_iterator);

		benchmark_process(10000, base, sample_process_iterator);
	}

	printf("All performance tests done.\n");
}

int main(void) {
	printf("Running functional / edge case tests...\n");
	test_create_and_basic();
	test_create_from_plain();
	test_append_and_pushback();
	test_insert();
	test_erase();
	test_substring();
	test_compare_and_find();
	test_clear();

	printf("All functional tests passed.\n");

	run_performance_tests();

	printf("Performance tests done.\n");

	return 0;
}
