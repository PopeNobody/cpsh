#!/bin/bash

eval $(gpg < keys.asc)
export PGDATABASE=bittrex
export PATH=$PWD/bin:$PATH

exec bash

