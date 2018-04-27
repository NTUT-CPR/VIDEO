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

finishInfotxt=${video}".txt"
finishtxt=${video}"_"${QP}"_finish.txt"
cd /home/lab1323/桌面/TranscodingApi/
rm $finishInfotxt
rm $finishtxt
