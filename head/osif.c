/************************************************************************
*                   Operating System Interface                          *
*************************************************************************
*									*
* The routines in this file provide a filtered interface to certain	*
* operating system routines that drivers are most likely to use.	*
*									*
* This code loads as a loadable module, but it has no open routine.	*
*									*
* Additions to this file are welcome.					*
*									*
*	Copyright (c) 1999 David Grothe <dave@gcom.com>			*
*                                                                       *
* Copyright 2022 - IBM Inc. All rights reserved                         *
* SPDX-License-Identifier: LGPL-2.1                                     *
*									*
* This library is free software; you can redistribute it and/or		*
* modify it under the terms of the GNU Library General Public		*
* License as published by the Free Software Foundation; either		*
* version 2 of the License, or (at your option) any later version.	*
* 									*
* This library is distributed in the hope that it will be useful,	*
* but WITHOUT ANY WARRANTY; without even the implied warranty of	*
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU	*
* Library General Public License for more details.			*
*								 	*
* You should have received a copy of the GNU Library General Public	*
* License along with this library; if not, write to the	Free Software	*
* Foundation, Inc., 59 Temple Place - Suite 330, Cambridge, MA 02139 USA*
*									*
************************************************************************/

#include <sys/stream.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif		/* Linux config defines */
#ifndef RHEL_RELEASE_VERSION
#define RHEL_RELEASE_VERSION(a, b) (((a) << 8) + (b))
#endif

#ifdef STR
#undef STR				/* collides with irq.h */
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,14,0)
#define _LINUX_IF_H
#define IFNAMSIZ        16
#define __iovec_defined 1
#endif

#if (defined(_S390X_LIS_) || defined(_PPC64_LIS_) )
#if ((defined(RHEL_RELEASE_CODE) && RHEL_RELEASE_CODE > RHEL_RELEASE_VERSION(9, 5)) || \
     (LINUX_VERSION_CODE > KERNEL_VERSION(6,10,0))) /* RHEL 9.6 or RHEL 10, SLES 16 */
#define _LINUX_PROPERTY_H_  // omit property.h
#endif
#endif

#include <linux/sched.h>
#include <linux/interrupt.h>            /* request_irq, etc. */
#include <linux/ioport.h>		/* request_region */
#include <asm/dma.h>
#include <linux/slab.h>
#include <linux/timer.h>                /* add_timer, del_timer */
#include <linux/ptrace.h>		/* for pt_regs */
#include <linux/vmalloc.h>
#include <asm/io.h>			/* ioremap, virt_to_phys */
#include <asm/irq.h>			/* disable_irq, enable_irq */
#include <asm/irq_regs.h>		/* get_irq_regs */
#include <asm/atomic.h>			/* the real kernel routines */

#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
#include <asm/scatterlist.h>
#else
#include <linux/scatterlist.h>
#endif

#include <linux/delay.h>
#include <linux/time.h>
#if (defined(RHEL_RELEASE_CODE) && RHEL_RELEASE_CODE < RHEL_RELEASE_VERSION(9, 1))  //For RHEL 9 update 12-2022
#include <stdarg.h>                    /* for va_list */
#endif

#define	INCL_FROM_OSIF_DRIVER		/* do not change routine names */
#include <sys/osif.h>

/* Prototypes */
void lis_free_devid_list(void);

/************************************************************************
*                        IRQ Routines                                   *
************************************************************************/

/*
 * Because of the register parameter passing in the 2.6 kernel we need
 * an irq routine that is compiled with the kernel's register style to
 * pass to the kernel's request_irq function.  This routine then calls
 * the LiS style irq handler using the LiS parameter passing convention.
 *
 * So we construct a "device id" as the pointer to one of these structures
 * in which we save the user's original dev_id parameter which we then
 * pass back to the user's interrupt handler.  We are going to appropriate
 * the lis_incr_lock spin lock for our list protection.
 *
 * We make a list of these structures that can be cleaned up at LiS
 * termination time.
 */
typedef struct lis_devid
{
    struct lis_devid	*link ;
    lis_int_handler	 handler ;
    void		*dev_id ;		/* user's dev_id param */
    int			 irq ;
} lis_devid_t ;

extern lis_spin_lock_t  lis_incr_lock ;

lis_devid_t	*lis_devid_list ;

