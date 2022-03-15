// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <iostream>

#include "../third-party/argparse/argparse.hpp"

namespace sun::cli {
    // CLI private constants
    static const std::string VERSION("0.1.0a");
    static const std::string
        COPYRIGHT("Copyright (c) 2022 NTBBloodbath. Sun is distributed under MIT license.");

    // Initialize CLI
    argparse::ArgumentParser init(int argc, char *argv[]) {
        // '--version' output
        static const std::string version_output("Sun compiler v" + VERSION + "\n" + COPYRIGHT +
                                                "\n");
        // Argparse
        static argparse::ArgumentParser cli("sun", version_output);
        cli.add_argument("[FILE] ...").help("Sun source code file").remaining();

        try {
            cli.parse_args(argc, argv);
        } catch (const std::runtime_error &err) {
            std::cerr << err.what() << std::endl;
            std::cerr << cli;
            std::exit(1);
        }

        return cli;
    }
} // namespace sun::cli
