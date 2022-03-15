// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <cstdlib>
#include <iostream>

#include "../third-party/rang/rang.hpp"

namespace sun::logger {
    void dbg(std::string dbg_message) {
        std::cerr << rang::fg::gray << rang::style::bold << "debug: " << rang::style::reset
                  << dbg_message << std::endl;
    }

    void err(std::string err_message) {
        std::cerr << rang::fg::red << rang::style::bold << "error: " << rang::style::reset
                  << err_message << std::endl;
    }

    void err_fatal(std::string err_message) {
        std::cerr << rang::fg::red << rang::style::bold << "fatal: " << rang::style::reset
                  << err_message << std::endl;
        std::exit(1);
    }
} // namespace sun::logger
