#!/bin/sh

video=""
QP=""

while getopts "V:Q:" arg
do
	case $arg in
	      V)
		  video=$OPTARG
	  	;;
	      Q)
		  QP=$OPTARG
	  	;;
	       ?)
		echo "no exist parameter"
       exit 1
	;;
	esac
done

bin=${video}".bin"
yuv=${video}".yuv"
txt=${video}".txt"
cd /home/lab1323/桌面/TranscodingApi/
./TAppDecoderStatic -b /home/lab1323/桌面/NFS_Server/video/$bin -o /home/lab1323/桌面/NFS_Server/video/$yuv > $txt

filename=${txt}
finish=${video}"_"${QP}"_finish.txt"

index="true"
while :
do
exec < $filename
	while read line
	
	do
		if [ "${line:0:10}" = "Total Time" ]
		then
			touch $finish
			index="false"
		fi
	done
	if [ "$index" = "false" ]
		then
			break
	fi
done
