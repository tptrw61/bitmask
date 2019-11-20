#ifndef BITMASK_H_INCLUDED
#define BITMASK_H_INCLUDED

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define BM_TRUE    TRUE
#define BM_FALSE   FALSE
#define BM_ERROR  -1

#define BM_OKAY                  0
#define BM_INDEX_OUT_OF_BOUNDS  -1
#define BM_INVALID_SIZE         -2
#define BM_NULL_POINTER         -3
#define BM_BAD_MALLOC           -4
#define BM_SIZE_MISMATCH        -5

typedef struct Bitmask Bitmask;

Bitmask * BM_create(int size);
void BM_destroy(Bitmask *bm);

int BM_get(Bitmask *bm, int ix);
int BM_set(Bitmask *bm, int ix);
int BM_unset(Bitmask *bm, int ix);
int BM_toggle(Bitmask *bm, int ix);
int BM_setTo(Bitmask *bm, int ix, int val);

int BM_size(Bitmask *bm);

int BM_lastError(const Bitmask *bm);
void BM_clearError(Bitmask *bm);

Bitmask * BM_clone(const Bitmask *bm);

int BM_setAll(Bitmask *bm);
int BM_unsetAll(Bitmask *bm);
int BM_toggleAll(Bitmask *bm);

int BM_and(Bitmask *bm1, const Bitmask *bm2);
int BM_or(Bitmask *bm1, const Bitmask *bm2);
int BM_xor(Bitmask *bm1, const Bitmask *bm2);

char * BM_string(const Bitmask *bm);

#endif 
