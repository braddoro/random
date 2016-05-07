#!/bin/bash

tick="eas-$1"
echo $tick
cvs co -R -d $tick eas
cd $tick
chmod -R 755 *
branch="eas-$1_BRANCH"
tag="eas-$1_TAG"
cvs tag $tag
cvs tag -b -r $tag $branch
cvs up -r $branch
ln -s /var/www/htdocs/isa/SmartClient_80_LGPL/ SmartClient_80_LGPL