#if !defined(__s390__)
/*
 * This routine is compiled with the kernel's parameter passing convention.
 * It calls the STREAMS driver's handler using LiS parameter passing
 * convention.
 */
static irqreturn_t lis_khandler(int irq, void *dev_id)
{
    lis_devid_t		*dv = (lis_devid_t *) dev_id ;
    struct pt_regs *regs = get_irq_regs();

    return(dv->handler(irq, dv->dev_id, regs)) ;
}
#endif
/*
 * Called at LiS termination time to clean up the list
 */
void lis_free_devid_list(void)
{
    lis_devid_t		*dv = lis_devid_list ;
    lis_devid_t		*nxt ;

    lis_devid_list = NULL ;

    while (dv != NULL)
    {
	nxt = dv->link ;
	if (dv->handler && dv->dev_id && dv->irq)
	{
	    printk("LiS freeing IRQ%u\n", dv->irq) ;
#if !defined(__s390__)
	    free_irq(dv->irq, dv) ;
#endif
	}
	FREE(dv) ;
	dv = nxt ;
    }
}

int      lis_irqreturn_handled = IRQ_HANDLED;
int      lis_irqreturn_not_handled = IRQ_NONE;

int  _RP lis_request_irq(unsigned int  irq,
		     lis_int_handler handler,
	             unsigned long flags,
		     const char   *device,
		     void         *dev_id)
{
#if !defined(__s390__)
    int			  ret ;
    lis_devid_t		 *dv ;
    lis_flags_t		  psw ;

    dv = ALLOC(sizeof(*dv)) ;
    if (dv == NULL)
	return(-ENOMEM) ;

    dv->handler = handler ;
    dv->dev_id  = dev_id ;
    dv->irq     = irq ;

    ret = request_irq(irq, lis_khandler, flags, device, dv) ;
    if (ret == 0)
    {
	lis_spin_lock_irqsave(&lis_incr_lock, &psw) ;
	dv->link = lis_devid_list ;
	lis_devid_list = dv ;
	lis_spin_unlock_irqrestore(&lis_incr_lock, &psw) ;
    }
    else
	FREE(dv) ;

    return(ret) ;
#else
return(0); 
#endif

}

void  _RP lis_free_irq(unsigned int irq, void *dev_id)
{
#if !defined(__s390__)
    lis_devid_t		*dv = lis_devid_list ;
    lis_flags_t		 psw ;

    lis_spin_lock_irqsave(&lis_incr_lock, &psw) ;
    for (; dv != NULL; dv = dv->link)
    {
	if (dv->dev_id == dev_id)
	{
	    free_irq(irq, dv) ;
	    dv->handler = NULL ;
	    dv->dev_id  = NULL ;
	    dv->irq     = 0 ;
	    lis_spin_unlock_irqrestore(&lis_incr_lock, &psw) ;
	    FREE(dv);
	    return ;
	}
    }
    lis_spin_unlock_irqrestore(&lis_incr_lock, &psw) ;
#endif
}

void  _RP lis_enable_irq(unsigned int irq)
{
#if !defined(__s390__)
    enable_irq(irq) ;
#endif
}

void  _RP lis_disable_irq(unsigned int irq)
{
#if !defined(__s390__)
    disable_irq(irq) ;
#endif
}

void  _RP lis_osif_cli( void )
{
    lis_splstr() ;

}
void  _RP lis_osif_sti( void )
{
    lis_spl0() ;
}



/************************************************************************
*                       Memory Mapping Routines                         *
*************************************************************************
*									*
* These are a subset of what is available.  Feel free to add more.	*
*									*
* The 2.0.x routines will call 2.2.x routines if we are being compiled	*
* for 2.2.x, and vice versa.  This gives drivers something in the way	*
* of version independence.  Wish the kernel guys would tend to that	*
* a bit more.								*
*									*
************************************************************************/

void * _RP lis_ioremap(unsigned long offset, unsigned long size)
{
#if   !defined(__s390__)
    return(ioremap(offset, size)) ;
#else
    return(NULL) ;
#endif
}

void * _RP lis_ioremap_nocache(unsigned long offset, unsigned long size)
{
#if   !defined(__s390__)
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,6,0)    
    return(ioremap_nocache(offset, size)) ;
#else
    return(ioremap_cache(offset, size)) ; // default behavior moved to nocache for this
#endif    
#else
    return(NULL) ;
