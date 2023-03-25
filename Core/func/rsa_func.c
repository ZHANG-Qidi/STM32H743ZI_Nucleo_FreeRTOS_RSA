#include "rsa_func.h"

#include "common_func.h"
#include "usart_func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static BN *p, *q, *p1, *p2, *phi;
static BN *p_minus_1, *q_minus_1;
static BN *e, *n, *d, *dP, *dQ, *qInv;

SECTION_FLASH int32_t rsa_init_new(void) {
    BN_NEW(e);
    BN_NEW(p1);
    BN_NEW(p2);
    BN_NEW(p);
    BN_NEW(q);
    BN_NEW(n);
    BN_NEW(p_minus_1);
    BN_NEW(q_minus_1);
    BN_NEW(phi);
    BN_NEW(d);
    BN_NEW(dP);
    BN_NEW(dQ);
    BN_NEW(qInv);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t rsa_init(void) {
    BN *bn_1;
    BN_NEW(bn_1);
    BN_DEC2BN(1, bn_1)
    BN_DEC2BN(0x10001, e);
    if (1 == BN_cmp(p1, p2)) {
        BN_COPY(p, p1);
        BN_COPY(q, p2);
    } else {
        BN_COPY(p, p2);
        BN_COPY(q, p1);
    }
    BN_sub(p, bn_1, p_minus_1);
    BN_sub(q, bn_1, q_minus_1);
    for (int32_t i = 0; i != BIG_NUM_SIZE - 1; i++) {
        n->data[i] = 0xFFFFFFFFFFFFFFFF;
    }
    n->uint64_length = BN_uint64_length(n);
    BN_mul(p, q, n, n);
    BN_mul(p_minus_1, q_minus_1, n, phi);
    BN_Extended_Euclid_Algorithm(e, phi, d);
    BN_Extended_Euclid_Algorithm(e, p_minus_1, dP);
    BN_Extended_Euclid_Algorithm(e, q_minus_1, dQ);
    BN_Extended_Euclid_Algorithm(q, p, qInv);
    BN_DEL(bn_1);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t rsa_Encrypt(const BN *bn_x, const BN *e, const BN *n, BN *bn_y) {
    BN_pow_fast(bn_x, e, n, bn_y);
    return EXIT_SUCCESS;
}

SECTION_FLASH int32_t rsa_Decrypt(const BN *bn_x, const BN *d, const BN *n, BN *bn_y) {
    BN *m1, *m2, *h, *temp;
    BN_NEW(m1);
    BN_NEW(m2);
    BN_NEW(h);
    BN_NEW(temp);
    BN_div_mod(bn_x, p, temp);
    BN_pow_fast(temp, dP, p, m1);
    BN_div_mod(bn_x, q, temp);
    BN_pow_fast(temp, dQ, q, m2);
    if (1 == BN_cmp(m1, m2)) {
        BN_sub(m1, m2, temp);
    } else {
        BN_sub(m2, m1, temp);
        BN_sub(p, temp, temp);
    }
    BN_mul(qInv, temp, p, h);
    BN_mul(h, q, n, temp);
    BN_add(temp, m2, bn_y);
    if (1 == BN_cmp(bn_y, n)) {
        BN_sub(bn_y, n, bn_y);
    }
    BN_DEL(m1);
    BN_DEL(m2);
    BN_DEL(h);
    BN_DEL(temp);
    return EXIT_SUCCESS;
}

int32_t rsa(const BN *bn_x, const int32_t flag, BN *bn_y) {
    if (flag) {
        rsa_Encrypt(bn_x, e, n, bn_y);
    } else {
        rsa_Decrypt(bn_x, d, n, bn_y);
    }
    return EXIT_SUCCESS;
}

int32_t rsa_set_prime(BN **bn) {
    BN_COPY(p1, bn[0]);
    BN_COPY(p2, bn[1]);
    return EXIT_SUCCESS;
}

int32_t rsa_usart(const char *str) {
    int32_t rsa_flag;
    if ('p' == str[0]) {
        rsa_flag = 1;
    } else if ('c' == str[0]) {
        rsa_flag = 0;
    } else {
        return EXIT_FAILURE;
    }
    BN *bn_input, *bn_output;
    BN_NEW(bn_input);
    BN_NEW(bn_output);
    BN_str2bn(str, bn_input);
    char *str_temp = calloc(1024, sizeof(int8_t));
    rsa(bn_input, rsa_flag, bn_output);
    BN_bn2str(str_temp, bn_output);
    if (rsa_flag) {
        usart3_printf("c = %s\n", str_temp);
    } else {
        usart3_printf("p = %s\n", str_temp);
    }
    free(str_temp);
    BN_DEL(bn_input);
    BN_DEL(bn_output);
    return EXIT_SUCCESS;
}

int32_t rsa_print_key(void) {
    char *str_temp = calloc(1024, sizeof(char));
    BN_bn2str(str_temp, p);
    usart3_printf("p = %s\n\n", str_temp);
    BN_bn2str(str_temp, q);
    usart3_printf("q = %s\n\n", str_temp);
    BN_bn2str(str_temp, n);
    usart3_printf("n = %s\n\n", str_temp);
    BN_bn2str(str_temp, e);
    usart3_printf("e = %s\n\n", str_temp);
    BN_bn2str(str_temp, d);
    usart3_printf("d = %s\n\n", str_temp);
    BN_bn2str(str_temp, dP);
    usart3_printf("dP = %s\n\n", str_temp);
    BN_bn2str(str_temp, dQ);
    usart3_printf("dQ = %s\n\n", str_temp);
    BN_bn2str(str_temp, qInv);
    usart3_printf("qInv = %s\n", str_temp);
    free(str_temp);
    return EXIT_SUCCESS;
}
