#!/bin/bash

N=10;
K=100;
S=1;
P1C="";
P2C="";
AI1="";
AI2="";

function check {
  local DX=$(( $1 > $3 ? $1 - $3 : $3 - $1 ))
  local DY=$(( $2 > $4 ? $2 - $4 : $4 - $2 ))
  local D=$(( DX > DY ? DX : DY ))
  if [[ $D > 7 ]];
  then
    echo 0
  else
    echo 1
  fi
}

function abs {
  echo $(( $1 > 0 ? $1 : -$1 ))
}

function random {
  case $1 in
    "-b")
      if [[ $2 -le 7 ]]
      then
        echo "-1,-1,-1,-1"
        exit 1
      fi 
      P1X=$(( (RANDOM % $N) + 1))
      P2X=$(( (RANDOM % $N) + 1))
      DX=`abs $((P1X - P2X))`
      if [[ $DX > 7 ]];
      then
        P1Y=$(( (RANDOM % $N) + 1))
        P2Y=$(( (RANDOM % $N) + 1))
      else
        Y1=$(( (RANDOM % ($N - 9)) + 1 ))
        Y2=$(( (RANDOM % ($N - $Y1 - 8)) + $Y1 + 8 ))
        Random=$((RANDOM % 2))
        if [[ $Random == 0 ]];
        then
          P1Y=$Y1
          P2Y=$Y2
        else
          P1Y=$Y2
          P2Y=$Y1
        fi
      fi
      echo "$P1X,$P1Y,$P2X,$P2Y"
      exit 0
      ;;
    "-s")
      if [[ $3 < 9 && $3 > $2-8 && $4 < 9 && $4 > $2-8 ]];
      then
        echo "-1,-1"
        exit 0
      fi
      if [[ $3 < 9 && $3 > $2-8 ]];
      then
        local X=$(( (RANDOM % $2) + 1 ))
        if [[ $4 < 9 ]];
        then
          local Y=$(( (RANDOM % ($2 - $4 - 7)) + $4 + 8 ))
        elif [[ $4 > $2-8 ]];
        then
          
          local Y=$(( (RANDOM % ($4 - 9)) + 1 ))
        else
          local Y1=$(( (RANDOM % ($2 - $4 - 7)) + $4 + 8))
          local Y2=$(( (RANDOM % ($4 - 9)) + 1 ))
          local Y=$(( RANDOM % 2 == 0 ? Y1 : Y2 ))
        fi
        echo "$X,$Y"
        exit 0
      fi
      if [[ $4 < 9 && $4 > $2-8 ]];
      then
        local Y=$(( (RANDOM % $2) + 1 ))
        if [[ $3 < 9 ]];
        then
          local X=$(( (RANDOM  % ($2 - $3 - 7)) + $3 + 8 ))
        elif [[ $3 > $2-8 ]];
        then

          local X=$(( (RANDOM % ($3 - 9)) + 1 ))
        else
          local X1=$(( (RANDOM % ($2 - $3 - 7)) + $3 + 8 ))
          local X2=$(( (RANDOM % ($3 - 8)) + 1 ))
          local X=$(( RANDOM % 2 == 0 ? X1 : Y1 ))
       fi
       echo "$X,$Y"
       exit 0 
      fi
      local X=$(( (RANDOM % $2) + 1))
      local Z=$(( X -$3 ))
      if [[ `abs $Z` < 8 ]];
      then
        if [[ $4-8 -gt 0 ]];
        then
          local Y1=$(( (RANDOM % ($4 - 8)) + 1 ))
          if [[ $2-$4-7 > 0 ]]
          then
            local Y2=$(( (RANDOM % ($2 - $4 - 7)) + $4 + 8 ))
            local Y=$(( RANDOM % 2 == 0 ? Y1 : Y2 ))
          else
            local Y=$Y2
          fi
        else
          local Y=$(( (RANDOM % ($2 - $4 - 7)) + $4 + 8 ))
        fi
      else
        local Y=$(( (RANDOM % $2) + 1))
      fi
      echo "$X,$Y"
  esac
}  
   
while [[ $# > 1 ]]; do
  P="$1"
  case $P in
    "-n") N=$2 ;;
    "-k") K=$2 ;;
    "-s") S=$2 ;;
    "-p1") P1C=$2 ;;
    "-p2") P2C=$2 ;;
    "-ai1") AI1=$2 ;;
    "-ai2") AI2=$2 ;;
    *) exit 1
  esac
  shift 2
done;

if [[ ! $N =~ ^[1-9][0-9]{0,8}$ || ! $K =~ ^[1-9][0-9]{0,9}$ || ! $S =~ ^[1-9][0-9]{0,9}$ ]];
then
  exit 1
fi

