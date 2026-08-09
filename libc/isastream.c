/* #
   # Copyright 2022 - IBM Inc. All rights reserved
   # SPDX-License-Identifier: LGPL-2.1
# */
#ident "@(#) CSLiS isastream.c 7.11 2022-10-26 15:30:00 "
#include <stropts.h>
#include <sys/ioctl.h>


int	isastream(int fd)
{
    return (ioctl( fd, I_CANPUT, 0 ) != -1);
}
