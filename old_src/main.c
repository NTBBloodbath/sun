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
#include <llvm-c/Analysis.h>
#include <llvm-c/BitWriter.h>
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
#include "ast.h"
#include "cli.h"
#include "gen.h"
#include "scanner.h"
#include "tokenizer.h"

static void init_compiler() {
    curr_line = 1;
    go_back = '\n';

    sun_builder = LLVMCreateBuilder();

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
    // Verify generated LLVM IR module
    char *err = NULL;
    LLVMVerifyModule(sun_mod, LLVMAbortProcessAction, &err);
    LLVMDisposeMessage(err);

    // Dump a representation of sun module to stderr
    printf("===== LLVM IR code:\n");
    LLVMDumpModule(sun_mod);

    // Free pass pipeline memory
    LLVMDisposePassManager(pass_manager);
    // Free builder memory
    LLVMDisposeBuilder(sun_builder);
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
    sun_out_file = fopen("out.bc", "w");
    if (sun_out_file == NULL) {
        fprintf(stderr, "Unable to create 'out.bc' file: %s\n", strerror(errno));
        exit(1);
    }

    // Get the first token from input file
    scan(&Token);
    struct sun_ast_node_st *tree = bin_expr(0);
    LLVMValueRef generated_llvm = gen_ast(tree, sun_mod, sun_builder);

    // Write module bytecode to output file
    if (LLVMWriteBitcodeToFile(sun_mod, "out.bc") != 0) {
        fprintf(stderr, "Error writing bitcode to file\n");
        exit(1);
    }
    // Close input/output files
    fclose(sun_file);
    // fclose(sun_out_file);

    // Free all AST nodes
    free_ast_node(tree);

    // Stop compiler, send LLVM IR to stderr and free LLVM builder, engine and pass manager
    stop_compiler();
    LLVMDumpValue(generated_llvm);
    printf("\n");

    exit(0);
}
