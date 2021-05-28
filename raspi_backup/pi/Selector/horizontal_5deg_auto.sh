######################################################
#This shell can auto change a loudspeaker every 5 deg.
#
#June 30, 2016 by Shimokawara
######################################################

#!/bin/sh

onNo=2
offNo=0
S_num=0

while true
do
    if [ ${S_num} != $(cat /home/pi/share/angle) ] ;then
	S_num=$(cat /home/pi/share/angle)

	clear
	echo "###################################"
	echo "Controlling now..."
        echo "Present speaker number ... ${S_num}"
	echo "###################################"

        for offNo in `seq 1 19` ;do
            echo ${offNo} > /sys/class/gpio/export
            echo out > /sys/class/gpio/gpio${offNo}/direction
            echo 0 > /sys/class/gpio/gpio${offNo}/value
        done
	
	onNo=$(( S_num + 1 ))
       	echo ${onNo} > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio${onNo}/direction
	echo 1 > /sys/class/gpio/gpio${onNo}/value
    fi
done
