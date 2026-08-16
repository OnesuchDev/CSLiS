/*                               -*- Mode: C -*- 
 * <strpoll> --- poll
 * Author          : gram & nemo
 * Created On      : Mon Oct 17 11:37:12 1994
 * RCS Id          ; $Id: poll.h,v 1.1.1.1 2005/04/12 20:27:06 ragnar Exp $
 * Last Modified By: David Grothe
 * Last Modified On: 
 * Update Count    : 6
 * Status          : Debugged
 * Prefix(es)      : 
 * Requirements    :
 * Purpose         :
 *                 :
 *    Copyright (C) 1995  Graham Wheeler, Francisco J. Ballesteros
 *    Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>
 *
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
 *
 *    You can reach us by email to any of
 *    gram@aztec.co.za, nemo@ordago.uc3m.es
 */

#ifndef _SYS_POLL_H
#define	_SYS_POLL_H

/*  -------------------------------------------------------------------  */


#if defined(__linux__)
/*
 * The strategy here is that if we are compiling for a 2.1 or 2.2 kernel
 * we want to use the kernel's poll.h.  Otherwise we will use the poll.h
 * that comes with LiS.  "Otherwise" also includes the use of LiS as a
 * portable STREAMS with no kernel poll.h to be included.
 */
# if !defined(_LIS_M_DEP_H) || !defined(__KERNEL__)
#  include <linux/version.h>
#  ifndef KERNEL_VERSION
#  define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#  endif
# endif				/* _LIS_M_DEP_H */
#endif				/* __linux__ */

/*    For Kernel 3.10.0 and later, only use linux poll includes of sys/types.h not already defined. */
# if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
// #  ifndef _SYS_TYPES_H
#   ifndef CONFIG_HZ              /*  for 3.10 kernel, CONFIG_HZ not in regular autoconf.h */
#    include <generated/autoconf.h>
#   endif
#   if defined(__KERNEL__)   /*  For 3.10 kernel, if driver, get poll.h from kernel headers, */
#     include <linux/poll.h>
#   else                     /*   else get the uapi version  */
#     if (LINUX_VERSION_CODE > KERNEL_VERSION(4, 12, 0))
#        if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 11, 0))
#           include <linux/compiler.h>
#           include <uapi/linux/types.h>
#        endif
#     endif
#     include <uapi/linux/poll.h>
#  endif
# else
#  include <linux/poll.h>		/* instead of the rest of this file */
#  if defined(_SPARC_LIS_) && !defined(POLLMSG)
#    define POLLMSG         0x0200  /* supply missing def for SPARC */
#  endif
#endif

/*
 * The prototype for the routine called from user programs for the
 * poll system call.
 */
#ifndef poll		/* no macro by that name */

#ifdef __cplusplus
extern "C" {
#endif

extern int poll(struct pollfd *fds, unsigned long nfds, int timeout);

#ifdef __cplusplus
}
#endif

#endif

/*  -------------------------------------------------------------------  */


#endif	/* _SYS_POLL_H */
