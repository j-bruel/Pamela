#!/bin/sh

#if [ `whoami` != root ]; then
#    echo "[My PAMela]: Please run this script as root or using sudo.\n"
 #   exit
#fi

cd
if [ -d "secure_data-rw" ]; then
    echo "[My PAMela] [Test-#1] [Success]"
else
    echo "[My PAMela] [Test-#1] [Faillure]"
fi

if [ -f ".crypted_secure_data-rw" ]; then
    echo "[My PAMela] [Test-#2] [Success]"
else
    echo "[My PAMela] [Test-#2] [Faillure]"
fi

if ! [ -r ".crypted_secure_data-rw" ]; then
    echo "[My PAMela] [Test-#3] [Success]"
else
    echo "[My PAMela] [Test-#3] [Faillure]"
fi

