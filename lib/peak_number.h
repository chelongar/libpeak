/*
 * Copyright (c) 2014 Masoud Chelongar <masoud@packetwerk.com>
 * Copyright (c) 2014 Franco Fichtner <franco@packetwerk.com>
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

#ifndef PEAK_NUMBER_H
#define PEAK_NUMBER_H

enum {
	NUMBER_EQ,
	NUMBER_NE,
	NUMBER_GT,
	NUMBER_LT,
	NUMBER_LE,
	NUMBER_GE,
	NUMBER_MAX	/* last element */
};

enum {
	NUMBER_STRING = 0x0000,
	NUMBER_LENGTH = 0x0100,
};

struct peak_numbers	*peak_number_init(void);
const char		*peak_number_parse(const char *, const size_t);
unsigned int		 peak_number_add(struct peak_numbers *,
			     const unsigned int, const char *,
			     const size_t, const unsigned int);
void			 peak_number_find(struct peak_numbers *,
			     const char *, const size_t, stash_t);
void			 peak_number_exit(struct peak_numbers *);

#endif /* !PEAK_NUMBER_H */
