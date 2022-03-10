// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_NEW_CG_H
#define SUN_NEW_CG_H

#include "ast.h"
#include <llvm-c/Core.h>

LLVMValueRef sun_cg(struct sun_ast_node_st *node, LLVMModuleRef module, LLVMBuilderRef builder);
LLVMValueRef sun_cg_number(struct sun_ast_node_st *node);
LLVMValueRef sun_cg_bin_expr(struct sun_ast_node_st *node, LLVMModuleRef module,
                             LLVMBuilderRef builder);
#endif
