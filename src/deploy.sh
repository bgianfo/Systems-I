#!/bin/bash
HOST='dsl.cs.rit.edu'
USER='bjg1955'
PASSWD='..'
ftp -n -v $HOST << EOT
ascii
user $USER $PASSWD
prompt
cd sys
put db.c
put db.h
put project1.c
put project1.h
put common.h
put str.h
put str.c
put ../header.mak
EOT
