/************************************************************************
*                       STREAMS Exported Symbols			*
*************************************************************************
*									*
* Author:	David Grothe	<dave@gcom.com>				*
*									*
* Copyright (C) 2002  David Grothe, Gcom, Inc <dave@gcom.com>		*
*                                                                       *
* Copyright 2022 - IBM Inc. All rights reserved                         *
* SPDX-License-Identifier: LGPL-2.1                                     *
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

#include <sys/LiS/linux-mdep.h>		/* redefine dev_t b4 any kernel incls */

#ifdef MODVERSIONS
# ifdef LISMODVERS
#  include <sys/modversions.h>      /* /usr/src/LiS/include/sys */
# endif
#endif
#define CONFIG_MODULES  1
#define EXPORT_SYMTAB   1
#define __NO_VERSION__	1	/* 2.2 kernel needs this */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,14,0)
#define _LINUX_IF_H
#define	IFNAMSIZ	16
#define __iovec_defined 1
#endif
#include <linux/module.h>

#include <sys/stream.h>
#include <sys/poll.h>
#include <sys/lislocks.h>
#include <sys/lismem.h>
#include <sys/cmn_err.h>
#include <sys/osif.h>
/************************************************************************
*                           Prototypes                                  *
*************************************************************************
*									*
* For things that aren't in the LiS header files.			*
*									*
************************************************************************/


extern int		lis_major;
extern char		lis_version[] ;
extern char		lis_date[] ;
extern char		*lis_stropts_file ;
extern int              lis_num_cpus ;
extern lis_atomic_t     lis_in_syscall ;
extern lis_atomic_t     lis_queues_running ; 
extern volatile unsigned long lis_runq_cnts[LIS_NR_CPUS] ;
extern volatile unsigned long lis_queuerun_cnts[LIS_NR_CPUS] ;
extern volatile unsigned long lis_setqsched_cnts[LIS_NR_CPUS] ;
extern volatile unsigned long lis_setqsched_isr_cnts[LIS_NR_CPUS] ;
extern lis_atomic_t	lis_strcount ;
extern long		lis_max_mem ;
extern int        	lis_seq_cntr ;




/************************************************************************
*                             Exports                                   *
*************************************************************************
*									*
* Symbols exported by LiS.  These are in alphabetical order.		*
*									*
************************************************************************/
EXPORT_SYMBOL(lis_adjmsg);
EXPORT_SYMBOL(lis_alloc_atomic_fcn);
EXPORT_SYMBOL(lis_allocb);
EXPORT_SYMBOL(lis_allocb_physreq);
EXPORT_SYMBOL(lis_alloc_dma_fcn);
EXPORT_SYMBOL(lis_alloc_kernel_fcn);
EXPORT_SYMBOL(lis_appq);
EXPORT_SYMBOL(lis_apush_get);		/* for sad */
EXPORT_SYMBOL(lis_apush_set);		/* for sad */
EXPORT_SYMBOL(lis_assert_fail);
EXPORT_SYMBOL(lis_atomic_add);
EXPORT_SYMBOL(lis_atomic_dec);
EXPORT_SYMBOL(lis_atomic_dec_and_test);
EXPORT_SYMBOL(lis_atomic_inc);
EXPORT_SYMBOL(lis_atomic_read);
EXPORT_SYMBOL(lis_atomic_set);
EXPORT_SYMBOL(lis_atomic_sub);
EXPORT_SYMBOL(lis_backq);
EXPORT_SYMBOL(lis_backq_fcn);
EXPORT_SYMBOL(lis_bcanput);
EXPORT_SYMBOL(lis_bcanputnext);
EXPORT_SYMBOL(lis_bcanputnext_anyband);
EXPORT_SYMBOL(lis_bufcall);
EXPORT_SYMBOL(lis_clone_major);
EXPORT_SYMBOL(lis_cmn_err);
EXPORT_SYMBOL(lis_copyb);
EXPORT_SYMBOL(lis_copymsg);
EXPORT_SYMBOL(lis_date);
EXPORT_SYMBOL(lis_debug_mask);
EXPORT_SYMBOL(lis_debug_mask2);
#if 0 && defined(CONFIG_DEV)
EXPORT_SYMBOL(lis_dec_mod_cnt_fcn);
#endif
EXPORT_SYMBOL(lis_disable_irq);
EXPORT_SYMBOL(lis_down_fcn);
EXPORT_SYMBOL(lis_down_nosig_fcn);
EXPORT_SYMBOL(lis_dupb);
EXPORT_SYMBOL(lis_dupmsg);
EXPORT_SYMBOL(lis_enable_irq);
EXPORT_SYMBOL(lis_esballoc);
EXPORT_SYMBOL(lis_esbbcall);
EXPORT_SYMBOL(lis_flushband);
EXPORT_SYMBOL(lis_flushq);
EXPORT_SYMBOL(lis_free);
EXPORT_SYMBOL(lis_freeb);
EXPORT_SYMBOL(lis_free_dma);
EXPORT_SYMBOL(lis_free_irq);
EXPORT_SYMBOL(lis_free_mem_fcn);
EXPORT_SYMBOL(lis_freemsg);
EXPORT_SYMBOL(lis_free_pages_fcn);
EXPORT_SYMBOL(lis_freezestr);
EXPORT_SYMBOL(lis_get_free_pages_atomic_fcn);
EXPORT_SYMBOL(lis_get_free_pages_fcn);
EXPORT_SYMBOL(lis_get_free_pages_kernel_fcn);
EXPORT_SYMBOL(lis_getq);
EXPORT_SYMBOL(lis_gettimeofday);
#if 0 && defined(CONFIG_DEV)
EXPORT_SYMBOL(lis_inc_mod_cnt_fcn);
#endif
EXPORT_SYMBOL(lis_insq);
EXPORT_SYMBOL(lis_in_interrupt);

