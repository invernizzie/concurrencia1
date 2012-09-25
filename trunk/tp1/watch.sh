#!/bin/bash

FILENAME="$1"
if [ -z $FILENAME ]
then
	echo "No filename provided"
	exit 1
fi

LAST_DATE=$(stat -c "%Y" $FILENAME)

while true
do
	DATE=$(stat -c "%Y" $FILENAME)
	if [ $DATE -gt $LAST_DATE ]
	then
		pdflatex $FILENAME
		LAST_DATE="$DATE"
	fi
done
