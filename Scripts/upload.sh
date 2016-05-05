#!/bin/sh
# Copyright 2016 Magic of Things <contact@magicofthings.fr>
# Date: February 2016
# based on
# Copyright 2010 Ovidiu Predescu <ovidiu@gmail.com>
# Date: June 2010


#Copy all the file modified less than 10 minutes ago from ContenuSD to SD card
SHUTTER=192.168.0.9

if [ $# != 0 ]; then
    FILES="$*"
else
    FILES=`find ContenuSD -type f \( -iname "*.*" ! -iname ".*" \) -mmin -10`
fi

echo `date`
for f in $FILES; do
    if [[ $(echo $f | egrep "~|CVS") ]]; then
	echo Skipping $f
    else
	size=`ls -l $f | awk '{print $5}'`
	echo "Uploading $f ($size bytes)"
        curl -0 -T $f http://$SHUTTER/upload/
    fi
done
