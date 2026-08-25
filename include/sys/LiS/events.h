/*                               -*- Mode: C -*- 
 * events.h --- streams events
 * Created On      : Tue May 31 22:25:19 1994
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

#ifndef _EVENTS_H
#define _EVENTS_H 1

#ifdef __KERNEL__

/*  -------------------------------------------------------------------  */
/*				 Dependencies                            */

#include <linux/types.h>
#include <sys/strconfig.h>	/* config definitions */
/*  -------------------------------------------------------------------  */
/*				    Types                                */
/* Stream event info
 */
typedef
struct strevent {
    struct strevent *se_next;	/* next event for this stream or NULL*/
    struct strevent *se_prev;	/* previous event for this stream or last
				 * event if this is the first one*/
    pid_t se_pid;		/* process to be signaled */
    short se_evs;		/* events wanted */
} strevent_t;

/*  -------------------------------------------------------------------  */
/*				 Glob. Vars.                             */

extern struct strevent *lis_sefreelist; /* list of free stream events */
extern struct strevent *lis_secachep;   /* reserve store of free str events */

/*  -------------------------------------------------------------------  */
/*			Exported functions & macros                      */

/* get events for pid in list
 * STATUS: complete, untested
 */
extern  short
lis_get_elist_ent( strevent_t *list, pid_t pid );

/* add event to list
 * STATUS: complete, untested
 */
extern int
lis_add_to_elist( strevent_t **list, pid_t pid, short events );

/* del event from list
 * rets non-zero if not-found
 * STATUS: complete, untested
 */
extern int
lis_del_from_elist( strevent_t **list, pid_t pid, short events );

/*
 * Free the entire elist
 */
extern void
lis_free_elist( strevent_t **list);

/*  -------------------------------------------------------------------  */
#endif /* __KERNEL__ */
#endif /*!_EVENT_H*/
