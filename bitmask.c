#include "bitmask.h"
#include <stdlib.h>
#include <stdint.h>

#define ARR_SIZE(bm) (((bm->size / 8) + (bm->size % 8 == 0 ? 0 : 1)))

typedef struct Mask {
	uint8_t b0:1;
	uint8_t b1:1;
	uint8_t b2:1;
	uint8_t b3:1;
	uint8_t b4:1;
	uint8_t b5:1;
	uint8_t b6:1;
	uint8_t b7:1;
} Mask;

struct Bitmask {
	int lastError;
	int size;
	Mask *mask;
};

static int lastError = BM_OKAY;

Bitmask * BM_create(int size) {
	if (size <= 0) {
		lastError = BM_INVALID_SIZE;
		return NULL;
	}
	Bitmask *bm = (Bitmask *)malloc(sizeof(Bitmask));
	if (bm == NULL) {
		lastError = BM_BAD_MALLOC;
		return NULL;
	}
	bm->size = size;
	bm->mask = (Mask*)malloc(sizeof(Mask) * ARR_SIZE(bm));
	if (bm->mask == NULL) {
		lastError = BM_BAD_MALLOC;
		free(bm);
		return NULL;
	}
	BM_unsetAll(bm);
	return bm;
}

void BM_destroy(Bitmask *bm) {
	if (bm == NULL)
		return;
	free(bm->mask);
	free(bm);
}

int BM_get(Bitmask *bm, int ix) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	if (ix >= bm->size) {
		bm->lastError = BM_INDEX_OUT_OF_BOUNDS;
		return BM_ERROR;
	}
	Mask byte = bm->mask[ix / 8];
	switch (ix % 8) {
		case 0: return byte.b0;
		case 1: return byte.b1;
		case 2: return byte.b2;
		case 3: return byte.b3;
		case 4: return byte.b4;
		case 5: return byte.b5;
		case 6: return byte.b6;
		case 7: return byte.b7;
	}
	return BM_ERROR; //never reached
}

int BM_set(Bitmask *bm, int ix) {
	return BM_setTo(bm, ix, 1);
}
int BM_unset(Bitmask *bm, int ix) {
	return BM_setTo(bm, ix, 0);
}
int BM_setTo(Bitmask *bm, int ix, int val) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	if (ix >= bm->size) {
		bm->lastError = BM_INDEX_OUT_OF_BOUNDS;
		return BM_ERROR;
	}
	Mask *byte = bm->mask + (ix / 8);
	switch (ix % 8) {
		case 0: byte->b0 = 1 & val; return BM_OKAY;
		case 1: byte->b1 = 1 & val; return BM_OKAY;
		case 2: byte->b2 = 1 & val; return BM_OKAY;
		case 3: byte->b3 = 1 & val; return BM_OKAY;
		case 4: byte->b4 = 1 & val; return BM_OKAY;
		case 5: byte->b5 = 1 & val; return BM_OKAY;
		case 6: byte->b6 = 1 & val; return BM_OKAY;
		case 7: byte->b7 = 1 & val; return BM_OKAY;
	}
	return BM_ERROR; //never reached
}

int BM_toggle(Bitmask *bm, int ix) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	if (ix >= bm->size) {
		bm->lastError = BM_INDEX_OUT_OF_BOUNDS;
		return BM_ERROR;
	}
	Mask *byte = bm->mask + (ix / 8);
	switch (ix % 8) {
		case 0: byte->b0 = 1 - byte->b0; return BM_OKAY;
		case 1: byte->b1 = 1 - byte->b1; return BM_OKAY;
		case 2: byte->b2 = 1 - byte->b2; return BM_OKAY;
		case 3: byte->b3 = 1 - byte->b3; return BM_OKAY;
		case 4: byte->b4 = 1 - byte->b4; return BM_OKAY;
		case 5: byte->b5 = 1 - byte->b5; return BM_OKAY;
		case 6: byte->b6 = 1 - byte->b6; return BM_OKAY;
		case 7: byte->b7 = 1 - byte->b7; return BM_OKAY;
	}
	return BM_ERROR; //never reached
}

int BM_size(Bitmask *bm) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	return bm->size;
}

int BM_lastError(const Bitmask *bm) {
	if (bm == NULL)
		return lastError;
	return bm->lastError;
}

void BM_clearError(Bitmask *bm) {
	if (bm == NULL)
		lastError = BM_OKAY;
	else
		bm->lastError = BM_OKAY;
}

Bitmask * BM_clone(const Bitmask *bm) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return NULL;
	}
	Bitmask *cl = BM_create(bm->size);
	if (cl == NULL)
		return NULL;
	int arrsize = ARR_SIZE(bm);
	for (int i = 0; i < arrsize; i++)
		cl->mask[i] = bm->mask[i];
	return cl;
}

