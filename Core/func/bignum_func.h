#ifndef _BIGNUM_FUNC_H_
#define _BIGNUM_FUNC_H_

#include <stdint.h>
#include <string.h>

#define BN_SIZE 512

#if 256 == BN_SIZE
#define BIG_NUM_SIZE 5
#define RAND_SIZE 2
#elif 512 == BN_SIZE
#define BIG_NUM_SIZE 17
#define RAND_SIZE 8
#elif 1024 == BN_SIZE
#define BIG_NUM_SIZE 33
#define RAND_SIZE 16
#endif

typedef struct big_number {
    uint64_t *data;
    uint32_t sign;
    uint32_t uint64_length;
    uint32_t high_length;
} BN;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define BN_NEW(bn)  \
{   \
    bn = (BN*)calloc(1, sizeof(BN));   \
    bn->data = (uint64_t*)calloc(BIG_NUM_SIZE ,sizeof(uint64_t));   \
}

#define BN_INIT(bn) \
{   \
    memset(bn->data, 0, BIG_NUM_SIZE * sizeof(uint64_t));    \
    bn->sign = 0;   \
    bn->uint64_length = 0; \
}

#define BN_DEL(bn)  \
{   \
    free(bn->data); \
    free(bn);   \
}

#define BN_COPY(bn_to, bn_from) \
{   \
    memcpy(bn_to->data, bn_from->data, BIG_NUM_SIZE * sizeof(uint64_t)); \
    bn_to->sign = bn_from->sign;    \
    bn_to->uint64_length = bn_from->uint64_length;   \
}

#define BN_DEC2BN(dec, bn)  \
{    \
    BN_INIT(bn);   \
    bn->data[0] = dec;   \
    bn->uint64_length = 1; \
}

#define BN_EQ_Y(bn_x, y) ((y == bn_x->data[0] && 1 == bn_x->uint64_length) ? 1 : 0)

#define BN_BIT_LENGTH_DIFF(bn_x, bn_y) (BN_bit_length(bn_x) - BN_bit_length(bn_y))

#define BN_SUB_MOD_SELF(bn_x, bn_y)   \
{   \
    if(1 == BN_cmp(bn_x, bn_y)){  \
        BN_sub(bn_x, bn_y, bn_x);  \
    }   \
}

int32_t BN_bn2str(char *str, const BN *bn_x);
int32_t BN_str2bn(const char *str, BN *bn_x);
int32_t BN_uint64_length(const BN *bn_x);
int32_t BN_high_length(const BN *bn_x);
int32_t BN_bit_length(const BN *bn_x);
int32_t BN_add(const BN *bn_x, const BN *bn_y, BN *result);
int32_t BN_sub(const BN *bn_x, const BN *bn_y, BN *result);
int32_t BN_mul2(const BN *bn_x, BN *result);
int32_t BN_div2(const BN *bn_x, BN *result);
int32_t BN_left(const BN *bn_x, const int32_t additive, BN *result);
int32_t BN_cmp(const BN *bn_x, const BN *bn_y);
int32_t BN_mul(const BN *bn_x, const BN *bn_y, const BN *bn_n, BN *result);
int32_t BN_pow_fast(const BN *bn_x, const BN *e, const BN *n, BN *bn_y);
int32_t BN_1_left(const int32_t additive, BN *result);
int32_t BN_div(const BN *bn_x, const BN *bn_y, BN *result, BN *remain);
int32_t BN_div_mod(const BN *bn_x, const BN *bn_y, BN *remain);
int32_t BN_Extended_Euclid_Algorithm(const BN *bn_x, const BN *bn_y, BN *bn_x_inv);
int32_t BN_Miller_Rabin(const BN *bn_x);
int32_t BN_rand(BN *bn_x, const int32_t uint64_length);

#endif
