# The gcc and flags
CC_PREFIX_LINUX = riscv64-unknown-linux-gnu

DIR_PWD    		= $(abspath .)

DIR_SUDO		= $(DIR_PWD)/sudo
DIR_EXPOLIT		= $(DIR_PWD)/CVE-2021-3156
DIR_BUILD  		= $(DIR_PWD)/build

TARGET_SUDO		= $(DIR_PWD)/sudo/src/sudo
TARGET_EXPOLIT  = $(DIR_BUILD)/expolit

.PYON: all

all: build sudo expolit

build:
	mkdir -p $(DIR_BUILD)

# Compile sudo
sudo:  build
	cd $(DIR_SUDO) && \
	./configure --prefix=$(DIR_BUILD) --host=riscv64 CXX=$(CC_PREFIX_LINUX)-g++ CC=$(CC_PREFIX_LINUX)-gcc
	make -C $(DIR_SUDO) -j`nproc`

# Compile expolit
expolit:  build
	make -C $(DIR_EXPOLIT)
	mv $(DIR_EXPOLIT)/exploit $(DIR_BUILD)
	mv $(DIR_EXPOLIT)/libnss_x-x.so.2 $(DIR_BUILD)

clean:
	rm -rf $(DIR_BUILD)
	make -C $(DIR_SUDO) clean