if [[ $P1C == "" ]];
then
  if [[ $P2C == "" ]];
  then
    P12=`random -b $N`
    P1X=$(echo $P12 | cut -f1 -d,)
    P1Y=$(echo $P12 | cut -f2 -d,)
    P2X=$(echo $P12 | cut -f3 -d,)
    P2Y=$(echo $P12 | cut -f4 -d,)
    P1C="$P1X,$P1Y"
    P2C="$P2X,$P2Y"
  else
    P2X=$(echo $P2C | cut -f1 -d,)
    P2Y=$(echo $P2C | cut -f2 -d,)
    P1C=`random -s $N $P2X $P2Y`
    P1X=$(echo $P1C | cut -f1 -d,)
    P1Y=$(echo $P1C | cut -f2 -d,)
  fi
elif [[ $P1C =~ ^[1-9][0-9]{0,9},[1-9][0-9]{0,9}$ ]];
then
  P1X=$(echo $P1C | cut -f1 -d,)
  P1Y=$(echo $P1C | cut -f2 -d,)
else
  exit 1
fi

if [[ $P2C == "" ]];
then
  P2C=`random -s $N $P1X $P1Y`
  P2X=$(echo $P2C | cut -f1 -d,)
  P2Y=$(echo $P2C | cut -f2 -d,)
  if [[ $P2X -eq -1 ]];
  then
    exit 1
  fi
elif [[ $P1C =~ ^[1-9][0-9]{0,9},[1-9][0-9]{0,9}$ ]];
then
  P2X=$(echo $P2C | cut -f1 -d,)
  P2Y=$(echo $P2C | cut -f2 -d,) 
  if [[ `check $P1X $P1Y $P2X $P2Y` != 0 ]];
  then
    exit 1
  fi
else
  exit 1
fi

if [[ $AI1 != "" && ! -x $AI1 ]]
then
  exit 1
fi

if [[ $AI2 != "" && ! -x $AI2 ]]
then
  exit 1
fi

PIPE=`mktemp -u`
mkfifo $PIPE
exec 3<>$PIPE
rm $PIPE

PIPE=`mktemp -u`
mkfifo $PIPE
exec 4<>$PIPE
rm $PIPE

GUI="./sredniowiecze_gui_with_libs.sh"

if [[ $AI1 == "" && $AI2 == "" ]];
then
  $GUI -human1 -human2 <&3 1>/dev/null 2>&1 &
  GUI_pid=$!
  echo "INIT $N $K 1 $P1X $P1Y $P2X $P2Y" >&3
  echo "INIT $N $K 2 $P1X $P1Y $P2X $P2Y" >&3
  wait $GUI_pid
  exit 0
fi

if [[ $AI1 != "" &&  $AI2 == "" ]];
then
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 5<>$PIPE
  rm $PIPE
  
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 6<>$PIPE
  rm $PIPE
  
  $GUI -human2 <&3 1>&4 2>/dev/null &
  GUI_pid=$!
  
  $AI1 <&5 1>&6 2>/dev/null &
  AI_pid=$!
  
  echo "INIT $N $K 1 $P1X $P1Y $P2X $P2Y" >&3
  echo "INIT $N $K 2 $P1X $P1Y $P2X $P2Y" >&3
  echo "INIT $N $K 1 $P1X $P1Y $P2X $P2Y" >&5
  PLAYER_N=1
  while true;
  do
    if [[ $PLAYER_N == 1 ]];
    then
      read out <&6
      echo $out >&3
      if [[ $out == "END_TURN" ]];
      then
        PLAYER_N=2
      fi
    else
      read out <&4
      echo $out >&5
      if [[ $out == "END_TURN" ]]
      then
        PLAYER_N=1
      fi
    fi
    if ! kill -0 $AI_pid 1>/dev/null 2>&1
    then
      wait -n $AI_pid
      OC=$?
      if [[ $OC < 0 || $OC > 2 ]];
      then
        exit 1
      fi
      while true;
      do
        read -t 1 out <&6
        OCR=$?
        if [[ $OCR != 0 ]];
        then
          exit 0
        fi
        echo $OCR >&3
      done
    elif ! kill -0 $GUI_pid 1>/dev/null 2>&1
    then
      kill $AI_pid
      wait $AI_pid
      exit 0
    fi
  done
fi

