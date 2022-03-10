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

/**
 * @brief Generate LLVM IR code for signed 32 bits integers
 *
 * @param node [in] AST Node for i32 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_i32(struct AST_node *node) {
    return LLVMConstInt(LLVMInt32Type(), node->value.int_value, 1);
}

/**
 * @brief Generate LLVM IR code for signed 64 bits integers
 *
 * @param node [in] AST Node for i64 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_i64(struct AST_node *node) {
    return LLVMConstInt(LLVMInt64Type(), node->value.int_value, 1);
}

/**
 * @brief Generate LLVM IR code for unsigned 32 bits integers
 *
 * @param node [in] AST Node for u32 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_u32(struct AST_node *node) {
    return LLVMConstInt(LLVMInt32Type(), node->value.int_value, 0);
}

/**
 * @brief Generate LLVM IR code for unsigned 64 bits integers
 *
 * @param node [in] AST Node for u64 type
 * @return Value reference to integer
 */
LLVMValueRef sun_cg_u64(struct AST_node *node) {
    return LLVMConstInt(LLVMInt64Type(), node->value.int_value, 0);
}

LLVMValueRef sun_cg_bin_expr(struct SUN_AST_NODE *node, LLVMModuleRef module,
                             LLVMBuilderRef builder) {
    LLVMValueRef lhs = sun_cg(node->bin_expr.lhs, module, builder);
    LLVMValueRef rhs = sun_cg(node->bin_expr.rhs, module, builder);

    // Return NULL if oen of the expression sides is invalid
    if (lhs == NULL || rhs == NULL) {
        return NULL;
    }

    switch (node->bin_expr.operator) {
        case SUN_BINOP_ADD:
            return LLVMBuildFAdd(builder, lhs, rhs, "addtmp");
            break;
        case SUN_BINOP_SUB:
            return LLVMBuildFSub(builder, lhs, rhs, "subtmp");
            break;
        case SUN_BINOP_MUL:
            return LLVMBuildFMul(builder, lhs, rhs, "multmp");
            break;
        case SUN_BINOP_DIV:
            return LLVMBuildFDiv(builder, lhs, rhs, "divtmp");
            break;
            break;
    }

    return NULL;
}
