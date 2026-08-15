/************************************************************************
*                                                                       *
* Copyright 2022 - IBM Inc. All rights reserved                         *
* SPDX-License-Identifier: LGPL-2.1                                     *
*                                                                       *
*                      Operating System Interface                       *
*************************************************************************
*									*
* These are routines that call kernel routines indirectly.  This allows	*
* driver writers to reference the lis_... names in compiled code	*
* and the osif.c module itself will use the mangled names of the	*
* kernel routines when "modversions" is set.				*
*									*
* Include this file AFTER any linux includes.				*
*									*
************************************************************************/
#if !defined(OSIF_H)
#define OSIF_H		/* file included */

#ident "@(#) CSLiS osif.h 7.112 2025-05-28 15:30:00 "
#include <sys/LiS/genconf.h>
#include <linux/version.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 33)
#include <linux/autoconf.h>
#else
#include <generated/autoconf.h>
#endif

#ifndef RHEL_RELEASE_VERSION
#define RHEL_RELEASE_VERSION(a, b) (((a) << 8) + (b))
#endif

#if (defined(RHEL_RELEASE_CODE) && RHEL_RELEASE_CODE < RHEL_RELEASE_VERSION(9, 1))  //For RHEL 9 update 12-2022
#include <stdarg.h>                    /* for va_list */
#endif
#include <linux/wait.h>			/* for struct wait_queue */
#include <linux/timer.h>                /* for struct timer_list */
#include <linux/time.h>

#if !defined(INCL_FROM_OSIF_DRIVER)

/*
 * Redefine the "official" kernel names of these routines to the
 * "lis_..." names.
 *
 * Modversions has already defined most of these names to be the
 * version-mangled form.  So we have to test each one to see if
 * it has already been defined before redefining to the lis_... form.
 * This is messy but saves many warning messages when compiling
 * with modversions.
 */

#ifdef request_irq
#undef request_irq
#endif
#define	request_irq			lis_request_irq
#ifdef free_irq
#undef free_irq
#endif
#define	free_irq			lis_free_irq
#ifdef disable_irq
#undef disable_irq
#endif
#define	disable_irq			lis_disable_irq
#ifdef enable_irq
#undef enable_irq
#endif
#define	enable_irq			lis_enable_irq
#ifdef cli
# undef cli
#endif
#define cli                   		lis_osif_cli
#ifdef sti
# undef sti
#endif
#define sti                   		lis_osif_sti



#ifdef ioremap
#undef ioremap
#endif
#define	ioremap				lis_ioremap
#ifdef ioremap_nocache
#undef ioremap_nocache
#endif
#define	ioremap_nocache			lis_ioremap_nocache
#ifdef iounmap
#undef iounmap
#endif
#define	iounmap				lis_iounmap
#ifdef vremap
#undef vremap
#endif
#define	vremap				lis_vremap
#ifdef virt_to_phys
#undef virt_to_phys
#endif
#define virt_to_phys			lis_virt_to_phys
#ifdef phys_to_virt
#undef phys_to_virt
#endif
#define phys_to_virt			lis_phys_to_virt

#ifdef check_region
#undef check_region
#endif
#define	check_region			lis_check_region
#ifdef request_region
#undef request_region
#endif
#define	request_region			lis_request_region
#ifdef release_region
#undef release_region
#endif
#define	release_region			lis_release_region

#ifdef add_timer
#undef add_timer
#endif
#define add_timer			lis_add_timer
#ifdef del_timer
#undef del_timer
#endif
#define del_timer			lis_del_timer

#ifdef do_gettimeofday
#undef do_gettimeofday
#endif
#define do_gettimeofday			lis_osif_do_gettimeofday
#ifdef do_settimeofday
#undef do_settimeofday
#endif
#define do_settimeofday			lis_osif_do_settimeofday

#ifdef kmalloc
#undef kmalloc
#endif
#define	kmalloc				lis_kmalloc
#ifdef kfree
#undef kfree
#endif
#define	kfree				lis_kfree
#ifdef vmalloc
#undef vmalloc
#endif
#define	vmalloc				lis_vmalloc
#ifdef vfree
#undef vfree
#endif
#define	vfree				lis_vfree

