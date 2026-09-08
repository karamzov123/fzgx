#!/bin/sh
# Compile many sources in one mwcc run (the process start is most of a single compile),
# then write one depfile for the whole group in ninja's format.
#
#   mwcc_batch.sh <group depfile> <object dir> <source>... -- <wibo> <mwcc> <cflags>...
#
# On failure the sources are retried one at a time so the culprit is named:
#   FAILED unit: <source>
depfile=$1; basedir=$2; shift 2
n=0
while [ "$1" != "--" ]; do
    eval "src_$n=\$1"; n=$((n + 1)); shift
done
shift
i=0; srcs=""
while [ $i -lt $n ]; do
    eval "srcs=\"\$srcs \$src_$i\""; i=$((i + 1))
done
if ! "$@" -MMD -c $srcs -o "$basedir"; then
    for s in $srcs; do
        "$@" -MMD -c "$s" -o "$basedir" >/dev/null 2>&1 || echo "FAILED unit: $s" >&2
    done
    exit 1
fi
: > "$depfile"
for s in $srcs; do
    b=${s##*/}; b=${b%.*}
    # mwcc writes CRLF Windows paths: backslashes (the ` \` continuation must stay), and Z: for
    # absolute host paths
    tr -d '\r' < "$basedir/$b.d" | sed -e 's| \\$| @CONT@|' -e 's|\\|/|g' -e 's| @CONT@$| \\|' -e 's|\([ 	]\)[Zz]:|\1|g' >> "$depfile"
done
