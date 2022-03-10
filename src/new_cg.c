// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "globals.h"
#include "new_cg.h"

/**
 * @brief Generate LLVM IR code for signed 32 bits integers
 *
 * @param node [in] AST Node for i32 type
 * @return Value reference to integer
 */
// static LLVMValueRef sun_cg_i32(struct sun_ast_node_st *node) {
//     return LLVMConstInt(LLVMInt32Type(), node->number.value, 1);
// }

/**
 * @brief Generate LLVM IR code for signed 64 bits integers
 *
 * @param node [in] AST Node for i64 type
 * @return Value reference to integer
 */
static LLVMValueRef sun_cg_i64(struct sun_ast_node_st *node) {
    return LLVMConstInt(LLVMInt64Type(), node->number.value, 1);
}

/**
 * @brief Generate LLVM IR code for unsigned 32 bits integers
 *
 * @param node [in] AST Node for u32 type
 * @return Value reference to integer
 */
// static LLVMValueRef sun_cg_u32(struct sun_ast_node_st *node) {
//     return LLVMConstInt(LLVMInt32Type(), node->number.value, 0);
// }

/**
 * @brief Generate LLVM IR code for unsigned 64 bits integers
 *
 * @param node [in] AST Node for u64 type
 * @return Value reference to integer
 */
// static LLVMValueRef sun_cg_u64(struct sun_ast_node_st *node) {
//     return LLVMConstInt(LLVMInt64Type(), node->number.value, 0);
// }

/**
 * @brief Generate LLVM IR code for integers
 *
 * @param node [in] AST Node for integer type (signed and unsigned)
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_number(struct sun_ast_node_st *node) {
    // TODO: uncomment this code once we start parsing number types
    // char *type = node->number.type;
    // switch (*type) {
    //     case 'i':
    //         if (!strcmp(type, "i32")) {
    //             return sun_cg_i32(node);
    //         }
    //         if (!strcmp(type, "i64")) {
    //             return sun_cg_i64(node);
    //         }
    //         break;
    //     case 'u':
    //         if (!strcmp(type, "u32")) {
    //             return sun_cg_u32(node);
    //         }
    //         if (!strcmp(type, "u64")) {
    //             return sun_cg_u64(node);
    //         }
    //         break;
    // }
    return sun_cg_i64(node);

    // return NULL;
}

LLVMValueRef sun_cg_bin_expr(struct sun_ast_node_st *node, LLVMModuleRef module,
                             LLVMBuilderRef builder) {
    LLVMValueRef lhs = sun_cg(node->bin_expr.lhs, module, builder);
    LLVMValueRef rhs = sun_cg(node->bin_expr.rhs, module, builder);

    // Return NULL if oen of the expression sides is invalid
    if (lhs == NULL || rhs == NULL) {
        return NULL;
    }

    // char *ops[] = {"", "+", "-", "*", "/"};
    // printf("[DEBUG] Binary expression operator: %s\n", ops[node->bin_expr.operator]);
    switch (node->bin_expr.operator) {
        case ADD:
            return LLVMBuildFAdd(builder, lhs, rhs, "addtmp");
            break;
        case SUB:
            return LLVMBuildFSub(builder, lhs, rhs, "subtmp");
            break;
        case MUL:
            return LLVMBuildFMul(builder, lhs, rhs, "multmp");
            break;
        case DIV:
            return LLVMBuildFDiv(builder, lhs, rhs, "divtmp");
            break;
        default:
            fprintf(stderr, "Syntax error: Unknown operator on line %d\n", curr_line);
    }

    return NULL;
}

LLVMValueRef sun_cg(struct sun_ast_node_st *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    switch (node->type) {
        case NUMBER:
            printf("[DEBUG] Got number '%d'\n", node->number.value);
            return sun_cg_number(node);
            break;
        case BIN_EXPR:
            printf("[DEBUG] Got binary expression\n");
            return sun_cg_bin_expr(node, module, builder);
            break;
        default:
            fprintf(stderr, "Got unknown AST node type '%d' on line %d\n", node->type, curr_line);
            exit(1);
    }

    return NULL;
}
