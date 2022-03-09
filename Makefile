.POSIX:
.SUFFIXES:
.PHONY: all objs help info clean
.SILENT: setup_dirs help info clean
.DEFAULT_GOAL := all

# Build macros
# ============
release = 0

CC      = clang
CFLAGS  = -Wall -Wextra -std=c18
LDFLAGS =
INCLUDE =

EXTRA_CFLAGS  = -fdata-sections -ffunction-sections
EXTRA_LDFLAGS =

# Include $(INCLUDE) into $(CFLAGS) for compilation linking
CFLAGS += $(INCLUDE)

# Include LLVM flags into $(CFLAGS) and $(LDFLAGS)
CFLAGS += $(shell llvm-config --cflags)
LDFLAGS += $(shell llvm-config --libs --cflags --ldflags)

# Add extra flags for GCC
ifeq ($(CC),gcc)
	EXTRA_LDFLAGS += -Wl,--gc-sections
endif

# Check if building for release so we can add optimization flags and debug symbols when needed
ifeq ($(release),1)
	# -O2 : Moderate level of optimization which enables most optimizations.
	CFLAGS += -O2
else
	# -g3 : include extra debugging information, such as macros definitions present in the program.
	# -O0 : “no optimization”, this level compiles the fastest and generates the most debuggable code.
	CFLAGS += -g3 -O0
endif

# Files and Directories
# =====================
# Directories
SRC_DIR       = $(CURDIR)/src
BUILD_DIR     = $(CURDIR)/build
BUILD_BIN_DIR = $(BUILD_DIR)/bin
BUILD_OBJ_DIR = $(BUILD_DIR)/.objects

# Output executable
ifeq ($(OS), Windows_NT)
BUILD_BIN_PATH = $(BUILD_BIN_DIR)/sun
	BUILD_BIN_PATH = $(BUILD_BIN_DIR)/sun.exe
else
	BUILD_BIN_PATH = $(BUILD_BIN_DIR)/sun
endif

# Files patterns
SRC_FILES = .c
HDR_FILES = .h
# Recursively get all C source files from src directory
SOURCES   = $(foreach d,$(SRC_DIR),$(wildcard $(addprefix $(d)/*,$(SRC_FILES))))
# Recursively get all C header files from src directory
HEADERS   = $(foreach d,$(SRC_DIR),$(wildcard $(addprefix $(d)/*,$(HDR_FILES))))
# Set the produced objects path to build objects directory
OBJS      = $(addprefix $(BUILD_OBJ_DIR)/,$(notdir $(addsuffix .o, $(basename $(SOURCES)))))

# Rules to generate object Files
# ==============================
objs: $(OBJS)

$(BUILD_OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@

all: sun

setup_dirs:
ifeq ($(OS),Windows_NT)
	pwsh -c 'if (!(Test-Path "$(BUILD_BIN_DIR)")) { mkdir "$(BUILD_BIN_DIR)" }'
	pwsh -c 'if (!(Test-Path "$(BUILD_OBJ_DIR)")) { mkdir "$(BUILD_OBJ_DIR)" }'
else
	if [ ! -d "$(BUILD_BIN_DIR)" ]; then mkdir -p "$(BUILD_BIN_DIR)"; fi
	if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir -p "$(BUILD_OBJ_DIR)"; fi
endif

sun: setup_dirs $(OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) $(OBJS) -o $(BUILD_BIN_PATH)
	@echo
	@echo "Produced Sun binary is located at '$(BUILD_BIN_PATH)'"

clean:
ifeq ($(OS),Windows_NT)
	pwsh -c '"$(OBJS)".split(" ") | rm -fo'
	pwsh -c 'rm "$(BUILD_BIN_PATH)"'
else
	rm -f $(OBJS) $(BUILD_BIN_PATH)
endif

rebuild: clean $(OBJS) sun

help:
	echo "Sun Programming Language Makefile"
	echo "Sun is owned by NTBBloodbath and licensed under GPLv3 License"
	echo
	echo "Usage: make [TARGET]"
	echo
	echo "Targets:"
	echo -e "\tall\tCompile and link Sun, default target"
	echo -e "\tobjs\tCompile Sun (not linking)"
	echo -e "\tclean\tClean objects and executable Sun binary"
	echo -e "\trebuild Runs clean and all targets"
	echo -e "\thelp\tShow this message"
	echo -e "\tinfo\tShow debugging build information"
	echo
	echo "Report bugs at https://github.com/NTBBloodbath/sun/issues/new"

info:
	echo "SRC_DIR       : $(SRC_DIR)"
	echo "BUILD_DIR     : $(BUILD_DIR)"
	echo "BUILD_OBJ_DIR : $(BUILD_OBJ_DIR)"
	echo "BUILD_BIN_DIR : $(BUILD_BIN_DIR)"
	echo "BUILD_BIN_PATH: $(BUILD_BIN_PATH)"
	echo "SOURCES       : $(SOURCES)"
	echo "HEADERS       : $(HEADERS)"
	echo "OBJS          : $(OBJS)"
	echo
	echo "CC            : $(CC)"
	echo "CFLAGS        : $(CFLAGS)"
	echo "LDFLAGS       : $(LDFLAGS)"
	echo "INCLUDE       : $(INCLUDE)"
	echo "EXTRA_CFLAGS  : $(EXTRA_CFLAGS)"
	echo "EXTRA_LDFLAGS : $(EXTRA_LDFLAGS)"
