#!/bin/bash

echo "the file: $1"

number_of_offsets=$(od -v -A n --endian big -t u2 -j 76 -N 2 "$1")
offset_ui=$(od -v -A n --endian big -t u -j 78 -N 4 "$1")
mobi_length=$(od -v -A n --endian big -t u -j $((offset_ui+16+4)) -N 4 "$1" )
exth_start=$((mobi_length+16+offset_ui))
exth_length=$(od -v -A n --endian big -t u -j $((exth_start+4)) -N 4 "$1" )
echo mobi length: "$mobi_length"
echo "exth length:" "$exth_length"
printf "exth start:  %#x, exth end without padding: %#x \n" "$exth_start" $((exth_start+exth_length))
od -v -t x1z -A x -j $((exth_start+exth_length-16)) -N 32 "$1"

echo "number of offsets:  $number_of_offsets"
for i in {0..5}; do 
offset_hex=$(od -v -A n --endian big -t x -j $((78+i*8)) -N 4 "$1")
next_offset_hex=$(od -v -A n --endian big -t x -j $((78+(i+1)*8)) -N 4 "$1")
offset_ui=$(od -v -A n --endian big -t u -j $((78+i*8)) -N 4 "$1")
next_offset_ui=$(od -v -A n --endian big -t u -j $((78+(i+1)*8)) -N 4 "$1")
printf "%d'th record at: %#x-%#x \t with length %d \t \n " "$i" "$offset_ui" "$next_offset_ui" $((next_offset_ui-offset_ui))
od -v -t x1z -A n -j $((offset_ui)) -N 16 "$1"
od -v -t x1z -A n -j $((next_offset_ui-16)) -N 16 "$1"
# with length: $(($next_offset_ui-$offset_ui)) ""
done

for((i=0; i< "$number_of_offsets"; i++)); do
offset_ui=$(od -v -A n --endian big -t u -j $((78+i*8)) -N 4 "$1")
# printf "%#x \n" "$offset_ui"
if [ offset_ui = 0 ]
then echo "$i" th records offset is 0
fi 
done