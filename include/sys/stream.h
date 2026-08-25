/*                               -*- Mode: C -*- 
 * <stream.h> --- Linux STREAMS common declarations. 
 * Created On      : Mon Oct 17 11:37:12 1994
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

#ifndef _STREAM_H
#define _STREAM_H
#define  STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * I heartily recommend using this feature.  It allows you to know
 * who allocated pieces of memory used by streams.  It makes it
 * particularly easy to track down lost streams buffers.
 */
#ifndef MSG_TRACE
#define	MSG_TRACE	1
#endif

/*  -------------------------------------------------------------------  */
/*                               Dependencies                            */

/* These are std defines to allow you to include only `stream.h' and run
 * miles w/o including anything else from LiS. */

/* plain usr API */
#include <sys/strport.h>	/* porting structures */
#include <sys/strconfig.h>	/* streams config symbols are here */
#ifdef __KERNEL__
#include <sys/strmsg.h>		/* streams msgs */
#endif
#include <sys/stropts.h>	/* streams operations */
#include <sys/ddi.h>		/* SVR4 compatible prototypes */
#include <sys/dki.h>		/* SVR4 compatible prototypes */

/*include everything exported to usr-level from LiS internal modules */

#include <sys/LiS/msg.h>	/* streams msgs management */
#include <sys/LiS/msgutl.h>	/* streams msg utilities  */
#include <sys/LiS/mod.h>	/* streams modules */
#ifdef __KERNEL__
#include <sys/LiS/queue.h>	/* streams queue symbols & types */
#endif
#include <sys/LiS/buffcall.h>	/* buffcalls & mem stuff */
#include <sys/LiS/events.h>	/* streams events */
#include <sys/LiS/stats.h>	/* streams statistics */
#include <sys/LiS/head.h>	/* streams statistics */
#include <sys/LiS/wait.h>	/* timers,waits,... */
#include <sys/LiS/strmdbg.h>	/* debugging stuff */

/* Bit flags
 */
#define F_ISSET(f,v)	((f) & (v))
#define F_SET(f,v)	((f) |= (v))
#define F_CLR(f,v)	((f) &= ~(v))


/*  -------------------------------------------------------------------  */
/*                               Entry points                            */

/* These are just definitions to the appropriate functions exported
 * from any of the <sys/LiS/...> stuff
 * We shouldn't have exported symbols starting w/ other than `lis' to
 * avoid kernel namespace pollution. 
 */


/* get ptr to the queue behind q. That queue with q_next == q.
 * returns NULL if no such queue
 */
#ifdef __KERNEL__
#define backq(q)	lis_backq_fcn((q),__FILE__,__LINE__)
#endif				/* __KERNEL__ */

/*  -------------------------------------------------------------------  */

/* test for flow cntl in band
 * returns 1 if msg can be put(), 0 if not.
 */
#ifdef __KERNEL__
#define bcanput		lis_bcanput
#define bcanputnext(q,b) lis_bcanput((q)->q_next,(b))
#define canput(q)       bcanput((q),0)
#define canputnext(q)   bcanputnext(q,0)
#endif				/* __KERNEL__ */

/*  -------------------------------------------------------------------  */
#ifdef __KERNEL__
#define  noenable    lis_noenable   
#define  enableok    lis_enableok   
#define  canenable   lis_canenable  
#define  putnext     lis_putnext
#define  qreply      lis_qreply 
#define	 OTHERQ      LIS_OTHERQ     
#define	 OTHER       LIS_OTHERQ     
#define	 RD          LIS_RD         
#define	 WR          LIS_WR         
#define  SAMESTR     LIS_SAMESTR    
#define	 strqset	lis_strqset
#define	 strqget	lis_strqget
extern void lis_safe_putmsg(queue_t *q, mblk_t *mp, char *f, int l)_RP;
#define put(q,m)	lis_safe_putmsg((q),(m),__FILE__,__LINE__)
#endif				/* __KERNEL__ */

/*  -------------------------------------------------------------------  */

#ifdef __KERNEL__

/*Remove a msg block (2nd arg) from the msg */
#define rmvb	lis_rmvb

/* Allocate a message */
#define allocb(s1,s2)	lis_allocb(s1,s2,"mblk " __FILE__,__LINE__)
#define esballoc(b,s,p,f) lis_esballoc(b,s,p,f,"mblk " __FILE__,__LINE__)
#define allocb_physreq(s1,s2,s3)	\
		       lis_allocb_physreq(s1,s2,s3,"physblk " __FILE__,__LINE__)

/* Check if a message can be allocated */
#define testb	lis_testb

/* Free a message block */
#define freeb	lis_freeb

/* Free a message list */
#define freemsg	lis_freemsg

/* Duplicate a message block */
#define dupb		lis_dupb

/* Duplicate a message */
#define dupmsg	lis_dupmsg

/* Copy a message block */
#define copyb	lis_copyb

/* Copy a message */
#define copymsg	lis_copymsg

/* Link message blocks */
#define linkb	lis_linkb

/* Unlink first block */
#define unlinkb	lis_unlinkb

/* Pull up the first N bytes */
#define pullupmsg	lis_pullupmsg
#define msgpullup	lis_msgpullup

/* Trim first/last(N<0) bytes */
#define adjmsg	lis_adjmsg

/* # data bytes */
#define msgdsize	lis_msgdsize

/* # bytes of first msg block type */
#define xmsgsize	lis_xmsgsize

/* # bytes in all blocks */
#define msgsize	lis_msgsize

/* Remove first msg from queue */
#define getq	lis_getq

/* Remove this msg from queue */
#define rmvq	lis_rmvq

/* Flush messages */
#define flushq	lis_flushq
#define	flushband	lis_flushband

/* Append message to queue */
#define putq	lis_putq
#define putqf	lis_putqf

/* Put message back onto queue */
#define putbq	lis_putbq
#define putbqf	lis_putbqf

/* Insert message(3) before message(2) or at end */
#define insq	lis_insq

/* Zero-byte ctl msg */
#define putctl(q, t)		lis_putctl((q),(t), __FILE__,__LINE__)
#define putnextctl(q, t)	lis_putnextctl((q),(t), __FILE__,__LINE__)

/* One-byte ctl msg */
#define putctl1(q, t, p)	lis_putctl1((q),(t),(p), __FILE__,__LINE__)
#define putnextctl1(q, t, p)	lis_putnextctl1((q),(t),(p), __FILE__,__LINE__)



/* Schedule a service procedure */
#define qenable		lis_qenable

/* Enable or disable q procs for MP version */
#define	qprocson	lis_qprocson
#define	qprocsoff	lis_qprocsoff

/* recover from allocb failure: call this when allocb() rets NULL
 * func will be called asap when a buffer for size bytes is available.
 * arg is the argument given to func
 * Returns 1 if ok and 0 if couldn't install func.
 */
#define bufcall 	 lis_bufcall
#define esbbcall	 lis_esbbcall
#define unbufcall	 lis_unbufcall

/* Check is msg is a data msg
 */
#define datamsg	    lis_datamsg
#define isdatamsg   lis_isdatamsg
#define isdatablk   lis_isdatablk

#endif				/* __KERNEL__ */

/*  -------------------------------------------------------------------  */


#ifdef __cplusplus
}
#endif

#endif /*!_STREAM_H*/