EXPORT_SYMBOL(lis_ioremap);
EXPORT_SYMBOL(lis_ioremap_nocache);
EXPORT_SYMBOL(lis_iounmap);
EXPORT_SYMBOL(lis_irqreturn_handled);
EXPORT_SYMBOL(lis_irqreturn_not_handled);
EXPORT_SYMBOL(lis_jiffies);
EXPORT_SYMBOL(lis_kernel_down);
EXPORT_SYMBOL(lis_kernel_up);
EXPORT_SYMBOL(lis_kfree);
EXPORT_SYMBOL(lis_kmalloc);
EXPORT_SYMBOL(lis_linkb);
EXPORT_SYMBOL(lis_getmajor);
EXPORT_SYMBOL(lis_getminor);
EXPORT_SYMBOL(lis_makedevice);
EXPORT_SYMBOL(lis_major);
EXPORT_SYMBOL(lis_malloc);
EXPORT_SYMBOL(lis_mark_mem_fcn);
EXPORT_SYMBOL(lis_milli_to_ticks);
EXPORT_SYMBOL(lis_mknod);
#if 0 && defined(CONFIG_DEV)
EXPORT_SYMBOL(lis_mod_cnt_sync_fcn);
#endif
EXPORT_SYMBOL(lis_msgdsize);
EXPORT_SYMBOL(lis_msgpullup);
EXPORT_SYMBOL(lis_msgsize);
EXPORT_SYMBOL(lis_msg_type_name);
EXPORT_SYMBOL(lis_num_cpus);
EXPORT_SYMBOL(lis_osif_cli);
EXPORT_SYMBOL(lis_osif_do_gettimeofday);
EXPORT_SYMBOL(lis_osif_do_settimeofday);

