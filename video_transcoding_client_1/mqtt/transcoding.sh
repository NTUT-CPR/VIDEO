#!/bin/sh
videoorg=""
video=""
QP=""

while getopts "O:V:Q:" arg #选项后面的冒号表示该选项需要参数
do
        case $arg in
	      O)
                videoorg=$OPTARG
                ;;
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
cfg=${videoorg}".cfg"
yuv=${video}".yuv"
source_width_height=""
echo ${str}
cd /mnt/nfs/videocfg
filename_width_Height=${cfg}

exec < $filename_width_Height
	while read line
	do
	#sleep 1;
		#echo $line
		echo ${line:0:11}
		if [ "${line:0:11}" = "SourceWidth" ]
		then
			width=${line:22:26}
		fi
	
		if [ "${line:0:12}" = "SourceHeight" ]
		then
			height=${line:22:27}
		fi
	done
source_width_height=${width}"x"${height}
echo ${source_width_height}
cd /home/lab1323/桌面/transcodingFile
kvazaar -i /mnt/nfs/video/$yuv -q $QP --input-res $source_width_height --threads 2 --tiles 4x2 --gop 8 --frame 9 -o /mnt/nfs/videoTranscode/$str

touch finish.txt

#cd /home/lab1323/桌面/transcodingFile
#./TAppEncoderStatic -c /mnt/nfs/videocfg/$cfg -c /mnt/nfs/encoder_randomaccess_main.cfg -q $QP -b /#mnt/nfs/videoTranscode/$str > list.txt

#filename='list.txt'
#index="true"
#while :
#echo $index
#do
#exec < $filename
#	while read line
#	do
#		if [ "${line:0:10}" = "Total Time" ]
#		then
#			touch finish.txt
#			index="false"
#		fi
#	done
#	if [ "$index" = "false" ]
#		then
#			break
#		fi
#done

#./TAppEncoderStatic -c /mnt/nfs/videocfg/ParkScene.cfg -c /mnt/nfs/encoder_lowdelay_main.cfg -q 22
