#!/bin/bash
cd /home/bhughes/dev/sandboxes/credentials
echo ''
echo 'update credentials'
echo '--------------------------'
cvs -q up -dP

cd /home/bhughes/dev/sandboxes/database
echo ''
echo 'update database'
echo '--------------------------'
cvs -q up -dP

cd /home/bhughes/dev/sandboxes/shared
echo ''
echo 'update shared'
echo '--------------------------'
cvs -q up -dP

cd /home/bhughes/dev/sandboxes/eas
echo ''
echo 'update eas'
echo '--------------------------'
cvs -q up -dP

#
#cd /home/bhughes/dev/sandboxes/isa2
#echo 'update isa2'
#echo '--------------------------'
#cvs -q up -dP
#
#cd /home/bhughes/dev/sandboxes/portal
#echo 'update portal'
#echo '--------------------------'
#cvs -q up -dP
#echo ''
#echo ''
