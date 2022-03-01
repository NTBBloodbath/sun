#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <llvm-c/Core.h>
#include <llvm-c/Analysis.h>

#include "ast.h"

/*
 * @brief Generate a LLVM value for a number AST
 *
 * @param node [in] The AST node
 * @return LLVM value reference
 */
LLVMValueRef sun_cg_int(struct AST_node *node) {
    return LLVMConstReal(LLVMDoubleType(), node->int_value);
}

LLVMValueRef sun_cg(struct AST_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    // TODO: Make a more robust AST with typedefs and unions
    // so I can actually finish this code with easiness
    // switch (node->type) {
    // }
    return NULL;
}

/*
 * @brief Generate a LLVM value for a binary expression AST
 *
 * @param node [in] The AST node
 * @return LLVM value reference
 */
LLVMValueRef sun_cg_bin_expr(struct AST_node *node, LLVMModuleRef module, LLVMBuilderRef builder) {
    // Evaluate lhs and rhs values
    LLVMValueRef lhs = sun_cg(node->left, module, builder);
    LLVMValueRef rhs = sun_cg(node->right, module, builder);

    if (lhs == NULL || rhs == NULL) {
        return NULL;
    }

    // Create LLVM IR code depending on the operator
    switch (node->op) {
        case A_ADD: {
            return LLVMBuildFAdd(builder, lhs, rhs, "add");
        }
        case A_MINUS: {
            return LLVMBuildFSub(builder, lhs, rhs, "sub");
        }
        case A_MULTIPLY: {
            return LLVMBuildFMul(builder, lhs, rhs, "mul");
        }
        case A_DIVIDE: {
            return LLVMBuildFDiv(builder, lhs, rhs, "div");
        }
    }

    return NULL;
}
