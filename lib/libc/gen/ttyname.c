/*
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)ttyname.c	8.2 (Berkeley) 1/27/94
 * $FreeBSD: src/lib/libc/gen/ttyname.c,v 1.24 2007/01/09 00:27:55 imp Exp $
 * $DragonFly: src/lib/libc/gen/ttyname.c,v 1.14 2005/11/19 22:32:53 swildner Exp $
 */

#include "namespace.h"
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <paths.h>
#include <errno.h>
#include <machine/stdint.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "reentrant.h"
#include "un-namespace.h"

#include "libc_private.h"

#define TTYNAME_DEVFS_COMPAT 1

static char ttyname_buf[sizeof(_PATH_DEV) + NAME_MAX];

static once_t		ttyname_init_once = ONCE_INITIALIZER;
static thread_key_t	ttyname_key;
static int		ttyname_keycreated = 0;

int
ttyname_r(int fd, char *buf, size_t len)
{
	struct stat	sb;
	struct fiodname_args fa;
	size_t used;

	*buf = '\0';

	/* Must be a valid file descriptor */
	if (_fstat(fd, &sb))
		return (EBADF);
	/* Must be a character device */
	if (!S_ISCHR(sb.st_mode))
		return (ENOTTY);
	/* Must be a terminal. */
	if (!isatty(fd))
		return (ENOTTY);
	/* Must have enough room */
	if (len <= sizeof(_PATH_DEV))
		return (ERANGE);

	strcpy(buf, _PATH_DEV);
	used = strlen(buf);
	fa.len = len - used;
	fa.name = buf + used;
	if (_ioctl(fd, FIODNAME, &fa) == -1) {
#ifdef TTYNAME_DEVFS_COMPAT
		/* If compat mode is set, fall back to old method */
		devname_r(sb.st_rdev, S_IFCHR, buf + used, len - used);
#else
		return ERANGE;
#endif
	}
	return (0);
}

static void
ttyname_keycreate(void)
{
	ttyname_keycreated = (thr_keycreate(&ttyname_key, free) == 0);
}

char *
ttyname(int fd)
{
	int	error;
	char	*buf;

	if (thr_main() != 0)
		buf = ttyname_buf;
	else {
		if (thr_once(&ttyname_init_once, ttyname_keycreate) != 0 ||
		    !ttyname_keycreated)
			return (NULL);
		if ((buf = thr_getspecific(ttyname_key)) == NULL) {
			if ((buf = malloc(sizeof ttyname_buf)) == NULL)
				return (NULL);
			if (thr_setspecific(ttyname_key, buf) != 0) {
				free(buf);
				return (NULL);
			}
		}
	}

	if (((error = ttyname_r(fd, buf, sizeof ttyname_buf))) != 0) {
		errno = error;
		return (NULL);
	}
	return (buf);
}
