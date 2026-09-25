include ../config.mk

C_FLAGS += -I../$(INCLUDE_DIR)
CXX_FLAGS += -I../$(INCLUDE_DIR)

SOURCE_DIR              = .
OBJECTS_DIR             = ../build/fs
VFS_DIR			= $(SOURCE_DIR)/vfs
USTAR_DIR		= $(SOURCE_DIR)/ustar
TMPFS_DIR		= $(SOURCE_DIR)/tmpfs
DEVFS_DIR		= $(SOURCE_DIR)/devfs

CXX_FILES               = $(SOURCE_DIR)/path.cpp

CXX_OBJECTS             = $(patsubst ./%, $(OBJECTS_DIR)/%, $(CXX_FILES:.cpp=.o))

.PHONY: all clean

all: subdirs

subdirs: $(CXX_OBJECTS)
	$(MAKE) -C $(VFS_DIR)
	$(MAKE) -C $(USTAR_DIR)
	$(MAKE) -C $(TMPFS_DIR)
	$(MAKE) -C $(DEVFS_DIR)

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX_COMPILER) $(CXX_FLAGS) -c $< -o $@

clean:
	$(MAKE) -C $(VFS_DIR) clean
	$(MAKE) -C $(USTAR_DIR) clean
	$(MAKE) -C $(TMPFS_DIR) clean
	$(MAKE) -C $(DEVFS_DIR) clean
	rm -rf $(OBJECTS_DIR)