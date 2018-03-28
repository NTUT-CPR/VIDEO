#!/bin/sh

video=""
QP=""

while getopts "V:Q:" arg #选项后面的冒号表示该选项需要参数
do
        case $arg in
             V)
                video=$OPTARG
                ;;
             Q)
                QP=$OPTARG
                ;;
             ?)  #当有不认识的选项的时候arg为?
            echo "含有未知参数"
        exit 1
        ;;
        esac
done

str=${video}"_"${QP}".bin"

cd /home/lab1323/桌面/transcodingFile
./TAppEncoderStatic -c /mnt/nfs/videocfg/$video -c /mnt/nfs/encoder_lowdelay_main.cfg -q $QP -b /mnt/nfs/videoTranscode/$str > list.txt

filename='list.txt'
index="true"
while :
echo $index
do
exec < $filename
	while read line
	#sleep 1;
	do
		#echo $line
		if [ "${line:0:10}" = "Total Time" ]
		then
			touch finish.txt
			index="false"
		fi
	done
	if [ "$index" = "false" ]
		then
			break
		fi
done

#./TAppEncoderStatic -c /mnt/nfs/videocfg/ParkScene.cfg -c /mnt/nfs/encoder_lowdelay_main.cfg -q 22
