#ifndef BF_COMMON_H
#define BF_COMMON_H

typedef enum {
    BF_INC_DATA,
    BF_DEC_DATA,
    BF_INC_PTR,
    BF_DEC_PTR,
    BF_OUTPUT,
    BF_INPUT,
    BF_LOOP,
    BF_BAD
} BfOpType;

#endif