if [[ $AI1 == "" &&  $AI2 != "" ]];
then
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 5<>$PIPE
  rm $PIPE
  
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 6<>$PIPE
  rm $PIPE
  
  $GUI -human1 <&3 1>&4 2>/dev/null &
  GUI_pid=$!
  
  $AI2 <&5 1>&6 2>/dev/null &
  AI_pid=$!
  
  echo "INIT $N $K 1 $P1X $P1Y $P2X $P2Y" >&3
  echo "INIT $N $K 2 $P1X $P1Y $P2X $P2Y" >&3
  echo "INIT $N $K 1 $P1X $P1Y $P2X $P2Y" >&5
  PLAYER_N=1
  while true;
  do
    if [[ $PLAYER_N == 1 ]];
    then
      read out <&4
      echo $out >&5
      if [[ $out == "END_TURN" ]];
      then
        PLAYER_N=2
      fi
    else
      read out <&6
      echo $out >&3
      if [[ $out == "END_TURN" ]]
      then
        PLAYER_N=1
      fi
    fi
    if ! kill -0 $AI_pid 1>/dev/null 2>&1
    then
      wait -n $AI_pid
      OC=$?
      if [[ $OC < 0 || $OC > 2 ]];
      then
        exit 1
      fi
      while true;
      do
        read -t 1 out <&6
        OCR=$?
        if [[ $OCR != 0 ]];
        then
          exit 0
        fi
        echo $OCR >&3
      done
    elif ! kill -0 $GUI_pid 1>/dev/null 2>&1
    then
      kill $AI_pid
      wait $AI_pid
      exit 0
    fi
  done
fi

if [[ $AI1 != "" && $AI1 != "" ]];
then
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 5<>$PIPE
  rm $PIPE
  
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 6<>$PIPE
  rm $PIPE
  
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 7<>$PIPE
  rm $PIPE
  
  PIPE=`mktemp -u`
  mkfifo $PIPE
  exec 8<>$PIPE
  rm $PIPE
  
  $GUI <&3 1>&4 2>/dev/null &
  GUI_pid=$!
  
  $AI1 <&5 1>&6 2>/dev/null &
  AI1_pid=$!
  
  $AI2 <&7 1>&8 2>/dev/null &
  AI2_pid=$!
  
  echo "INIT $N $K 1 $P1X $P1Y $P2X $P2Y" >&3
  echo "INIT $N $K 1 $P1X $P1Y $P2X $P2Y" >&5
  echo "INIT $N $K 2 $P1X $P1Y $P2X $P2Y" >&3
  echo "INIT $N $K 2 $P1X $P1Y $P2X $P2Y" >&7
  
  PLAYER_N=1
  while true;
  do
    if [[ $PLAYER_N == 1 ]];
    then
      read out <&6
      echo $out >&7
      echo $out >&3
      if [[ $out == "END_TURN" ]];
      then
        PLAYER_N=2
      fi
    else
      read out <&8
      echo $out >&5
      echo $out >&3
      if [[ $out == "END_TURN" ]];
      then
        PLAYER_N=1
      fi
    fi
    sleep $S
    if ! kill -0 $AI1_pid 1>/dev/null 2>&1
    then
      wait -n $AI1_pid
      OC=$?
      if [[ $OC < 0 || $OC > 2 ]];
      then
        if kill -0 $AI2_pid 1>/dev/null 2>&1
        then
          kill $AI2_pid
          wait $AI2_pid
          exit 1
        else
          wait $AI2_pid
          exit 1
        fi
      else
        while true;
        do
          read -t 1 out <&6
          OCR=$?
          if [[ $OC != 0 ]]
          then
            break
          fi
          echo $out >&7
          echo $out >&3
        done
        if kill -0 $AI2_pid 1>/dev/null 2>&1
        then
          kill $AI2_pid
          wait $AI2_pid
          exit 1
        else
          wait -n $AI2_pid
          OC2=$?
          if [[ $OC == $OC2 ]];
          then
            while true;
            do
              read -t 1 out <&8
              OCR=$?
              if [[ $OCR != 0 ]];
              then
                 break
              fi
              echo $out >&3
            done
            exit 0
          else
            exit 1
          fi
        fi
      fi
    elif ! kill -0 $AI2_pid 1>/dev/null 2>&1
    then
      wait -n $AI2_pid
      OC=$?
      if [[ $OC < 0 || $OC > 2 ]];
      then
        kill $AI1_pid
        wait $AI1_pid
        exit 1
      else
        while true
        do
          read -t 1 out <&8
          OCR=$?
          if [[ $OCR != 0 ]];
          then
            break
          fi
          echo $out >&5
          echo $out >&3
        done
        if kill -0 $AI1_pid 1>/dev/null 2>&1
        then
          kill $AI1_pid
          wait $AI1_pid
          exit 1
        else
          wait -n $AI1_pid
          OC1=$?
          if [[ $OC == $OC1 ]];
          then
            while true;
            do
              read -t 1 out <&6
              OCR=$?
              if [[ $OCR != 0 ]];
              then
                break
              fi
              echo $out >&3
            done
            exit 0
          else
            exit 1
          fi
        fi
      fi
    elif ! kill -0 $GUI_pid 1>/dev/null 2>&1
    then
      wait -n $GUI_pid
      OC=$?
      kill $AI1_pid
      wait $AI1_pid
      kill $AI2_pid
      wait $AI2_pid
      if [[ $GUI_pid == 0 ]];
      then
        exit 0
      else
        exit 1
      fi
    fi
  done
fi

exit 0
