# obj-m = lab1_module_param.o

# obj-m += lab2_export.o
# obj-m += lab2_import.o
# obj-m += lab3_1_chardev.o
# obj-m += lab3_2_signal.o
# obj-m += lab4_1_procfs.o
# obj-m += lab4_2_sysfs.o
# obj-m += lab5_waitqueue.o
# obj-m += lab5_workqueue.o
# obj-m += lab5_workqueue2.o
# obj-m += lab6_tasklet.o
# obj-m += spi-bcm2835.o
# obj-m += lab7_intrrupt.o
# obj-m += lab8_list.o
# obj-m += lab9_kthread.o
# obj-m += lab10_mutex.o
obj-m +=lab11_timer.o

# EXTRA_CFLAGS = -Wall -g -O0
build:
	make -C /home/niyu/Desktop/raspberry/buildroot/output/build/linux-custom M=`pwd` ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- modules
	aarch64-linux-gnu-gcc t_dev.c -o t_dev
	aarch64-linux-gnu-gcc t_signal.c -o t_signal
clean:
	make -C /home/niyu/Desktop/raspberry/buildroot/output/build/linux-custom M=`pwd` ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- clean
	rm t_dev t_signal
# userage: 1.make 2. make clean 

#补充 make V=1 获取 具体编译命令:
#aarch64-linux-gnu-gcc -Wp,-MD,/home/niyu/Desktop/raspberry/buildroot/study/.test.o.d  \
-nostdinc \
-isystem /usr/lib/gcc-cross/aarch64-linux-gnu/9/include \
-I./arch/arm64/include \
-I./arch/arm64/include/generated \
-I./include \
-I./arch/arm64/include/uapi \
-I./arch/arm64/include/generated/uapi \
-I./include/uapi \
-I./include/generated/uapi \
-include ./include/linux/kconfig.h \
-include ./include/linux/compiler_types.h \
-D__KERNEL__ -mlittle-endian \
-DKASAN_SHADOW_SCALE_SHIFT=3 \
-Wall -Wundef -Werror=strict-prototypes \
-Wno-trigraphs -fno-strict-aliasing -fno-common -fshort-wchar -fno-PIE -Werror=implicit-function-declaration \
-Werror=implicit-int -Wno-format-security -std=gnu89 -mgeneral-regs-only -DCONFIG_AS_LSE=1 -DCONFIG_CC_HAS_K_CONSTRAINT=1 \
-fno-asynchronous-unwind-tables -Wno-psabi -mabi=lp64 -mbranch-protection=none -DKASAN_SHADOW_SCALE_SHIFT=3 -fno-delete-null-pointer-checks \
-Wno-frame-address -Wno-format-truncation -Wno-format-overflow -Wno-address-of-packed-member -O2 --param=allow-store-data-races=0 \
-Wframe-larger-than=2048 -fstack-protector-strong -Wno-unused-but-set-variable -Wimplicit-fallthrough -Wno-unused-const-variable \
-fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-var-tracking-assignments -pg -Wdeclaration-after-statement \
-Wvla -Wno-pointer-sign -Wno-stringop-truncation -Wno-array-bounds -Wno-stringop-overflow -Wno-restrict -Wno-maybe-uninitialized \
-fno-strict-overflow -fno-merge-all-constants -fmerge-constants -fno-stack-check -fconserve-stack -Werror=date-time \
-Werror=incompatible-pointer-types -Werror=designated-init -fmacro-prefix-map=./= -Wno-packed-not-aligned -mstack-protector-guard=sysreg \
-mstack-protector-guard-reg=sp_el0 -mstack-protector-guard-offset=1336  -DMODULE  -DKBUILD_BASENAME='"test"' -DKBUILD_MODNAME='"test"' \
-E -o /home/niyu/Desktop/raspberry/buildroot/study/test.e /home/niyu/Desktop/raspberry/buildroot/study/test.c_3