/*
 * Copyright (c) 2013-2014 Franco Fichtner <franco@packetwerk.com>
 * Copyright (c) 2014 Masoud Chelongar <masoud@packetwerk.com>
 * Copyright (c) 2014 Tobias Boertitz <tobias@packetwerk.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <peak.h>
#include <assert.h>

output_init();

static void
test_string_single(void)
{
	STASH_DECLARE(stash, unsigned int, 10);
	struct peak_strings *root;
	unsigned int *ret;

	peak_string_exit(peak_string_init());

	root = peak_string_init();
	assert(root);

	/* finds nothing */
	peak_string_find(root, "test", 4, stash);
	assert(STASH_EMPTY(stash));

	/* deduplicate strings */
	assert(1 == peak_string_add(root, 1, "test", 4, 0));
	assert(1 == peak_string_add(root, 2, "test", 4, 0));

	/* finds nothing */
	peak_string_find(root, NULL, 0, stash);
	assert(STASH_EMPTY(stash));
	peak_string_find(root, "test", 3, stash);
	assert(STASH_EMPTY(stash));

	/* single match */
	peak_string_find(root, "test", 4, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	/* single match over zeroed bytes */
	peak_string_find(root, "oO\0test", 7, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	/* multiple (sub)matches */
	peak_string_find(root, "test testest tesT tes", 21, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	peak_string_exit(NULL);
	peak_string_exit(root);
}

static void
test_string_left(void)
{
	STASH_DECLARE(stash, unsigned int, 10);
	struct peak_strings *root;
	unsigned int *ret;

	root = peak_string_init();
	assert(root);

	assert(1 == peak_string_add(root, 1, "test", 4, STRING_LEFT));

	/* finds nothing */
	peak_string_find(root, "string test", 11, stash);
	assert(STASH_EMPTY(stash));

	/* single match */
	peak_string_find(root, "test string", 11, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	peak_string_exit(root);
}

static void
test_string_wildcard(void)
{
	STASH_DECLARE(stash, unsigned int, 10);
	struct peak_strings *root;
	unsigned int *ret;

	root = peak_string_init();
	assert(root);

	assert(1 == peak_string_add(root, 1, "t?st", 4, 0));

	/* finds nothing */
	peak_string_find(root, "string tst", 10, stash);
	assert(STASH_EMPTY(stash));

	/* single match */
	peak_string_find(root, "test string", 11, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	peak_string_exit(root);
}

static void
test_string_nocase(void)
{
	STASH_DECLARE(stash, unsigned int, 10);
	struct peak_strings *root;
	unsigned int *ret;

	root = peak_string_init();
	assert(root);

	assert(1 == peak_string_add(root, 1, "aToZ", 4, STRING_NOCASE));

	/* single match */
	peak_string_find(root, "atoz", 4, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	/* multiple (sub)matches */
	peak_string_find(root, "aToz AtOZ ATOzatOz aTO", 21, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	peak_string_exit(NULL);
	peak_string_exit(root);
}

static void
test_string_right(void)
{
	STASH_DECLARE(stash, unsigned int, 10);
	struct peak_strings *root;
	unsigned int *ret;

	root = peak_string_init();
	assert(root);

	assert(1 == peak_string_add(root, 1, "test", 4, STRING_RIGHT));

	/* finds nothing */
	peak_string_find(root, "test string", 11, stash);
	assert(STASH_EMPTY(stash));

	/* single match */
	peak_string_find(root, "string test", 11, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	peak_string_exit(root);
}

static void
test_string_exact(void)
{
	STASH_DECLARE(stash, unsigned int, 10);
	struct peak_strings *root;
	unsigned int *ret;

	root = peak_string_init();
	assert(root);

	assert(1 == peak_string_add(root, 1, "test", 4,
	    STRING_LEFT|STRING_RIGHT));

	/* finds nothing */
	peak_string_find(root, "test string test", 16, stash);
	assert(STASH_EMPTY(stash));

	/* single match */
	peak_string_find(root, "test", 4, stash);
	assert((ret = STASH_POP(stash)));
	assert(*ret == 1);
	assert(STASH_EMPTY(stash));
	STASH_CLEAR(stash);

	peak_string_exit(root);
}

static void
test_string_multiple(void)
{
	const char *text1 = "Lorem ipsum dolor sit amet, "
	    "consetetur sadipscing elitr, sed diam nonumy "
	    "eirmod tempor invidunt ut labore et dolore "
	    "magna aliquyam erat, sed diam voluptua.";
	const char *text2 = "";
	const unsigned int result1[] = { 7, 1, 6, 2, 5, 2, 5, 4 };
	const unsigned int result2[] = { 7, 8 };
	STASH_DECLARE(stash, unsigned int, 10);
	struct peak_strings *root;
	unsigned int *ret, i = 0;

	root = peak_string_init();
	assert(root);

	assert(0 == peak_string_add(root, 1, NULL, 1, 0));
	assert(1 == peak_string_add(root, 1, " ipsum ", 7, 0));
	assert(2 == peak_string_add(root, 2, ", sed", 5, 0));
	assert(3 == peak_string_add(root, 3, ", sef ", 5, 0));
	assert(4 == peak_string_add(root, 4, ".", 1, 0));
	assert(5 == peak_string_add(root, 5, "diam ", 4, 0));
	assert(6 == peak_string_add(root, 6, "dolor sit amet,", 15, 0));
	assert(7 == peak_string_add(root, 7, "", 0, 0));
	assert(8 == peak_string_add(root, 8, " ipsum ", 0,
	    STRING_LEFT|STRING_RIGHT));
	assert(9 == peak_string_add(root, 9, " erat ", 8, 0));

	peak_string_find(root, text1, strlen(text1), stash);
	assert(STASH_COUNT(stash) == lengthof(result1));
	STASH_FOREACH(ret, stash) {
		assert(*ret == result1[i++]);
	}

	i = 0;
	STASH_CLEAR(stash);
	peak_string_find(root, text2, strlen(text2), stash);
	assert(STASH_COUNT(stash) == lengthof(result2));
	STASH_FOREACH(ret, stash) {
		assert(*ret == result2[i++]);
	}

	peak_string_exit(root);
}

int
main(void)
{
	pout("peak string test suite... ");

	test_string_single();
	test_string_multiple();
	test_string_wildcard();
	test_string_nocase();
	test_string_left();
	test_string_right();
	test_string_exact();

	pout("ok\n");

	return (0);
}
