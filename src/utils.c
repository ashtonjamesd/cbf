#include "common.h"

char *bf_op_type_to_str(BfOpType c) {
    switch (c) {
        case BF_INC_DATA: return "+";
        case BF_DEC_DATA: return "-";
        case BF_INC_PTR: return ">";
        case BF_DEC_PTR: return "<";
        case BF_OUTPUT: return ".";
        case BF_INPUT: return ",";
        case BF_LOOP: return "[]";
        default: return "unknown";
    }
}