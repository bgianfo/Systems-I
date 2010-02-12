#!/bin/bash
HOST='dsl.cs.rit.edu'
USER='bjg1955'
PASSWD='..'
ftp -n -v $HOST << EOT
ascii
user $USER $PASSWD
prompt
cd sys
get db.c
get db.h
get project1.c
get project1.h
get common.h
get str.h
get str.c
put ../header.mak
EOT