#ifdef request_dma
#undef request_dma
#endif
#define	request_dma			lis_request_dma
#ifdef free_dma
#undef free_dma
#endif
#define	free_dma			lis_free_dma

#ifdef udelay
#undef udelay
#endif
#define	udelay				lis_udelay
#ifdef jiffies
#undef jiffies
#endif
#define	jiffies				lis_jiffies()

#ifdef printk
#undef printk
#endif
#define	printk				lis_printk
#ifdef sprintf
#undef sprintf
#endif
#define sprintf				lis_sprintf
#ifdef vsprintf
#undef vsprintf
#endif
#define	vsprintf			lis_vsprintf

#ifdef wait_event
#undef wait_event
#endif
#define	wait_event			lis_wait_event
#ifdef wait_event_interruptible
#undef wait_event_interruptible
#endif
#define	wait_event_interruptible	lis_wait_event_interruptible
#ifdef wake_up
#undef wake_up
#endif
#define	wake_up				lis_wake_up
#ifdef wake_up_interruptible
#undef wake_up_interruptible
#endif
#define	wake_up_interruptible		lis_wake_up_interruptible


#endif				/* !defined(INCL_FROM_OSIF_DRIVER) */


/*
 * IRQ routines
 */
#if defined(_LINUX_PTRACE_H)
#define	OSIF_REGS_T	struct pt_regs
#else
#define	OSIF_REGS_T	void
#endif

/*
 * Pre 2.6 kernels use a void routine for interrupt handling.  In 2.6
 * it is an int routine.  For portability in LiS we will always use
 * an int handler.  It will be called as a void in earlier kernels.
 * This should allow you do move your driver back and forth between
 * 2.4 and 2.6 without recompilation.
 *
 * If you need to, use one of the typedefs here to cast your routine
 * pointer to the proper type to keep the compiler from squawking.
 */
typedef int  _RP (*lis_int_handler) (int, void *, OSIF_REGS_T *) ;
typedef void _RP (*lis_void_handler)(int, void *, OSIF_REGS_T *) ;

/*
 * In 2.6 the kernel wants the irq handler to return 1 if it handled the
 * interrupt and 0 if not.  LiS will provide a pair of variables that
 * hold these values for portability.  The externs are here and the
 * variables are in osif.c.  The variables will exist even on a 2.4
 * kernel so your handler can be portable.
 */
extern int	lis_irqreturn_handled ;
extern int	lis_irqreturn_not_handled ;

int  lis_request_irq(unsigned int  irq,
		      lis_int_handler handler,
	              unsigned long flags,
		      const char   *device,
		      void         *dev_id) _RP;
void lis_free_irq(unsigned int irq, void *dev_id) _RP;
void lis_disable_irq(unsigned int irq) _RP;
void lis_enable_irq(unsigned int irq) _RP;
void lis_osif_cli( void )_RP;
void lis_osif_sti( void )_RP;


#if defined(_SPARC_LIS_) || defined(_SPARC64_LIS_)

/*
 * On the sparc we have the whole physical IO address space mapped at all
 * times, so ioremap() and ioremap_nocache() do not need to do anything.
 */
#undef ioremap
#undef ioremap_nocache
#undef iounmap

extern __inline__ void *sparc_ioremap(unsigned long offset, unsigned long size)
{
	return __va(offset);
}

#define	ioremap				sparc_ioremap
#define ioremap_nocache(offset, size)	sparc_ioremap((offset), (size))
#define iounmap(ptr)			/* nothing to do */

#endif

/*
 * Memory mapping routines
 */
void *lis_ioremap(unsigned long offset, unsigned long size) _RP;
void *lis_ioremap_nocache(unsigned long offset, unsigned long size) _RP;
void  lis_iounmap(void *addr) _RP;
void *lis_vremap(unsigned long offset, unsigned long size) _RP;
unsigned long lis_virt_to_phys(volatile void *addr) _RP;
void         *lis_phys_to_virt(unsigned long addr) _RP;

/*
 * I/O port routines <linux/ioport.h>
 */
int  lis_check_region(unsigned int from, unsigned int extent) _RP;
void lis_request_region(unsigned int from,
			 unsigned int extent,
			 const char  *name) _RP;
