#!/bin/sh

onNo=2
offNo=0
angle=0
Snum=1

while true
do
    echo ${onNo} > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio${onNo}/direction
    echo  0 > /sys/class/gpio/gpio${onNo}/value

    onNo=`expr ${onNo} + 1`
    
    if [ ${onNo} = "22" ] ;then
        onNo=2
	break
    fi
done

while true
do
    echo "Controlling now..."
    echo "Number of Loudspeaker  ... ${Snum}"
    echo "angle ... ${angle}"

    echo ${onNo} > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio${onNo}/direction
    echo 1 > /sys/class/gpio/gpio${onNo}/value

    echo ${offNo} > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio${offNo}/direction
    echo 0 > /sys/class/gpio/gpio${offNo}/value

    offNo=${onNo}
    onNo=`expr ${onNo} + 1`
    angle=`expr ${angle} + 5`
    Snum=$((Snum + 1))
    
    if [ ${onNo} = "7" ] ;then
        onNo=9
    fi
    
    if [ ${onNo} = "22" ] ;then
        onNo=2
    fi
    
    if [ ${onNo} = "2" ] ;then
        offNo=21
    fi
    
    if [ ${angle} = "90" ] ;then
	angle=0
    fi
    
    if [ ${Snum} = "19" ] ;then
        Snum=1
    fi

    sleep 0.5s
done
