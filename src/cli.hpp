// ┌                                                          ┐
// │  Copyright (c) 2022 NTBBloodbath. All rights reserved.   │
// │  Use of this source code is governed by an MIT license   │
// │          that can be found in the LICENSE file.          │
// └                                                          ┘
#pragma once
#include "../third-party/argparse/argparse.hpp"

namespace sun::cli {
   argparse::ArgumentParser init(int argc, char *argv[]);
}