void lis_release_region(unsigned int from, unsigned int extent) _RP;


/*
 * Memory allocator <linux/malloc.h>
 */
void *lis_kmalloc(size_t nbytes, int type) _RP;
void  lis_kfree(const void *ptr) _RP;
void *lis_vmalloc(unsigned long size)_RP;
void  lis_vfree(void *ptr) _RP;


/*
 * DMA routines <asm/dma.h>
 */
int  lis_request_dma(unsigned int dma_nr, const char *device_id) _RP;
void lis_free_dma(unsigned int dma_nr) _RP;

/*
 * Delay routine in <linux/delay.h> and <asm/delay.h>
 */
void lis_udelay(long micro_secs) _RP;
unsigned long lis_jiffies(void) _RP;

/*
 * Printing routines.
 */
#define PRINTF_LIKE(a,b)	__attribute__ ((format (printf, a, b)))
int lis_printk(const char *fmt, ...) PRINTF_LIKE(1,2) _RP;
int lis_sprintf(char *bfr, const char *fmt, ...) PRINTF_LIKE(2,3) _RP;
int lis_vsprintf(char *bfr, const char *fmt, va_list args) PRINTF_LIKE(2,0) _RP;

/*
 * Timer routines.
 */
void lis_add_timer(struct timer_list * timer)_RP;
int  lis_del_timer(struct timer_list * timer)_RP;

/*
 * Time routines in <linux/time.h>
 */
void lis_osif_do_gettimeofday( struct timeval *tp ) _RP;
void lis_osif_do_settimeofday( struct timeval *tp ) _RP;

/*
 * Sleep/wakeup routines
 *
 * Note: It it only legitimate to use these in open and close
 * routines in STREAMS.  DO NOT sleep in put or service routines.
 */
#define	OSIF_WAIT_Q_ARG		wait_queue_head_t *wq
#define	OSIF_WAIT_E_ARG		wait_queue_head_t  wq
void lis_sleep_on(OSIF_WAIT_Q_ARG) _RP;
void lis_interruptible_sleep_on(OSIF_WAIT_Q_ARG) _RP;
void lis_sleep_on_timeout(OSIF_WAIT_Q_ARG, long timeout) _RP;
void lis_interruptible_sleep_on_timeout(OSIF_WAIT_Q_ARG, long timeout) _RP;
void lis_wait_event(OSIF_WAIT_E_ARG, int condition) _RP;
void lis_wait_event_interruptible(OSIF_WAIT_E_ARG, int condition) _RP;
void lis_wake_up(OSIF_WAIT_Q_ARG) _RP;
void lis_wake_up_interruptible(OSIF_WAIT_Q_ARG) _RP;


/*
 * Wrapped functions.
 *
 * These are some common functions that drivers might use and want to wrap
 * so that LiS can change the parameter passing convention prior to calling
 * the kernel's version of the function.
 */
extern char * _RP __wrap_strcpy(char *,const char *);
extern char * _RP __wrap_strncpy(char *,const char *, __kernel_size_t);
extern char * _RP __wrap_strcat(char *, const char *);
extern char * _RP __wrap_strncat(char *, const char *, __kernel_size_t);
extern int _RP __wrap_strcmp(const char *,const char *);
extern int _RP __wrap_strncmp(const char *,const char *,__kernel_size_t);
extern int _RP __wrap_strnicmp(const char *, const char *, __kernel_size_t);
extern char * _RP __wrap_strchr(const char *,int);
extern char * _RP __wrap_strrchr(const char *,int);
extern char * _RP __wrap_strstr(const char *,const char *);
extern __kernel_size_t _RP __wrap_strlen(const char *);
extern void * _RP __wrap_memset(void *,int,__kernel_size_t);
extern void * _RP __wrap_memcpy(void *,const void *,__kernel_size_t);
extern int _RP __wrap_memcmp(const void *,const void *,__kernel_size_t);
extern int _RP __wrap_sprintf(char *, const char *, ...);
extern int _RP __wrap_snprintf(char *, size_t, const char *, ...);
extern int _RP __wrap_vsprintf(char *, const char *, va_list);
extern int _RP __wrap_vsnprintf(char *, size_t, const char *, va_list);

#endif			/* from top of file */
