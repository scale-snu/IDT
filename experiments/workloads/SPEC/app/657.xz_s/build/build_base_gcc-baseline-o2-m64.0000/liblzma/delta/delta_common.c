///////////////////////////////////////////////////////////////////////////////
//
/// \file       delta_common.c
/// \brief      Common stuff for Delta encoder and decoder
//
//  Author:     Lasse Collin
//
//  This file has been put into the public domain.
//  You can do whatever you want with this file.
//
///////////////////////////////////////////////////////////////////////////////

#include "delta_common.h"
#include "delta_private.h"


static void
delta_coder_end(lzma_coder *pcoder, lzma_allocator *allocator)
{
	struct lzma_coder_s* coder = pcoder;
	lzma_next_end(&coder->next, allocator);
	lzma_free(coder, allocator);
	return;
}


extern lzma_ret
lzma_delta_coder_init(lzma_next_coder *next, lzma_allocator *allocator,
		const lzma_filter_info *filters)
{
	// Allocate memory for the decoder if needed.
	if (next->coder == NULL) {
		next->coder = lzma_alloc(sizeof(struct lzma_coder_s), allocator);
		if (next->coder == NULL)
			return LZMA_MEM_ERROR;

		// End function is the same for encoder and decoder.
		next->end = &delta_coder_end;
		((struct lzma_coder_s*)next->coder)->next = LZMA_NEXT_CODER_INIT;
	}

	// Validate the options.
	if (lzma_delta_coder_memusage(filters[0].options) == UINT64_MAX)
		return LZMA_OPTIONS_ERROR;

	// Set the delta distance.
	const lzma_options_delta *opt = filters[0].options;
	((struct lzma_coder_s*)next->coder)->distance = opt->dist;

	// Initialize the rest of the variables.
	((struct lzma_coder_s*)next->coder)->pos = 0;
	memzero(((struct lzma_coder_s*)next->coder)->history, LZMA_DELTA_DIST_MAX);

	// Initialize the next decoder in the chain, if any.
	return lzma_next_filter_init(&((struct lzma_coder_s*)next->coder)->next,
			allocator, filters + 1);
}


extern uint64_t
lzma_delta_coder_memusage(const void *options)
{
	const lzma_options_delta *opt = options;

	if (opt == NULL || opt->type != LZMA_DELTA_TYPE_BYTE
			|| opt->dist < LZMA_DELTA_DIST_MIN
			|| opt->dist > LZMA_DELTA_DIST_MAX)
		return UINT64_MAX;

	return sizeof(struct lzma_coder_s);
}
