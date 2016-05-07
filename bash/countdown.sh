for i in `seq 1 $1|tac` ; do echo "$i Minutes"  ; sleep 60 ; done ; notify-send -t 0 "Bash Alert" "$2";
# for i in `seq 1 $1|tac` ; do echo $i ; sleep 60 ; done ; notify-send -t 0 "Bash Alert" "$2";
# mplayer tardis_pan.wav.m4a;
# gmessage -center -nofocus -font 'Sans Bold 48' "Laundry Done"
# for i in `seq 1 100|tac` ; do echo $i ; sleep 60 ; done ; mp3c foo.mp3
