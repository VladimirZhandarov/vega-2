MACHINE_TYPE=`uname -m`
if [ ${MACHINE_TYPE} = 'x86_64' ] ; then
   echo "ARCH: 64-bit"
   dmidecode > system_info.txt
elif [ ${MACHINE_TYPE} = 'i686' ] ; then
   echo "ARCH: 32-bit"
   dmidecode > system_info.txt
else
   echo "ARCH: Arm"
   lsusb -v | grep "iSerial                 3" >> system_info.txt
fi