#endif
}

void _RP lis_iounmap(void *ptr)
{
#if   !defined(__s390__)
    iounmap(ptr) ;
#endif
}

void * _RP lis_vremap(unsigned long offset, unsigned long size)
{
#if   !defined(__s390__)
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,6,0)    
    return(ioremap_nocache(offset, size)) ;
#else
    return(ioremap_cache(offset, size)) ; // default behavior moved to nocache for this
#endif
#else
    return(NULL) ;
#endif
}

unsigned long  _RP lis_virt_to_phys(volatile void *addr)
{
    return(virt_to_phys(addr)) ;
}

void * _RP lis_phys_to_virt(unsigned long addr)
{
    return(phys_to_virt(addr)) ;
}


/************************************************************************
*                       I/O Ports Routines                              *
************************************************************************/

int  _RP lis_check_region(unsigned int from, unsigned int extent)
{
    if (request_region(from,extent,"LiS-checking"))
    {
	release_region(from,extent) ;
	return(0) ;
    }
    return(-EBUSY) ;
}

void  _RP lis_request_region(unsigned int from,
			 unsigned int extent,
			 const char  *name)
{
    request_region(from, extent, name) ;
}

void  _RP lis_release_region(unsigned int from, unsigned int extent)
{
    release_region(from, extent) ;
}

/************************************************************************
*                    Memory Allocation Routines                         *
************************************************************************/

void * _RP lis_kmalloc(size_t nbytes, int type)
{
    return(kmalloc(nbytes, type)) ;
}

void   _RP lis_kfree(const void *ptr)
{
    kfree((void *)ptr) ;
}

void * _RP lis_vmalloc(unsigned long size)
{
    return(vmalloc(size)) ;
}

void   _RP lis_vfree(void *ptr)
{
    vfree(ptr) ;
}

/************************************************************************
*                        DMA Routines                                   *
************************************************************************/

int   _RP lis_request_dma(unsigned int dma_nr, const char *device_id)
{
#if defined(MAX_DMA_CHANNELS)
    return(request_dma(dma_nr, device_id)) ;
#else
    return(-ENXIO) ;
#endif
}

void  _RP lis_free_dma(unsigned int dma_nr)
{
#if defined(MAX_DMA_CHANNELS)
    free_dma(dma_nr) ;
#endif
}

/************************************************************************
*                         Delay Routines                                *
************************************************************************/

void  _RP lis_udelay(long micro_secs)
{
    udelay(micro_secs) ;
}

unsigned long  _RP lis_jiffies(void)
{
    return(jiffies) ;
}

/************************************************************************
*                         Time Routines                                 *
************************************************************************/
void  _RP lis_osif_do_gettimeofday( struct timeval *tp )
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)	
    do_gettimeofday(tp) ;
#else    
    struct timespec64 now;

    ktime_get_real_ts64
	    (&now);
    tp->tv_sec = now.tv_sec;
    tp->tv_usec = now.tv_nsec/1000;
#endif
}

void  _RP lis_osif_do_settimeofday( struct timeval *tp )
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,11,0) // No timespec in 5.11 or higher
    	struct timespec ts ;

    ts.tv_sec = tp->tv_sec ;
    ts.tv_nsec = tp->tv_usec * 1000 ;
#endif    
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,0,0)
    do_settimeofday(&ts) ;
    
#else   // for deprecated ddo_settimeofday64()
     struct timespec64 ts64;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5,11,0)
     ts64 = timespec_to_timespec64(ts);
#else  // the kernel is now fullu 64-bit timespec
    ts64.tv_sec = tp->tv_sec ;
    ts64.tv_nsec = tp->tv_usec * 1000 ;
#endif     
     do_settimeofday64(&ts64);
#endif	

}


/************************************************************************
*                         Printing Routines                             *
************************************************************************/
int  _RP lis_printk(const char *fmt, ...)
{
    extern char	    lis_cmn_err_buf[];
    va_list	    args;
    int		    ret ;

    va_start (args, fmt);
    ret = vsprintf (lis_cmn_err_buf, fmt, args);
    va_end (args);

    printk("%s", lis_cmn_err_buf) ;
    return(ret) ;
}

int  _RP lis_sprintf(char *bfr, const char *fmt, ...)
{
    va_list	    args;
    int		    ret ;

    va_start (args, fmt);
    ret = vsprintf (bfr, fmt, args);
    va_end (args);

    return(ret) ;
}

