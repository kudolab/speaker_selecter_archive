####################################################                                                                                                                                                                                                                                                                                                                                          
This shell can change a loudspeaker every 5 deg.

June 17, 2016 by Shimokawara
####################################################                                                                                                                                                                                                                                                                                                                                          

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

    onNo=$((onNo + 2))

    if [ ${onNo} = "22" ] ;then
        onNo=2
        break
    fi
done

while true
do
    echo "If your ready, press ENTER key"
    clear

    echo "Controlling now..."
    echo "Number of Loudspeaker  ... ${Snum}"
    echo "angle ... ${angle}"
    read enter

    echo ${onNo} > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio${onNo}/direction
    echo 1 > /sys/class/gpio/gpio${onNo}/value

    echo ${offNo} > /sys/class/gpio/export
    echo out > /sys/class/gpio/gpio${offNo}/direction
    echo 0 > /sys/class/gpio/gpio${offNo}/value

    offNo=${onNo}
    onNo=$((onNo + 2))
    angle=$((angle + 10))
    Snum=$((Snum + 2))

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

    echo "If you will finish  ... Please input n"
    echo "Play  next speake   ... Input y"

    read RESPONSE
    if [ ${RESPONSE} = "n" ] ;then
        break
    fi

    clear
done

echo 0 > /sys/class/gpio/gpio${offNo}/value
echo "finish!"
