#!/bin/bash

FILEPATH_REGEX='^([[:print:]]+)/{1}'
FILENAME_REGEX='(\w+)\.[c|cpp]$'

while [[ $1 ]]; do
	if [[ $1 =~ $FILENAME_REGEX ]]; then
		OUTPUT=${BASH_REMATCH[1]}

		if [[ $1 =~ $FILEPATH_REGEX ]]; then
			FILEPATH=${BASH_REMATCH[1]}
		fi		

		if [[ -z $FILEPATH ]]; then
			FILEPATH=`pwd`
		fi

		if [[ ! -d "$FILEPATH/gen" ]]; then
			$(mkdir $FILEPATH/gen)
		fi
		
		echo "target: $1, output: $FILEPATH/gen/$OUTPUT.o"
		gcc -std=c99 -Wall "$1" -o "$FILEPATH/gen/$OUTPUT.o" -lGL -lGLU -lglut
	fi
	shift
done
