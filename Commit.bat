REM git add *
REM git commit -m ""
REM git push -u origin master


REM @echo off
 
REM title Project
 
REM set project="C:\Users\vince\Desktop\Git Code Arduino Avion"
REM set git="C:\Program Files\Git\bin" sh.exe -l
 
REM start /d %git% (cd %project% ) REM && git status && git add * && git commit -m "Auto commit" && git push -u origin master) 
REM && git --no-pager log -n 5 && cmd /k cd %project%)"

@echo off
echo type "commit" or "update"
cd "curl"

set GIT_PATH="C:\Program Files\Git\bin\git.exe"
set BRANCH = "origin"

:P
set ACTION=
set /P ACTION=Action: %=%
if "%ACTION%"=="c" (
  %GIT_PATH% add -A
	%GIT_PATH% commit -am "Auto-committed on %date%"
	%GIT_PATH% pull %BRANCH%
	%GIT_PATH% push %BRANCH%
)
if "%ACTION%"=="u" (
	%GIT_PATH% pull %BRANCH%
)
if "%ACTION%"=="exit" exit /b
goto P