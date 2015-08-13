echo off
REM Setting Environment Variables
@set ADBDIR=E:\ADB

e:
cd E:\ADB
echo on

adb push swap.sh /data
adb shell chmod 777 /data/swap.sh
adb shell ./data/swap.sh


