/*
 * Copyright 2026 - Onesuch Dev. All rights reserved
 * SPDX-License-Identifier: LGPL-2.1
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330, Cambridge,
 * MA 02139, USA.
 */

/*
 * This header is included by glibc's <sys/poll.h>. However, glibc's version
 * does not define some STREAMS-related constants by default, whereas
 * <linux/poll.h> does. They are almost compatible with each other, with the
 * exception that <linux/poll.h> defines struct pollfd, which glibc defines
 * elsewhere. To work around this, we suppress the Linux definition by
 * temporarily renaming it. We can't easily copy glibc's <bits/poll.h> because
 * it differs by architecture.
 */

#ifdef __KERNEL__
#error Use <linux/poll.h> instead of <sys/poll.h> in the kernel
#endif

#define pollfd __ignored_kernel_pollfd
#include <linux/poll.h>
#undef pollfd
