/************************************************************************
*                      poll() System Call				*
*************************************************************************
*									*
* This file does the hard work for the poll() system call.  It handles	*
* STREAMS files using the AT&T poll mechanism and non-streams files	*
* by using their select functions.					*
*									*
* Author:	David Grothe <dave@gcom.com>				*
*									*
* Copyright (C) 1997  David Grothe, Gcom, Inc <dave@gcom.com>		*
*                                                                       *
* Copyright 2022 - IBM Inc. All rights reserved                         *
* SPDX-License-Identifier: LGPL-2.1                                     *
*                                                                       *
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
#ident "@(#) CSLiS poll.c 7.111 2024-05-07 15:30:00 "

#include <sys/stream.h>
#define	USE_LINUX_POLL_H	1
#include <linux/poll.h>
#include <sys/lislocks.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,14,0)
#define _LINUX_IF_H
#define	IFNAMSIZ	16
#define __iovec_defined 1
#endif
#include <sys/osif.h>

char	*lis_poll_file =
#if defined(USED_LINUX_POLL_H)
		    "<linux/poll.h>"
#elif defined(USED_LIS_POLL_H)
		    "<LiS/include/sys/poll.h>"
#else
		    "<unknown/poll.h>"
#endif
;

/* typedef struct semaphore	semaphore_t ; */
typedef struct file		file_t ;

long		lis_poll_sleeping ;	/* # times slept > twice on same poll */

#ifdef POLL_TRACE

typedef struct
{
    int		time_out ;
    long	target_time ;
    long	jiffies ;
    long	time_interval ;
    long	ticks ;

} poll_trace_t ;

poll_trace_t		lis_poll_trace[5] ;

#endif

/*
 * Routines elsewhere in STREAMS
 */
extern char	*lis_poll_events(short events) _RP ;
extern unsigned	 lis_poll_bits(stdata_t *hd) ;

/************************************************************************
*                           lis_poll_2_1                                *
*************************************************************************
*									*
* This is the kernel version 2.1 poll routine.  It is pointed to by	*
* the fops structure for STREAMS.  It handles polling for one stream.	*
* It builds a list of processes waiting on the stream in the task list	*
* owned in the stdata structure.					*
*									*
* If we have to wait then lis_wake_up_poll in head.c does the wakeup.	*
*									*
************************************************************************/
unsigned lis_poll_2_1(struct file *fp, poll_table *wait)
{
    stdata_t	*head ;

    if (fp == NULL)
	return(POLLNVAL) ;

    head = FILE_STR(fp) ;
    if (head->magic != STDATA_MAGIC)
    {
	printk("lis_poll_2_1: fp=%p wait=%p head=%p magic=%lx should be %lx\n",
		fp, wait, head, head->magic, STDATA_MAGIC) ;
	return(POLLNVAL) ;
    }

    /*
     * Put us into the wait queue first.  If any files have returned
     * non-zero then Linux stops the wait queue insertion.  Linux poll
     * system call cleans up the wait queues after all the poll operations
     * are done.
     */
    poll_wait(fp, &head->sd_task_list, wait) ;
    return(lis_poll_bits(head)) ;

} /* lis_poll_2_1 */
