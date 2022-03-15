// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#pragma once
#include <iostream>

namespace sun::logger {
    void dbg(std::string dbg_message);

    void err(std::string err_message);

    void err_fatal(std::string err_message);
} // namespace sun::logger
