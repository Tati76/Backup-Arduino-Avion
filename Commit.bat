REM git add *
REM git commit -m ""
REM git push -u origin master


REM @echo off
 
title Project
 
set project="C:\Users\vince\Desktop\Git Code Arduino Avion"
set git="C:\Program Files\Git\bin" sh.exe -l
 
start /d %git% (cd %project% ) REM && git status && git add * && git commit -m "Auto commit" && git push -u origin master) 
REM && git --no-pager log -n 5 && cmd /k cd %project%)"