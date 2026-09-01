/*                               -*- Mode: C -*- 
 * <strport.h> --- Linux STREAMS operating system portability declarations.
 * Created On      : Fri Mar 24 2:40:21 1995
 * Restrictions    : SHAREd items can be read/writen by usr
 *                 : EXPORTed items can only be read by usr
 *                 : PRIVATEd items cannot be read nor writen
 *
 *    Copyright (C) 1995  Graham Wheeler, Francisco J. Ballesteros
 *    Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>
 *
 * Copyright 2022 - IBM Inc. All rights reserved
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

#ifndef _STRPORT_H
#define _STRPORT_H

/*  *******************************************************************  */
/*                               Dependencies                            */

#include <sys/LiS/linux-mdep.h>

#ifdef __KERNEL__
#ifndef NULL
#define NULL ((void*)0)
#endif

#ifndef VOID
#define VOID void
#endif
#endif

/*
 * <linux/types.h> does not have intptr_t before kernel 6.5.
 * For user level pgms, stdint.h supplies these
 */
#if defined(__KERNEL__) && !defined(_INTTYPES_H)
#define _INTTYPES_H	1		/* kernel types.h is just as good */
 					/* with the addition of intptr_t */
typedef long intptr_t;	/* What Linux uses - works on all architectures */

/*
 * Define some selected formatting phrases that we would have gotten
 * had we included the real inttypes.h.
 */
#define PRIx32	"x"
#define PRIx64	"llx"
#define PRId32	"d"
#define PRId64	"lld"
#define PRIu32	"u"
#define PRIu64	"llu"
#endif

#ifndef OPENFAIL
#define OPENFAIL	(-1)
#endif
#ifndef INFPSZ
#define INFPSZ		(-1)
#endif

#ifdef __KERNEL__
extern void	*lis_malloc(int nbytes, int class, int use_cache,
					char *file_name,int line_nr)_RP;
extern void	 lis_free(void *ptr, char *file_name,int line_nr)_RP;
#endif				/* __KERNEL__ */

#endif /* _STRPORT_H */
