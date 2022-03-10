// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_NEW_GEN_H
#define SUN_NEW_GEN_H

#include "ast.h"
#include <llvm-c/Types.h>

LLVMValueRef new_gen_ast(struct sun_ast_node_st *tree, LLVMModuleRef module,
                         LLVMBuilderRef builder);
#endif
