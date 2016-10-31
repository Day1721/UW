#!/bin/bash

TEMP_OUT=`mktemp`;
TEMP_ERR=`mktemp`;

if [ "$1" != "-v" ];
then
  for f in $2*.in; do
    $1 <$f 1>$TEMP_OUT;
    DIFF_OUT=`diff ${f%in}out $TEMP_OUT`;
    if [ "$DIFF_OUT" != "" ];
    then
      echo "test of $f is wrong";
    fi;
  done;
else
  for f in $3*.in; do
    $2 -v <$f 1>$TEMP_OUT 2>$TEMP_ERR;
    DIFF_OUT=`diff ${f%in}out $TEMP_OUT`;
    DIFF_ERR=`diff ${f%in}err $TEMP_ERR`;
    if [ "$DIFF_OUT" != "" ];
    then
      echo "test of $f is wrong";
    else
      if [ "$DIFF_ERR" != "" ];
      then
        echo "test of $f is wrong";
      fi;
    fi;
  done;
fi;
