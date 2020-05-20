#!/bin/bash

set -e
eval "$(cat $HOME/.cpsh/keys.asc | gpg -d )"
exec bash

