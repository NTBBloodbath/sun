// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#include <cstdlib>
#include <iostream>

#include "../third-party/rang/rang.hpp"

/**
 * @brief Sun Logger
 *
 * @namespace sun::logger
 */
namespace sun::logger {
    /**
     * @brief Send a debugging log to stderr
     *
     * @param[in] dbg_message Debug message to be shown
     */
    void dbg(std::string dbg_message) {
        std::cerr << rang::fg::gray << rang::style::bold << "debug: " << rang::style::reset
                  << dbg_message << std::endl;
    }

    /**
     * @brief Send an error log to stderr
     *
     * @param[in] err_message Error message to be shown
     */
    void err(std::string err_message) {
        std::cerr << rang::fg::red << rang::style::bold << "error: " << rang::style::reset
                  << err_message << std::endl;
    }

    /**
     * @brief Send an error log to stderr and exit program
     *
     * @param[in] err_message Error message to be shown
     */
    void err_fatal(std::string err_message) {
        std::cerr << rang::fg::red << rang::style::bold << "fatal: " << rang::style::reset
                  << err_message << std::endl;
        std::exit(1);
    }
} // namespace sun::logger
