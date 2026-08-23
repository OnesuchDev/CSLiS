#ifndef _USRIO_H_
#define _USRIO_H_
/************************************************************************
*                                                                       *
* Copyright 2022 - IBM Inc. All rights reserved                         *
* SPDX-License-Identifier: LGPL-2.1                                     *
*                                                                       *
*                        Linux I/O Functions				*
*************************************************************************
*									*
* This file is included by strtst.c if -DLINUX is set.  The strtst	*
* program is built for use in a user-level debugging context in which	*
* the test program (strtst.o) is linked in with the streams package	*
* itself plus a "user" I/O library.  When -DLINUX is set then the	*
* test program is a user level program running against the streams	*
* package in the kernel.  We have to change some names and the like	*
* to make this all work out.						*
*									*
************************************************************************/
/*
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
 * 
 */

#ifndef _SYS_POLL_H
#include <sys/poll.h>
#endif

#ifndef NULL
#define	NULL		( (void *) 0 )
#endif

#ifndef _IOW
#define _IOW(x,y,t)     (((((int)sizeof(t))&0xFFFF)<<16)|(x<<8)|y)
#endif

/************************************************************************
*                         fcntl Types                                   *
*************************************************************************
*									*
* The following codes will be defined here for calls to fcntl if they	*
* have not been defined elsewhere first.  These are SVR4 command codes	*
* that are not included in every environment's fcntl.h.			*
*									*
************************************************************************/

#ifndef F_ALLOCSP
#define F_ALLOCSP	10
#endif
#ifndef F_FREESP
#define F_FREESP	11
#endif
#ifndef F_RSETLK
#define F_RSETLK	20
#endif
#ifndef F_RGETLK
#define F_RGETLK	21
#endif
#ifndef F_RSETLKW
#define F_RSETLKW	22
#endif
#ifndef F_GETOWN
#define F_GETOWN	23
#endif
#ifndef F_SETOWN
#define F_SETOWN	24
#endif
#ifndef F_CHKFL
#define F_CHKFL		99			/* completely bogus value */
#endif




#endif /* _USRIO_H_ */
