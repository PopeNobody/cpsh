#!/bin/bash

test -z "${FILES_SCRIPTS_DEBUG}" || set -xv
set -e
test -e info/files.git && mv info/files.git info/files.git.old
git ls-files | sort -o info/files.git
set -- ${PIPESTATUS[@]}
for i; do (( !$i )) || exit 1; done
