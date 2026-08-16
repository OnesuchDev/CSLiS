# 
# Copyright 2022 - IBM Inc. All rights reserved
# SPDX-License-Identifier: LGPL-2.1
# 
include .config_mk

#
# Environment of LINUX means the intent is to link the code into the
# Linux kernel.
#
OPT = -DLINUX
ifeq ($(ARCH),x86_64)
OPT += -mcmodel=kernel
endif
ifeq ($(LINUX_TYPE),DEBIAN)
OPT += -fno-pie -fno-stack-protector
endif

# Are we compiling to a Linux kernel with module versions?
ifdef CONFIG_MODULES
  ifdef CONFIG_MODVERSIONS
    OPT += -DMODVERSIONS
  endif 
endif 

# Are we compiling LiS as a loadable kernel module?
ifeq ($(CONFIG_STREAMS),m)
  OPT += -DMODULE
  ifeq ($(QUOTEDNAMESPACE),y)
     OPT += -DDEFAULT_SYMBOL_NAMESPACE=\"STREAMS\"
  else 
     OPT += -DDEFAULT_SYMBOL_NAMESPACE=STREAMS
  endif
  ifeq ($(KBUILD_NEEDS_MODNAME),y)
    OPT += -DKBUILD_MODNAME=\"streams\"
  endif
endif 

#ifeq ($(DBG_OPT),y)
#  OPT += -DDEBUG
#endif

#
# include LiS's memory allocator interface.
#
XOBJS = lismem.o

#
# include LiS's locking code
#
XOBJS += lislocks.o

#
# include the LiS exported symbol list
#
XOBJS += exports.o

include $(HEADDIR)/mk.bdy

modules: streamshead.o

dep: common-dep
	$(Q_ECHO) $(qtag_DEP)$(relpwd)
