KVERS = $(shell uname -r)

# Kernel modules
obj-m += lkm-mem-oop.o

# Specify flags for the module compilation.
EXTRA_CFLAGS= -O0

build: kernel_modules

kernel_modules:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) modules

clean:
	make -C /lib/modules/$(KVERS)/build M=$(CURDIR) clean
