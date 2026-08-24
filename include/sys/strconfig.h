/*                               -*- Mode: C -*- 
 * strconfig.h --- configuration symbols for LiS
 * Created On      : Tue May 31 21:40:37 1994
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

#ifndef _LIS_CONFIG_H
#define _LIS_CONFIG_H 1

/*  -------------------------------------------------------------------  */
/*				   Symbols                               */

/*
 * The dimension of LiS structures that exist for each CPU.
 */
#define	LIS_NR_CPUS		64

/*  -------------------------------------------------------------------  */
/* This is only for documentation purposes
 */

#define SHARE          /* you can RW */
#define EXPORT         /* you can R */
#define PRIVATE        /* you can't */

/*  -------------------------------------------------------------------  */
/*		       Global tunable vars. and symbols                  */

/* From mod.h:
 * (we've been using a char for the module id, so if the # of modules
 * get's over 255 we should define  mid_t and use this.
 */
#ifdef USE_OLD_CONSTS
#define FMNAMESZ        16	/* Max module name size */
#else
#define FMNAMESZ        8	/* compatibility with Solaris and UnixWare */
#endif
#define	LIS_NAMESZ	64	/* LiS internal name length */

#define MAX_STRDEV	1024	/*Max # of stream devices */
#define MAX_STRMOD	256	 /* Max # of stream modules */

/* From stream.c: 
 */
#define LIS_CLTIME   15000	/* # of milli-secs. to wait on close */
#define LIS_RTIME    1		/* # of secs. to wait to fwd held msg */
#define LIS_MAXPSZ   4096	/* max. packet size (must be non-zero) */
#define LIS_MINPSZ   0		/* min. packet size */

#ifdef __KERNEL__
extern int lis_strhold;		/* if not zero str hold feature's activated*/
#endif				/* __KERNEL__ */

#endif /*!_LIS_CONFIG_H*/
