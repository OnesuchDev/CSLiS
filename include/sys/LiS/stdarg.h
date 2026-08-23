/*************************************************************************
 *                                                                       *
 * Copyright 2026 - OnesuchDev. All rights reserved                      *
 * SPDX-License-Identifier: LGPL-2.1                                     *
 *                                                                       *
 ************************************************************************/

#include <linux/version.h>

#ifndef RHEL_RELEASE_VERSION
#define RHEL_RELEASE_VERSION(a, b) (((a) << 8) + (b))
#endif

/* Even though RHEL 9 uses kernel 5.14, it has <linux/stdarg.h> instead of <stdarg.h> */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 15, 0) || (defined(RHEL_RELEASE_CODE) && RHEL_RELEASE_CODE >= RHEL_RELEASE_VERSION(9, 1))
#include <linux/stdarg.h>
#else
#include <stdarg.h>
#endif
