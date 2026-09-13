# Build configuration
# BUILD_MODE = BUILD_MODE_DEV or BUILD_MODE_TEST or BUILD_MODE_RELEASE
# In BUILD_MODE_DEV, the kernel is compiled with debug symbols.
# In BUILD_MODE_TEST, the kernel is compiled with debug symbols and also includes unit tests.
# In BUILD_MODE_RELEASE, the kernel is compiled with no debug symbols and no tests.
BUILD_MODE_DEV			= __dev__
BUILD_MODE_TEST			= __test__
BUILD_MODE_RELEASE		= __release__
BUILD_MODE			= $(BUILD_MODE_RELEASE)

ifeq ($(filter $(BUILD_MODE),$(BUILD_MODE_DEV) $(BUILD_MODE_TEST) $(BUILD_MODE_RELEASE)),)
$(error Build mode "$(BUILD_MODE)" not supported)
endif

# Tools
# It is strongly recommended to use the LLVM toolchain.
C_COMPILER			= x86_64-radishos-clang
CXX_COMPILER			= x86_64-radishos-clang++
NASM				= nasm
LINKER				= x86_64-radishos-clang++

# Directories
TEST_DIR			= test
INCLUDE_DIR			= include

# Flags
COMMON_FLAGS			= -ffreestanding \
				-fno-stack-protector \
				-fno-stack-check \
				-fno-lto \
				-fno-pic \
				-fno-pie \
				-mno-red-zone \
				-Wall \
				-Wextra \
				-nostdlib \
				-nostdinc \
				-mcmodel=kernel

ifeq ($(BUILD_MODE),$(BUILD_MODE_DEV))
COMMON_FLAGS += -g
endif
ifeq ($(BUILD_MODE),$(BUILD_MODE_TEST))
COMMON_FLAGS += -g -I../$(TEST_DIR) -DKIWI_BUILD_MODE_TEST
endif

C_FLAGS				= $(COMMON_FLAGS) \
				-std=c23 \
				-fno-builtin \
				-fvisibility=hidden

CXX_FLAGS			= $(COMMON_FLAGS) \
				-std=c++23 \
				-fno-exceptions \
				-fno-rtti \
				-fno-threadsafe-statics \
				-fno-use-cxa-atexit \
				-fno-unwind-tables \
				-fno-asynchronous-unwind-tables \
				-fno-builtin \
				-fvisibility=hidden \
				-fno-sized-deallocation

NASM_FLAGS			= -f elf64

LINKER_FLAGS			= -static \
				-nostdlib \
				-Wl,--build-id=none \
				-z max-page-size=0x1000 \
				-T kernel.lds \
				-fuse-ld=lld