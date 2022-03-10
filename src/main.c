// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
// Microsoft has deprecated lots of calls which use buffers to improve code security.
// Using this line to remove deprecation notice
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif

#include <errno.h>
#include <llvm-c/Core.h>
#include <llvm-c/ExecutionEngine.h>
#include <llvm-c/Target.h>
#include <llvm-c/Transforms/Scalar.h>
#include <llvm-c/Transforms/Utils.h>
#include <llvm-c/Types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define extern_
#include "globals.h"
#undef extern_
#include "cli.h"
#include "ast.h"
#include "new_gen.h"
#include "scanner.h"
#include "tokenizer.h"

static void init_compiler() {
    curr_line = 1;
    go_back = '\n';

    builder = LLVMCreateBuilder();

    // LLVMInitializeNativeTarget();
    // LLVMLinkInMCJIT();

    // Create engine for execution
    char *msg;
    if (LLVMCreateExecutionEngineForModule(&engine, sun_mod, &msg) == 1) {
        fprintf(stderr, "%s\n", msg);
        LLVMDisposeMessage(msg);
        exit(1);
    }

    /* Setup optimizations */
    pass_manager = LLVMCreateFunctionPassManagerForModule(sun_mod);
    // Promote allocas to registers
    LLVMAddPromoteMemoryToRegisterPass(pass_manager);
    // peephole optimizations
    LLVMAddInstructionCombiningPass(pass_manager);
    // Reassociate expressions
    LLVMAddReassociatePass(pass_manager);
    // Eliminate common subexprs
    LLVMAddGVNPass(pass_manager);
    // Simplify control flow graph, e.g. delete unreachable blocks
    LLVMAddCFGSimplificationPass(pass_manager);

    LLVMInitializeFunctionPassManager(pass_manager);
}

static void stop_compiler() {
    // Dump a representation of sun module to stderr
    printf("===== LLVM IR code:\n");
    LLVMDumpModule(sun_mod);

    // Free pass pipeline memory
    LLVMDisposePassManager(pass_manager);
    // Free builder memory
    LLVMDisposeBuilder(builder);
    // Free execution engine memory
    LLVMDisposeExecutionEngine(engine);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        help();
        exit(1);
    }

    // Create LLVM module for sun source code file and initialize compiler
    sun_mod = LLVMModuleCreateWithName(argv[1]);
    init_compiler();

    // Open input file
    sun_file = fopen(argv[1], "r");
    if (sun_file == NULL) {
        fprintf(stderr, "Unable to open '%s' file: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    // Create output file
    sun_out_file = fopen("out.s", "w");
    if (sun_out_file == NULL) {
        fprintf(stderr, "Unable to create 'out.sun' file: %s\n", strerror(errno));
        exit(1);
    }

    // Get the first token from input file
    scan(&Token);
    struct sun_ast_node_st *ast;
    ast = bin_expr(0);
    new_gen_ast(ast, sun_mod, builder);
    // Set preamble in output file
    // gen_preamble();
    // Parse statements in input file
    // statements();
    // Set postamble in output file
    // gen_postamble();
    // Close input/output file
    fclose(sun_file);
    fclose(sun_out_file);

    stop_compiler();
    exit(0);
}
