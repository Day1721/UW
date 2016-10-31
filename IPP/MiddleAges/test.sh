#!/bin/bash

TEMP_OUT=`mktemp`;
TEMP_ERR=`mktemp`;
echo "testing...";

for f in $2*.in; do
  $1 <$f 1>$TEMP_OUT 2>$TEMP_ERR;
  DIFF_OUT=`diff ${f%in}out $TEMP_OUT`;
  DIFF_ERR=`diff ${f%in}err $TEMP_ERR`;
  if [ "$DIFF_OUT" != "" ];
  then
    echo "test of $f is wrong";
  else
    if [ "$DIFF_ERR" != "" ];
    then
      echo "test of $f is wrong";
    else
      echo "test of $f is OK";
    fi;
  fi;
done;
