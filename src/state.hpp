// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#pragma once
#include <iostream>

using std::string;

struct State {
    int current_ln = 0;
    int file_pos = 0;
    string sun_source_file;
    string sun_out_file;
};
