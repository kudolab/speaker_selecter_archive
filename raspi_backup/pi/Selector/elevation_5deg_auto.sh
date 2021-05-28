######################################################
#This shell can auto change a loudspeaker every 5 deg.
#
#October 18, 2016 by Shimokawara
######################################################

#!/bin/sh

onNo=2
offNo=0

#全てのスピーカをOFFにする
while true
do
    echo ${onNo} > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio${onNo}/direction
    echo  0 > /sys/class/gpio/gpio${onNo}/value

    onNo=$((onNo + 1))
    
    if [ ${onNo} = "11" ] ;then
	break
    fi
done


#/home/pi/share/angleから値を読み取りスピーカをONする 
S_num=0

while true
do

    if [ ${S_num} != $(cat /home/pi/share/angle) ] ;then

	S_num=$(cat /home/pi/share/angle)

	echo "Controlling now..."
        echo "Present speaker number ... ${S_num}"
	
	onNo=$(( S_num + 1 ))
	
	if [ $onNo -gt 6 ] ;then
	    onNo=$(( onNo + 2 ))
	fi

	echo "PORT num ... ${onNo} "
	echo ${onNo} > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio${onNo}/direction
	echo 1 > /sys/class/gpio/gpio${onNo}/value
	
	offNo=$((onNo - 1)) 
	
	if [ $onNo = 9 ] ;then
	    
	    offNo=6
	fi

	if [ $onNo = 2 ] ;then
	    offNo=10
	fi
	
	echo ${offNo} > /sys/class/gpio/export
	echo out > /sys/class/gpio/gpio${offNo}/direction
	echo 0 > /sys/class/gpio/gpio${offNo}/value
	
    fi

done
