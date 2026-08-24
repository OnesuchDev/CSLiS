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

#ifndef STREAMS_REGPARM
#ifndef __KERNEL__
#define STREAMS_REGPARM 0
#else
#error STREAMS_REGPARM is not defined - there is a bug in the build system
#endif
#endif

#if STREAMS_REGPARM != 0
#define _RP  __attribute__ ((regparm(STREAMS_REGPARM))
#else
#define _RP /* nothing */
#endif
