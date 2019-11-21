#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "bitmask.h"

int getBit(int x, int b) {
	return 1 & (x>>b);
}

char * writeInt(int x, int bits) {
	char *s = malloc(sizeof(char) * (bits+1));
	if (s == NULL)
		return NULL;
	for (int i = 0; i < bits; i++) {
		s[i] = '0' + getBit(x, i);
	}
	s[bits] = 0;
	return s;
}

int main(void) {
	Bitmask *bm;
	bm = BM_create(0);
	assert(bm == NULL);
	assert(BM_lastError(0) == BM_INVALID_SIZE);
	BM_clearError(0);
	bm = BM_create(16);
	assert(bm != NULL);
	assert(BM_lastError(0) == BM_OKAY);
	assert(BM_lastError(bm) == BM_OKAY);
	int t = time(0);
	srand(t);
	int r = rand();
	r &= 0xffff;
	for (int i = 0; i < 16; i++) {
		BM_setTo(bm, i, getBit(r, i));
	}
	char *s = writeInt(r, 16);
	char *bms = BM_string(bm);
	int comp = strcmp(bms, s);
	free(s);
	free(bms);
	assert(comp == 0);
	BM_set(bm, 0);
	assert(BM_get(bm, 0) == 1);
	BM_toggle(bm, 0);
	assert(BM_get(bm, 0) == 0);
	BM_set(bm, 0);
	BM_unset(bm, 0);
	assert(BM_get(bm, 0) == 0);
	BM_destroy(bm);
	printf("TEST PASSED.\n");
	return 0;
}