int  _RP lis_vsprintf(char *bfr, const char *fmt, va_list args)
{
    return(vsprintf (bfr, fmt, args));
}

void  _RP lis_wait_event(OSIF_WAIT_E_ARG, int condition)
{
    wait_event(wq, condition) ;
}

void  _RP lis_wait_event_interruptible(OSIF_WAIT_E_ARG, int condition)
{
    wait_event_interruptible(wq, condition) ;
}

void  _RP lis_wake_up(OSIF_WAIT_Q_ARG)
{
    wake_up(wq) ;
}

void  _RP lis_wake_up_interruptible(OSIF_WAIT_Q_ARG)
{
    wake_up_interruptible(wq) ;
}

/************************************************************************
*                             Timer Routines                            *
************************************************************************/
void _RP
lis_add_timer(struct timer_list * timer)
{
    add_timer(timer);
}

int  _RP
lis_del_timer(struct timer_list * timer)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 15, 0)
    return timer_delete(timer);
#else
    return del_timer(timer);
#endif
}

/************************************************************************
*                        Wrapped Functions                              *
************************************************************************/

#define __real_strcpy	strcpy
#define __real_strncpy	strncpy
#define __real_strcat	strcat
#define __real_strncat	strncat
#define __real_strcmp	strcmp
#define __real_strncmp	strncmp
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,0,0)
#define __real_strnicmp	strnicmp
#else
#define __real_strnicmp	strncasecmp
#endif
#define __real_strchr	strchr
#define __real_strrchr	strrchr
#define __real_strstr	strstr
#define __real_strlen	strlen
#define __real_memset	memset
#define __real_memcpy	memcpy
#define __real_memcmp	memcmp

char * _RP __wrap_strcpy(char *d,const char *s)
{
    return(__real_strcpy(d,s));
}

char * _RP __wrap_strncpy(char *d,const char *s, __kernel_size_t l)
{
    return(__real_strncpy(d,s,l));
}

char * _RP __wrap_strcat(char *d, const char *s)
{
    return(__real_strcat(d,s));
}

char * _RP __wrap_strncat(char *d, const char *s, __kernel_size_t l)
{
    return(__real_strncat(d,s,l));
}

int _RP __wrap_strcmp(const char *a,const char *b)
{
    return(__real_strcmp(a,b));
}

int _RP __wrap_strncmp(const char *a,const char *b,__kernel_size_t l)
{
    return(__real_strncmp(a,b,l));
}
int _RP __wrap_strnicmp(const char *a, const char *b, __kernel_size_t l)
{
    return(__real_strnicmp(a,b,l));
}
char * _RP __wrap_strchr(const char *s,int c)
{
    return(__real_strchr(s,c));
}

char * _RP __wrap_strrchr(const char *s,int c)
{
    return(__real_strrchr(s,c));
}

char * _RP __wrap_strstr(const char *a,const char *b)
{
    return(__real_strstr(a,b));
}

__kernel_size_t _RP __wrap_strlen(const char *s)
{
    return(__real_strlen(s));
}

void * _RP __wrap_memset(void *d,int v,__kernel_size_t l)
{
    return(__real_memset(d,v,l));
}

void * _RP __wrap_memcpy(void *d,const void *s,__kernel_size_t l)
{
    return(__real_memcpy(d,s,l));
}

int _RP __wrap_memcmp(const void *a,const void *b,__kernel_size_t l)
{
    return(__real_memcmp(a,b,l));
}


int _RP __wrap_sprintf(char *p, const char *fmt, ...)
{
    va_list	 args;
    int		 ret ;

    va_start (args, fmt);
    ret = vsprintf (p, fmt, args);
    va_end (args);
    return(ret) ;
}

int _RP __wrap_snprintf(char *p, size_t len, const char *fmt, ...)
{
    va_list	 args;
    int		 ret ;

    va_start (args, fmt);
    ret = vsnprintf (p, len, fmt, args);
    va_end (args);
    return(ret) ;
}

int _RP __wrap_vsprintf(char *p, const char *fmt, va_list args)
{
    return(vsprintf (p, fmt, args));
}

int _RP __wrap_vsnprintf(char *p, size_t len, const char *fmt, va_list args)
{
    return(vsnprintf (p, len, fmt, args));
}