static uint8_t mask2char(Mask m) {
	uint8_t x = 0;
	x |= m.b0;
	x |= m.b1<<1;
	x |= m.b2<<2;
	x |= m.b3<<3;
	x |= m.b4<<4;
	x |= m.b5<<5;
	x |= m.b6<<6;
	x |= m.b7<<7;
	return x;
}

static Mask char2mask(uint8_t x) {
	Mask m;
	m.b0 = 1 & x;
	m.b1 = 1 & (x>>1);
	m.b2 = 1 & (x>>2);
	m.b3 = 1 & (x>>3);
	m.b4 = 1 & (x>>4);
	m.b5 = 1 & (x>>5);
	m.b6 = 1 & (x>>6);
	m.b7 = 1 & (x>>7);
	return m;
}

int BM_setAll(Bitmask *bm) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	int arrsize = ARR_SIZE(bm);
	for (int i = 0; i < arrsize; i++)
		bm->mask[i] = char2mask(0xff);
	return BM_OKAY;
}

int BM_unsetAll(Bitmask *bm) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	int arrsize = ARR_SIZE(bm);
	for (int i = 0; i < arrsize; i++)
		bm->mask[i] = char2mask(0);
	return BM_OKAY;
}

int BM_toggleAll(Bitmask *bm) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	int arrsize = ARR_SIZE(bm);
	for (int i = 0; i < arrsize; i++)
		bm->mask[i] = char2mask(~mask2char(bm->mask[i]));
	return BM_OKAY;
}

int BM_and(Bitmask *bm1, const Bitmask *bm2) {
	if (bm1 == NULL || bm2 == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	if (bm1->size != bm2->size) {
		bm1->lastError = lastError = BM_SIZE_MISMATCH;
		return BM_ERROR;
	}
	int arrsize = ARR_SIZE(bm1);
	for (int i = 0; i < arrsize; i++) {
		bm1->mask[i] = char2mask(mask2char(bm1->mask[i]) & mask2char(bm2->mask[i]));
	}
	return BM_OKAY;
}

int BM_or(Bitmask *bm1, const Bitmask *bm2) {
	if (bm1 == NULL || bm2 == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	if (bm1->size != bm2->size) {
		bm1->lastError = lastError = BM_SIZE_MISMATCH;
		return BM_ERROR;
	}
	int arrsize = ARR_SIZE(bm1);
	for (int i = 0; i < arrsize; i++) {
		bm1->mask[i] = char2mask(mask2char(bm1->mask[i]) | mask2char(bm2->mask[i]));
	}
	return BM_OKAY;
}

int BM_xor(Bitmask *bm1, const Bitmask *bm2) {
	if (bm1 == NULL || bm2 == NULL) {
		lastError = BM_NULL_POINTER;
		return BM_ERROR;
	}
	if (bm1->size != bm2->size) {
		bm1->lastError = lastError = BM_SIZE_MISMATCH;
		return BM_ERROR;
	}
	int arrsize = ARR_SIZE(bm1);
	for (int i = 0; i < arrsize; i++) {
		bm1->mask[i] = char2mask(mask2char(bm1->mask[i]) ^ mask2char(bm2->mask[i]));
	}
	return BM_OKAY;
}

char * BM_string(const Bitmask *bm) {
	if (bm == NULL) {
		lastError = BM_NULL_POINTER;
		return NULL;
	}
	char *s = (char *)malloc(sizeof(char) * (bm->size + 1));
	if (s == NULL) {
		lastError = BM_BAD_MALLOC;
		return NULL;
	}
	int i, j;
	for (i = j = 0; i < bm->size / 8; i++, j+=8) {
		s[j] = '0' + bm->mask[i].b0;
		s[j+1] = '0' + bm->mask[i].b1;
		s[j+2] = '0' + bm->mask[i].b2;
		s[j+3] = '0' + bm->mask[i].b3;
		s[j+4] = '0' + bm->mask[i].b4;
		s[j+5] = '0' + bm->mask[i].b5;
		s[j+6] = '0' + bm->mask[i].b6;
		s[j+7] = '0' + bm->mask[i].b7;
	}
	switch (bm->size % 8) {
		case 7: s[j+6] = bm->mask[i].b6;
		case 6: s[j+5] = bm->mask[i].b5;
		case 5: s[j+4] = bm->mask[i].b4;
		case 4: s[j+3] = bm->mask[i].b3;
		case 3: s[j+2] = bm->mask[i].b2;
		case 2: s[j+1] = bm->mask[i].b1;
		case 1: s[j] = bm->mask[i].b0;
		case 0: break;
	}
	s[bm->size] = '\0';
	return s;
}
