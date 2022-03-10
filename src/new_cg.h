// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#ifndef SUN_NEW_CG_H
#define SUN_NEW_CG_H

#include <llvm-c/Core.h>
#include "ast.h"

LLVMValueRef sun_cg_i32(struct AST_node *node);
LLVMValueRef sun_cg_i64(struct AST_node *node);
LLVMValueRef sun_cg_u32(struct AST_node *node);
LLVMValueRef sun_cg_u64(struct AST_node *node);
#endif
