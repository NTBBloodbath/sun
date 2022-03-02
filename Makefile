.POSIX:
.SUFFIXES:
.PHONY: all objs help info clean
.SILENT: setup_dirs help info clean
.DEFAULT_GOAL := all

# Build macros
# ============
CC      = gcc
CFLAGS  = -O3 -Wall -Wextra -std=c18
LDFLAGS =
INCLUDE =

EXTRA_CFLAGS  = -fdata-sections -ffunction-sections
EXTRA_LDFLAGS = -Wl,--gc-sections

# Include $(INCLUDE) into $(CFLAGS) for compilation linking
CFLAGS += $(INCLUDE)

# Files and Directories
# =====================
# Directories
SRC_DIR       = $(CURDIR)/src
BUILD_DIR     = $(CURDIR)/build
BUILD_BIN_DIR = $(BUILD_DIR)/bin
BUILD_OBJ_DIR = $(BUILD_DIR)/.objects

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
	if [ ! -d "$(BUILD_BIN_DIR)" ]; then mkdir -p "$(BUILD_BIN_DIR)"; fi
	if [ ! -d "$(BUILD_OBJ_DIR)" ]; then mkdir -p "$(BUILD_OBJ_DIR)"; fi

sun: setup_dirs $(OBJS)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(LDFLAGS) $(EXTRA_LDFLAGS) $(OBJS) -o $(BUILD_BIN_DIR)/sun
	@echo
	@echo "Produced Sun binary is located at '$(BUILD_BIN_DIR)/sun'"

clean:
	rm -f $(OBJS) $(BUILD_BIN_DIR)/sun

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
