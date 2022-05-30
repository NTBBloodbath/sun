// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../third-party/argparse/argparse.hpp"
#include "cli.hpp"
#include "logger.hpp"
#include "state.hpp"
#include "scanner.hpp"

int main(int argc, char *argv[]) {
    // ===== Initialize CLI
    argparse::ArgumentParser cli = sun::cli::init(argc, argv);

    // Get source code files from stdin
    std::vector<std::string> files;
    try {
        files = cli.get<std::vector<std::string>>("[FILE] ...");
    } catch (std::logic_error &err) {
        // Raise an error and print help message if no files were passed
        sun::logger::err("No source code files provided");
        std::cout << cli.help().str();
    }

    // TODO(ntbbloodbath): scan every passed file instead of just first one
    std::string input_file_name(files[0]);
    std::ifstream file(input_file_name);
    if (!file)
        sun::logger::err_fatal("Unable to open '" + input_file_name + "' file: " + strerror(errno));

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string sun_file_contents = ss.str();

    // Initialize compiler state
    State state = {
        0,                 // current line
        0,                 // file position
        sun_file_contents, // source files
        "out.bc",          // TODO: parse '--output' CLI argument
    };

    // std::cout << state.sun_source_file << std::endl;
    scan(state);
    std::exit(0);
}
