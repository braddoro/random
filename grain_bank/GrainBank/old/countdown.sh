for i in `seq 1 $1|tac` ; do echo "$i Minutes"  ; sleep 60 ; done ; notify-send -t 0 "Bash Alert" "$2";

