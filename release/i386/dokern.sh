#!/bin/sh
#
# $FreeBSD: src/release/i386/dokern.sh,v 1.58.2.7 2003/04/04 17:10:31 ru Exp $
# $DragonFly: src/release/i386/Attic/dokern.sh,v 1.3 2003/10/30 07:14:18 asmodai Exp $
#

sed	-e '/pty/d' \
	-e '/pass/d' \
	-e '/	agp/d' \
	-e '/	apm/d' \
	-e '/ppp/d' \
	-e '/gif/d' \
	-e '/faith/d' \
	-e '/gzip/d' \
	-e '/splash/d' \
	-e '/ICMP_BANDLIM/d' \
	-e '/PROCFS/d' \
	-e '/KTRACE/d' \
	-e '/SYSVMSG/d' \
	-e '/SOFTUPDATES/d' \
	-e '/UFS_DIRHASH/d' \
	-e '/MFS/d' \
	-e '/NFS_ROOT/d' \
	-e '/RANDOMDEV/d' \
	-e '/AHC_REG_PRETTY_PRINT/d' \
	-e '/AHD_REG_PRETTY_PRINT/d' \
	-e '/DDB/d' \
	-e '/INVARIANTS/d' \
	-e '/INVARIANT_SUPPORT/d' \
	-e '/	ncr/d' \
	-e '/	awi$/d' \
	-e '/atapist/d' \
	-e '/lpt/d' \
	-e '/ppi/d' \
	-e '/ugen/d' \
	-e '/uhid/d' \
	-e '/ulpt/d' \
	-e '/urio/d' \
	-e '/uscanner/d' \
	-e '/maxusers/d' \
	-e 's/ident.*GENERIC/ident		BOOTMFS/g'

echo "options  NETGRAPH"
echo "options  NETGRAPH_ETHER"
echo "options  NETGRAPH_PPPOE"
echo "options  NETGRAPH_SOCKET"

# reset maxusers to something lower
echo "maxusers	5"

echo "options  NFS_NOSERVER" 
echo "options  SCSI_NO_OP_STRINGS" 
echo "options  SCSI_NO_SENSE_STRINGS"
