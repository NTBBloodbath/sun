// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <llvm-c/Analysis.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>

#include "globals.h"
#include "new_ast.h"
#include "new_cg.h"

LLVMValueRef sun_cg(struct sun_ast_node_st *node, LLVMModuleRef module, LLVMBuilderRef builder);

// TODO: convert codegen functions for integers into static functions and
//       use them as wrappers for sun_cg_number one once we start coding it

/**
 * @brief Generate LLVM IR code for signed 32 bits integers
 *
 * @param node [in] AST Node for i32 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_i32(struct sun_ast_node_st *node) {
    return LLVMConstInt(LLVMInt32Type(), node->number.value, 1);
}

/**
 * @brief Generate LLVM IR code for signed 64 bits integers
 *
 * @param node [in] AST Node for i64 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_i64(struct sun_ast_node_st *node) {
    return LLVMConstInt(LLVMInt64Type(), node->number.value, 1);
}

/**
 * @brief Generate LLVM IR code for unsigned 32 bits integers
 *
 * @param node [in] AST Node for u32 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_u32(struct sun_ast_node_st *node) {
    return LLVMConstInt(LLVMInt32Type(), node->number.value, 0);
}

/**
 * @brief Generate LLVM IR code for unsigned 64 bits integers
 *
 * @param node [in] AST Node for u64 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_u64(struct sun_ast_node_st *node) {
    return LLVMConstInt(LLVMInt64Type(), node->number.value, 0);
}

LLVMValueRef sun_cg_bin_expr(struct sun_ast_node_st *node, LLVMModuleRef module,
                             LLVMBuilderRef builder) {
    LLVMValueRef lhs = sun_cg(node->bin_expr.lhs, module, builder);
    LLVMValueRef rhs = sun_cg(node->bin_expr.rhs, module, builder);

    // Return NULL if oen of the expression sides is invalid
    if (lhs == NULL || rhs == NULL) {
        return NULL;
    }

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
            break;
    }

    return NULL;
}

LLVMValueRef sun_cg(struct sun_ast_node_st *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    switch (node->type) {
        case NUMBER:
            // TODO: use 'sun_cg_number(struct sun_ast_node_st *node)' function once it have been
            // made
            return sun_cg_i64(node);
            break;
        case BIN_EXPR:
            return sun_cg_bin_expr(node, module, builder);
            break;
            break;
    }

    return NULL;
}
