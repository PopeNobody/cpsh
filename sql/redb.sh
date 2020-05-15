#!/bin/bash

echo 'drop database bittrex; create database bittrex;' | psql watcher
cat sql/schema.sql sql/data.sql sql/post.sql | psql "$@"
