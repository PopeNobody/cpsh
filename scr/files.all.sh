#!/bin/bash

test -z "${FILES_SCRIPTS_DEBUG}" || set -xv
set -e
not_v=('*.log' '*.new' 'lib*.a' 'lib*.a.lock' '*.[oid][oid]' '*.mk')

set -- $( printf ' -not -name %s ' "${not_v[@]}" )

test -e info/files.all && mv info/files.all info/files.all.old
find -name .git -prune , -type f "$@" -printf '%P\n' |
  grep -vf info/files.bin |
  sort -o info/files.all

set -- ${PIPESTATUS[@]}
for i; do (( !$i )) || exit 1; done
