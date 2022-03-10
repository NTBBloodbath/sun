// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <llvm-c/Types.h>
#include <stdio.h>
#include <stdlib.h>

#include "globals.h"
#include "ast.h"
#include "new_cg.h"

/*
 * @brief Generate LLVM IR code recursivrly from given AST
 *
 * @param tree [in] AST node
 * @param module [in] LLVM Module reference
 * @param builder [in] LLVM Builder reference
 * @return LLVM IR code
 */
LLVMValueRef new_gen_ast(struct sun_ast_node_st *tree, LLVMModuleRef module, LLVMBuilderRef builder) {
    // Recursively generate binary expressions
    return sun_cg_bin_expr(tree, module, builder);
}
