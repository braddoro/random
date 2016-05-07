#!/bin/bash

#notify-send "The Log." "`cat ns.txt`" -t 1000
notify-send "The Log." "`weather 28027`" -t 1000

#for i in {0..100..10}
#    do
#          killall notify-osd
#          notify-send "testing" $i
#          sleep 1
#    done
