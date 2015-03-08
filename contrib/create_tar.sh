#!/bin/bash
#
# Create release tarball
#

if test $# -ne 1;
then
    echo "Usage: create_tar.sh <ReleaseNum>"
    exit 1
fi

RELNUM=$1

git tag ${RELNUM}
git archive --format=tar --prefix=libdontdie-${RELNUM}/ ${RELNUM} | xz -9 >libdontdie-${RELNUM}.tar.xz
