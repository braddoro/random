#!/usr/local/bin/xdotool
#search --onlyvisible --classname $1
# windowsize %@ $2 $3
# windowraise %@
# windowmove %1 0 0
# windowmove %2 $2 0
# windowmove %3 0 $3
# windowmove %4 $2 $3
#exec php /home/bhughes/bin/busy.php 10
#windowmove --sync %@ 10 100
#sleep 1
#windowmove --relative %@ 200 200
#windowmove --sync %@ 10 100
#exec php /home/bhughes/bin/busy.php 10
#sleep 1
#windowmove --relative %@ 200 200
#windowmove --sync %@ 10 100
#sleep 1
#windowmove --relative %@ 200 200
#sleep 1
#--onlyvisible
# one
#
search $1
#search --name $1
#sleep 1
windowraise %@
windowmove %@ 0 30
windowsize %@ 25% 50%
#sleep 1
#windowminimize %@
#sleep 1
#windowraise %@

# two
#
#search --name $2
#sleep 10
#windowraise %@
#sleep 10
#windowminimize %@
#sleep 10
#windowraise %@
#
## three
##
#search --name $3
#sleep 10
#windowraise %@
#sleep 10
#windowminimize %@
#sleep 10
#windowraise %@