EXPORT_SYMBOL(lis_osif_sti);
EXPORT_SYMBOL(lis_own_spl);
EXPORT_SYMBOL(lis_phys_to_virt);
EXPORT_SYMBOL(lis_print_block);
EXPORT_SYMBOL(lis_print_data);
EXPORT_SYMBOL(lis_printk);
EXPORT_SYMBOL(lis_print_mem);
EXPORT_SYMBOL(lis_print_msg);
EXPORT_SYMBOL(lis_print_queue);
EXPORT_SYMBOL(lis_pullupmsg);
EXPORT_SYMBOL(lis_putbq);
EXPORT_SYMBOL(lis_putbqf);
EXPORT_SYMBOL(lis_putctl);
EXPORT_SYMBOL(lis_putctl1);
EXPORT_SYMBOL(lis_putnextctl);
EXPORT_SYMBOL(lis_putnextctl1);
EXPORT_SYMBOL(lis_putq);
EXPORT_SYMBOL(lis_putqf);
EXPORT_SYMBOL(lis_qcountstrm);
EXPORT_SYMBOL(lis_qenable);
EXPORT_SYMBOL(lis_qprocsoff);
EXPORT_SYMBOL(lis_qprocson);
EXPORT_SYMBOL(lis_qsize);
EXPORT_SYMBOL(lis_queue_name);
EXPORT_SYMBOL(lis_register_strdev);
EXPORT_SYMBOL(lis_register_strmod);
EXPORT_SYMBOL(lis_register_driver_qlock_option);
EXPORT_SYMBOL(lis_register_module_qlock_option);
EXPORT_SYMBOL(lis_release_region);
EXPORT_SYMBOL(lis_request_dma);
EXPORT_SYMBOL(lis_request_irq);
EXPORT_SYMBOL(lis_request_region);
EXPORT_SYMBOL(lis_rmvb);
EXPORT_SYMBOL(lis_rmvq);
EXPORT_SYMBOL(lis_rw_lock_alloc_fcn);
EXPORT_SYMBOL(lis_rw_lock_free_fcn);
EXPORT_SYMBOL(lis_rw_lock_init_fcn);
EXPORT_SYMBOL(lis_rw_read_lock_fcn);
EXPORT_SYMBOL(lis_rw_read_lock_irq_fcn);
EXPORT_SYMBOL(lis_rw_read_lock_irqsave_fcn);
EXPORT_SYMBOL(lis_rw_read_unlock_fcn);
EXPORT_SYMBOL(lis_rw_read_unlock_irq_fcn);
EXPORT_SYMBOL(lis_rw_read_unlock_irqrestore_fcn);
EXPORT_SYMBOL(lis_rw_write_lock_fcn);
EXPORT_SYMBOL(lis_rw_write_lock_irq_fcn);
EXPORT_SYMBOL(lis_rw_write_lock_irqsave_fcn);
EXPORT_SYMBOL(lis_rw_write_unlock_fcn);
EXPORT_SYMBOL(lis_rw_write_unlock_irq_fcn);
EXPORT_SYMBOL(lis_rw_write_unlock_irqrestore_fcn);
EXPORT_SYMBOL(lis_safe_canenable);
EXPORT_SYMBOL(lis_safe_enableok);
EXPORT_SYMBOL(lis_safe_noenable);
EXPORT_SYMBOL(lis_safe_OTHERQ);
EXPORT_SYMBOL(lis_safe_putmsg);
EXPORT_SYMBOL(lis_safe_putnext);
EXPORT_SYMBOL(lis_safe_qreply);
EXPORT_SYMBOL(lis_safe_RD);
EXPORT_SYMBOL(lis_safe_SAMESTR);
EXPORT_SYMBOL(lis_safe_WR);
EXPORT_SYMBOL(lis_secs);
EXPORT_SYMBOL(lis_sem_alloc);
EXPORT_SYMBOL(lis_sem_destroy);
EXPORT_SYMBOL(lis_sem_init);
EXPORT_SYMBOL(lis_wait_event);
EXPORT_SYMBOL(lis_wait_event_interruptible);
EXPORT_SYMBOL(lis_spin_is_locked_fcn);
EXPORT_SYMBOL(lis_spin_lock_alloc_fcn);
EXPORT_SYMBOL(lis_spin_lock_fcn);
EXPORT_SYMBOL(lis_spin_lock_free_fcn);
EXPORT_SYMBOL(lis_spin_lock_init_fcn);
EXPORT_SYMBOL(lis_spin_lock_irq_fcn);
EXPORT_SYMBOL(lis_spin_lock_irqsave_fcn);
EXPORT_SYMBOL(lis_spin_trylock_fcn);
EXPORT_SYMBOL(lis_spin_unlock_fcn);
EXPORT_SYMBOL(lis_spin_unlock_irq_fcn);
EXPORT_SYMBOL(lis_spin_unlock_irqrestore_fcn);
EXPORT_SYMBOL(lis_spl0_fcn);
EXPORT_SYMBOL(lis_splstr_fcn);
EXPORT_SYMBOL(lis_splx_fcn);
EXPORT_SYMBOL(lis_sprintf);
EXPORT_SYMBOL(lis_strm_name);
EXPORT_SYMBOL(lis_strm_name_from_queue);
EXPORT_SYMBOL(lis_stropts_file);
EXPORT_SYMBOL(lis_strqget);
EXPORT_SYMBOL(lis_strqset);
EXPORT_SYMBOL(lis_testb);
EXPORT_SYMBOL(lis_thread_start);
EXPORT_SYMBOL(lis_thread_stop);
EXPORT_SYMBOL(lis_timeout_fcn);
EXPORT_SYMBOL(lis_udelay);
EXPORT_SYMBOL(lis_unbufcall);
EXPORT_SYMBOL(lis_unfreezestr);
EXPORT_SYMBOL(lis_unlink);
EXPORT_SYMBOL(lis_unlinkb);
EXPORT_SYMBOL(lis_unregister_strdev);
EXPORT_SYMBOL(lis_unregister_strmod);
EXPORT_SYMBOL(lis_untimeout);
EXPORT_SYMBOL(lis_up_fcn);
EXPORT_SYMBOL(lis_version);
EXPORT_SYMBOL(lis_vfree);
EXPORT_SYMBOL(lis_virt_to_phys);
EXPORT_SYMBOL(lis_vmalloc);
EXPORT_SYMBOL(lis_vremap);
EXPORT_SYMBOL(lis_vsprintf);
EXPORT_SYMBOL(lis_wake_up);
EXPORT_SYMBOL(lis_wake_up_interruptible);
EXPORT_SYMBOL(lis_xmsgsize);
EXPORT_SYMBOL(lis_zmalloc);

#if (!defined(_S390_LIS_) && !defined(_S390X_LIS_) && !defined(_PPC64_LIS_) && !defined(_X86_64_LIS_))



/*
 * Wrapper functions
 */
EXPORT_SYMBOL(__wrap_strcpy);
EXPORT_SYMBOL(__wrap_strncpy);
EXPORT_SYMBOL(__wrap_strcat);
EXPORT_SYMBOL(__wrap_strncat);
EXPORT_SYMBOL(__wrap_strcmp);
EXPORT_SYMBOL(__wrap_strncmp);
EXPORT_SYMBOL(__wrap_strnicmp);
EXPORT_SYMBOL(__wrap_strchr);
EXPORT_SYMBOL(__wrap_strrchr);
EXPORT_SYMBOL(__wrap_strstr);
EXPORT_SYMBOL(__wrap_strlen);
EXPORT_SYMBOL(__wrap_memset);
EXPORT_SYMBOL(__wrap_memcpy);
EXPORT_SYMBOL(__wrap_memcmp);


#endif          /* S390 or S390X or PPC64 or X86_64 */
