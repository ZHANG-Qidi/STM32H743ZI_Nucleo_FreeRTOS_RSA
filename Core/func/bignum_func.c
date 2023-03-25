#include "bignum_func.h"

#include "common_func.h"
#include "rsa_func.h"
#include "usart_func.h"
#include "rng.h"
#include <stdio.h>
#include <stdlib.h>

SECTION_FLASH int32_t BN_bn2str(char *str, const BN *bn_x) {
    str[0] = '\0';
    char *str_temp = calloc(32, sizeof(int8_t));
    for (int32_t i = BIG_NUM_SIZE; i--;) {
        sprintf(str_temp, "%08lX%08lX ", (uint32_t) (bn_x->data[i] >> 32), (uint32_t) (bn_x->data[i] & 0xFFFFFFFF));
        strcat(str, str_temp);
    }
    free(str_temp);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_str2bn(const char *str, BN *bn_x) {
    uint64_t *data = calloc(64, sizeof(uint64_t));
    int32_t data_i = 0;
    char *token = strtok((char*) str, " ");
    while (NULL != token && data_i != 64) {
        data[data_i++] = strtoull(token, NULL, 16);
        token = strtok(NULL, " ");
    }
    for (int32_t i = 0; i != MIN(BIG_NUM_SIZE, data_i); i++) {
        bn_x->data[i] = data[data_i - 1 - i];
    }
    bn_x->uint64_length = BN_uint64_length(bn_x);
    free(data);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_uint64_length(const BN *bn_x) {
    for (int32_t i = BIG_NUM_SIZE; i--;) {
        if (0 != bn_x->data[i]) {
            return ++i;
        }
    }
    return 0;
}

SECTION_FLASH int32_t BN_high_length(const BN *bn_x) {
    for (int32_t i = 64; i--;) {
        if (0 != bn_x->data[bn_x->uint64_length - 1] >> i) {
            return ++i;
        }
    }
    return 0;
}

SECTION_FLASH int32_t BN_bit_length(const BN *bn_x) {
    for (int32_t i = 64; i--;) {
        if (0 != bn_x->data[bn_x->uint64_length - 1] >> i) {
            return ((bn_x->uint64_length - 1) << 6) + ++i;
        }
    }
    return 0;
}

SECTION_FLASH int32_t BN_add(const BN *bn_x, const BN *bn_y, BN *result) {
    uint64_t carry = 0, temp1, temp2;
    result->uint64_length = MAX(bn_x->uint64_length, bn_y->uint64_length);
    for (int32_t i = 0; i != result->uint64_length; i++) {
        temp1 = bn_x->data[i] + bn_y->data[i];
        temp2 = temp1 + carry;
        if (temp1 < bn_x->data[i] || temp2 < temp1) {
            carry = 1;
        } else {
            carry = 0;
        }
        result->data[i] = temp2;
    }
    if (1 == carry && result->uint64_length != BIG_NUM_SIZE) {
        result->data[result->uint64_length++] = 1;
    }
    for (int32_t i = result->uint64_length; i != BIG_NUM_SIZE; i++) {
        result->data[i] = 0;
    }
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_sub(const BN *bn_x, const BN *bn_y, BN *result) {
    uint64_t carry = 1, temp1, temp2;
    result->uint64_length = bn_x->uint64_length;
    for (int32_t i = 0; i != result->uint64_length; i++) {
        temp1 = bn_x->data[i] + ~bn_y->data[i];
        temp2 = temp1 + carry;
        if (temp1 < bn_x->data[i] || temp2 < temp1) {
            carry = 1;
        } else {
            carry = 0;
        }
        result->data[i] = temp2;
    }
    for (int32_t i = result->uint64_length; i != BIG_NUM_SIZE; i++) {
        result->data[i] = 0;
    }
    for (int32_t i = result->uint64_length; i--;) {
        if (0 != result->data[i]) {
            result->uint64_length = i + 1;
            return EXIT_SUCCESS;
        }
    }
    result->uint64_length = 0;
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_mul2(const BN *bn_x, BN *result) {
    uint64_t carry = 0, carry_temp = 0;
    result->uint64_length = bn_x->uint64_length;
    for (int32_t i = 0; i != result->uint64_length; i++) {
        if (0x8000000000000000 & bn_x->data[i]) {
            carry_temp = 1;
        } else {
            carry_temp = 0;
        }
        result->data[i] = (uint64_t) (bn_x->data[i] << 1) | carry;
        carry = carry_temp;
    }
    if (1 == carry && BIG_NUM_SIZE != result->uint64_length) {
        result->data[result->uint64_length++] = 1;
    }
    for (int32_t i = result->uint64_length; i != BIG_NUM_SIZE; i++) {
        result->data[i] = 0;
    }
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_div2(const BN *bn_x, BN *result) {
    uint64_t carry = 0, carry_temp = 0;
    result->uint64_length = bn_x->uint64_length;
    for (int32_t i = result->uint64_length; i--;) {
        if (1 & bn_x->data[i]) {
            carry_temp = 0x8000000000000000;
        } else {
            carry_temp = 0;
        }
        result->data[i] = (uint64_t) (bn_x->data[i] >> 1) | carry;
        carry = carry_temp;
    }
    if (0 == result->data[result->uint64_length - 1] && 0 != result->uint64_length) {
        result->uint64_length--;
    }
    for (int32_t i = result->uint64_length; i != BIG_NUM_SIZE; i++) {
        result->data[i] = 0;
    }
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_left(const BN *bn_x, const int32_t additive, BN *result) {
    int32_t i = additive >> 6;      //additive / 64
    int32_t j = additive & 0x3F;    //additive % 64
    BN *bn_x_temp;
    BN_NEW(bn_x_temp);
    memcpy(bn_x_temp->data + i, bn_x->data, MIN(bn_x->uint64_length, BIG_NUM_SIZE - i) * sizeof(uint64_t));
    uint64_t carry = 0, carry_temp = 0;
    bn_x_temp->uint64_length = MIN(bn_x->uint64_length + i, BIG_NUM_SIZE);
    for (int32_t k = i; 0 != j && k != bn_x_temp->uint64_length; k++) {
        carry_temp = bn_x_temp->data[k] >> (64 - j);
        bn_x_temp->data[k] <<= j;
        bn_x_temp->data[k] |= carry;
        carry = carry_temp;
    }
    if (0 != carry && BIG_NUM_SIZE != bn_x_temp->uint64_length) {
        bn_x_temp->data[bn_x_temp->uint64_length++] = carry;
    }
    BN_COPY(result, bn_x_temp);
    BN_DEL(bn_x_temp);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_cmp(const BN *bn_x, const BN *bn_y) {
    if (0 == memcmp(bn_x, bn_y, sizeof(BN))) {
        return 0;
    }
    if (bn_x->uint64_length > bn_y->uint64_length) {
        return 1;
    } else if (bn_x->uint64_length < bn_y->uint64_length) {
        return -1;
    }
    for (int32_t i = bn_x->uint64_length; i--;) {
        if (bn_x->data[i] == bn_y->data[i]) {
            continue;
        } else if (bn_x->data[i] > bn_y->data[i]) {
            return 1;
        } else {
            return -1;
        }
    }
    return 0;
}

SECTION_FLASH int32_t BN_mul(const BN *bn_x, const BN *bn_y, const BN *bn_n, BN *result) {
    if (-1 == BN_cmp(bn_x, bn_y)) {
        BN *bn_temp = (BN*) bn_x;
        bn_x = bn_y;
        bn_y = bn_temp;
    }
    BN *result_temp, *bn_x_left_shift;
    BN_NEW(result_temp);
    BN_NEW(bn_x_left_shift);
    BN_COPY(bn_x_left_shift, bn_x);
    uint64_t bn_y_data;
    int32_t bn_y_high_length = BN_high_length(bn_y);
    for (int32_t i = 0; i != bn_y->uint64_length; i++) {
        bn_y_data = bn_y->data[i];
        if (bn_x->uint64_length + i + 1 < bn_n->uint64_length) {
            for (int32_t j = 0; j != 64 && 0 != bn_y_data; j++) {
                if (1 & bn_y_data) {
                    BN_left(bn_x, (i << 6) + j, bn_x_left_shift);
                    BN_add(result_temp, bn_x_left_shift, result_temp);
                }
                bn_y_data >>= 1;
            }
        } else {
            if (bn_x->uint64_length + i + 1 == bn_n->uint64_length) {
                BN_left(bn_x, i << 6, bn_x_left_shift);
            }
            for (int32_t j = 0; j != 64; j++) {
                if (1 & bn_y_data) {
                    BN_add(result_temp, bn_x_left_shift, result_temp);
                    BN_SUB_MOD_SELF(result_temp, bn_n);
                }
                BN_mul2(bn_x_left_shift, bn_x_left_shift);
                BN_SUB_MOD_SELF(bn_x_left_shift, bn_n);
                bn_y_data >>= 1;
                if (i == bn_y->uint64_length - 1 && j == bn_y_high_length - 1) {
                    goto END;
                }
            }
        }
    }
    END:
    BN_COPY(result, result_temp);
    BN_DEL(result_temp);
    BN_DEL(bn_x_left_shift);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_pow_fast(const BN *bn_x, const BN *e, const BN *n, BN *bn_y) {
    BN *bn_x_temp;
    BN_NEW(bn_x_temp);
    uint64_t temp;
    BN_COPY(bn_x_temp, bn_x);
    BN_DEC2BN(1, bn_y);
    int32_t e_high_length = BN_high_length(e);
    for (int32_t i = 0; i != e->uint64_length; i++) {
        temp = e->data[i];
        for (int32_t j = 0; 64 != j; j++) {
            if (temp & 1) {
                BN_mul(bn_y, bn_x_temp, n, bn_y);
            }
            temp >>= 1;
            BN_mul(bn_x_temp, bn_x_temp, n, bn_x_temp);
            if (i == e->uint64_length - 1 && j == e_high_length - 1) {
                goto END;
            }
        }
    }
    END:
    BN_DEL(bn_x_temp);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_1_left(const int32_t additive, BN *result) {
    BN_INIT(result);
    result->data[additive >> 6] = (uint64_t) 1 << (additive & 0x3F);
    result->uint64_length = 1 + (additive >> 6);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_div(const BN *bn_x, const BN *bn_y, BN *result, BN *remain) {
    BN *bn_y_temp, *bn_1_left_shift;
    BN_NEW(bn_y_temp);
    BN_NEW(bn_1_left_shift);
    BN_COPY(remain, bn_x);
    BN_COPY(bn_y_temp, bn_y);
    int32_t bit_length_diff = BN_BIT_LENGTH_DIFF(bn_x, bn_y);
    BN_left(bn_y_temp, bit_length_diff, bn_y_temp);
    BN_INIT(result);
    while (-1 != bit_length_diff) {
        if (-1 != BN_cmp(remain, bn_y_temp)) {
            BN_1_left(bit_length_diff, bn_1_left_shift);
            BN_add(result, bn_1_left_shift, result);
            BN_sub(remain, bn_y_temp, remain);
        }
        BN_div2(bn_y_temp, bn_y_temp);
        bit_length_diff--;
    }
    BN_DEL(bn_y_temp);
    BN_DEL(bn_1_left_shift);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_div_mod(const BN *bn_x, const BN *bn_y, BN *remain) {
    if (1 != BN_cmp(bn_x, bn_y)) {
        if (remain != bn_x) {
            BN_COPY(remain, bn_x);
        }
        return EXIT_SUCCESS;
    }
    BN *bn_y_temp;
    BN_NEW(bn_y_temp);
    BN_COPY(remain, bn_x);
    BN_COPY(bn_y_temp, bn_y);
    int32_t bit_length_diff = BN_BIT_LENGTH_DIFF(bn_x, bn_y);
    BN_left(bn_y_temp, bit_length_diff, bn_y_temp);
    while (-1 != bit_length_diff) {
        if (-1 != BN_cmp(remain, bn_y_temp)) {
            BN_sub(remain, bn_y_temp, remain);
        }
        BN_div2(bn_y_temp, bn_y_temp);
        bit_length_diff--;
    }
    BN_DEL(bn_y_temp);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t BN_Extended_Euclid_Algorithm(const BN *bn_x, const BN *bn_y, BN *bn_x_inv) {
    BN *bn_x_inv_old_old, *bn_x_inv_new, *bn_x_inv_old, *bn_x_temp, *bn_y_temp, *quotient, *remainder;
    BN_NEW(bn_x_inv_new);
    BN_NEW(bn_x_inv_old);
    BN_NEW(bn_x_inv_old_old);
    BN_NEW(bn_x_temp);
    BN_NEW(bn_y_temp);
    BN_NEW(quotient);
    BN_NEW(remainder);
    BN_DEC2BN(1, bn_x_inv_new);
    BN_COPY(bn_x_temp, bn_x);
    BN_COPY(bn_y_temp, bn_y);
    int32_t sign_flag = 0;
    while (1 != BN_EQ_Y(bn_x_temp, 1)) {
        BN_div(bn_y_temp, bn_x_temp, quotient, remainder);
        BN_COPY(bn_y_temp, bn_x_temp);
        BN_COPY(bn_x_temp, remainder);
        BN_COPY(bn_x_inv_old, bn_x_inv_new);
        BN_mul(bn_x_inv_new, quotient, bn_y, bn_x_inv_new);
        BN_add(bn_x_inv_old_old, bn_x_inv_new, bn_x_inv_new);
        BN_COPY(bn_x_inv_old_old, bn_x_inv_old);
        sign_flag ^= 1;
    }
    if (1 == sign_flag) {
        BN_sub(bn_y, bn_x_inv_new, bn_x_inv_new);
    }
    BN_COPY(bn_x_inv, bn_x_inv_new);
    BN_DEL(bn_x_inv_new);
    BN_DEL(bn_x_inv_old);
    BN_DEL(bn_x_inv_old_old);
    BN_DEL(bn_x_temp);
    BN_DEL(bn_y_temp);
    BN_DEL(quotient);
    BN_DEL(remainder);
    return EXIT_SUCCESS;
}

#define PRIME_N 7

const static uint64_t prime[PRIME_N] = { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 };

SECTION_FLASH int32_t BN_Miller_Rabin(const BN *bn_x) {
    uint64_t r = 0;
    int32_t result = EXIT_SUCCESS;
    BN *d, *bn_dec_1, *bn_x_minus_1;
    BN_NEW(d);
    BN_NEW(bn_dec_1);
    BN_NEW(bn_x_minus_1);
    BN_DEC2BN(1, bn_dec_1);
    BN_sub(bn_x, bn_dec_1, bn_x_minus_1);
    BN_COPY(d, bn_x_minus_1);
    while (!(1 & d->data[0])) {
        r++;
        BN_div2(d, d);
    }
    BN *bn_a;
    BN_NEW(bn_a);
    for (int32_t i = 0; i < PRIME_N; i++) {
        BN_DEC2BN(prime[i], bn_a);
        BN_pow_fast(bn_a, d, bn_x, bn_a);
        if (BN_EQ_Y(bn_a, 0) || BN_EQ_Y(bn_a, 1) || (0 == BN_cmp(bn_a, bn_x_minus_1))) {
            continue;
        }
        for (int32_t j = 0; j < r; j++) {
            BN_mul(bn_a, bn_a, bn_x, bn_a);
            if (0 == BN_cmp(bn_a, bn_x_minus_1) && j != r - 1) {
                BN_DEC2BN(1, bn_a);
                break;
            }
            if (BN_EQ_Y(bn_a, 1)) {
                result = EXIT_FAILURE;
                goto END;
            }
        }
        if (!BN_EQ_Y(bn_a, 1)) {
            result = EXIT_FAILURE;
            goto END;
        }
    }
    END:
    BN_DEL(d);
    BN_DEL(bn_dec_1);
    BN_DEL(bn_x_minus_1);
    BN_DEL(bn_a);
    return result;
}

SECTION_FLASH int32_t BN_rand(BN *bn_x, const int32_t uint64_length) {
    BN_INIT(bn_x);
    uint32_t random_num = 0;
    for (int32_t i = 0; i != uint64_length; i++) {
        for (int32_t j = 2; j--;) {
            bn_x->data[i] <<= 32;
            HAL_RNG_GenerateRandomNumber(&hrng, &random_num);
            bn_x->data[i] |= random_num;
        }
    }
    bn_x->data[uint64_length - 1] |= 0x8000000000000000;
    bn_x->data[0] |= 1;
    bn_x->uint64_length = uint64_length;
    return EXIT_SUCCESS;
}
