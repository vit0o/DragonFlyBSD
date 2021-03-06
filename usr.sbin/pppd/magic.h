/*
 * magic.h - PPP Magic Number definitions.
 *
 * Copyright (c) 1989 Carnegie Mellon University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by Carnegie Mellon University.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * $FreeBSD: src/usr.sbin/pppd/magic.h,v 1.7 1999/08/28 01:19:05 peter Exp $
 * $DragonFly: src/usr.sbin/pppd/magic.h,v 1.3 2003/11/03 19:31:40 eirikn Exp $
 */

void magic_init(void);	/* Initialize the magic number generator */
u_int32_t magic(void);	/* Returns the next magic number */
