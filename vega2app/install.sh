#!/bin/sh

echo "*********************************************************************"
echo " Install boost lib "
echo "*********************************************************************"

sudo apt-get install -y libboost-all-dev

echo "*********************************************************************"
echo " Install video lib "
echo "*********************************************************************"

sudo apt-get install -y libva-dev libxvidcore-dev libopencore-amrnb-dev libopencore-amrwb-dev libmp3lame-dev libfaac-dev

echo "*********************************************************************"
echo " Install GTK lib "
echo "*********************************************************************"

sudo apt-get install -y libgtk2.0-0 libgtk2.0-dev

echo "*********************************************************************"
echo " Install Added lib "
echo "*********************************************************************"

sudo apt-get install -y libv4l-0 libvorbis-dev libtheora-dev
