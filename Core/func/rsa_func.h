#ifndef _RSA_GRUNT_H_
#define _RSA_GRUNT_H_

#include "bignum_func.h"

int32_t rsa_init_new(void);
int32_t rsa_init(void);
int32_t rsa_Encrypt(const BN *bn_x, const BN *e, const BN *n, BN *bn_y);
int32_t rsa_Decrypt(const BN *bn_x, const BN *d, const BN *n, BN *bn_y);
int32_t rsa(const BN *bn_x, const int32_t flag, BN *bn_y);
int32_t rsa_usart(const char *str);
int32_t rsa_print_key(void);
int32_t rsa_set_prime(BN **bn);

#endif
