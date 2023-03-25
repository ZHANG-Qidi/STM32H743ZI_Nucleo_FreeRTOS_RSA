#include "prime_func.h"

#include "common_func.h"
#include "bignum_func.h"
#include "rsa_func.h"
#include "usart_func.h"
#include <stdio.h>
#include <stdlib.h>

SECTION_ITCM void prime_random(void) {
    BN *bn_dec_2;
    BN_NEW(bn_dec_2);
    BN_DEC2BN(2, bn_dec_2);
    BN *bn_prime[2];
    BN_NEW(bn_prime[0]);
    BN_NEW(bn_prime[1]);
    int32_t rand_reset_count = 0;
    int32_t bn_rand_num = 0;
    int32_t bn_prime_num_cm7 = 0;
    int32_t bn_prime_num[2] = { 0 };
    uint32_t rand_reset = RAND_RESET;
    char *str = calloc(1024, sizeof(char));
    for (bn_prime_num_cm7 = 0; bn_prime_num_cm7 != 2; bn_prime_num_cm7++) {
        BN_rand(bn_prime[bn_prime_num_cm7], RAND_SIZE);
        for (;;) {
            BN_bn2str(str, bn_prime[bn_prime_num_cm7]);
            usart3_printf("M7 %03d %03d %03d: %s\n\n", bn_prime_num_cm7, rand_reset, ++bn_rand_num, str);
            if (EXIT_SUCCESS == BN_Miller_Rabin(bn_prime[bn_prime_num_cm7])) {
                rand_reset = RAND_RESET;
                rand_reset_count = 0;
                bn_prime_num[bn_prime_num_cm7] = bn_rand_num;
                break;
            }
            if (rand_reset == ++rand_reset_count) {
                rand_reset <<= 1;
                rand_reset_count = 0;
                BN_rand(bn_prime[bn_prime_num_cm7], RAND_SIZE);
            } else {
                BN_add(bn_prime[bn_prime_num_cm7], bn_dec_2, bn_prime[bn_prime_num_cm7]);
            }
        }
    }
    usart3_printf("CORE_CM7 DONE\n\n");
    for (int32_t i = 0; i != bn_prime_num_cm7; i++) {
        bn_prime[i]->uint64_length = BN_uint64_length(bn_prime[i]);
        BN_bn2str(str, bn_prime[i]);
        usart3_printf("PRIME M7 %03d %03d: %s\n\n", i, bn_prime_num[i], str);
    }
    rsa_set_prime(bn_prime);
    free(str);
    BN_DEL(bn_dec_2);
    BN_DEL(bn_prime[0]);
    BN_DEL(bn_prime[1]);
}
