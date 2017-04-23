#!/bin/bash

RANDOM=$$


if (( $# != 1 ))
then
	x=10
else
	x="$1"
fi	

filename="input$x.txt"
values=""

for i in $(seq $x)
do
	#concat the random values, adding a ',' after every number
	values="$values$RANDOM,"
done

# remove the last character. (the extra ',')
values=${values::-1}

# add a '.' at the end
values="$values."

# save content to filename.txt
echo "$values" >> $filename

echo $values
echo "The values were saved in $PWD/$filename"
