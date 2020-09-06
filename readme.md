PowerRequestWrapper is a tool to prevent your PC from going into Sleep mode while doing something critical.
Of course you can temporary change your power settings, but this tool simply creates a power lock to keep the PC awake during a critical operation.

##One important remark. 
Some virus scanners (i.e. Avira) will perform a Heuristic scan the first time calling PowerRequestWrapper with command line options. 
As a result the program will 'stall' for 30-60 seconds. Note, that happens only the first time. PowerRequestWrapper is not hanging.

##Two usage options:
Interactivemode: call PowerRequestWrapper without a command (option -c). PowerRequestWrapper will open a pop-up dialog. The lock will be released when you close that dialog
Wrapper mode: call PowerRequestWrapper with a command. PowerRequestWrapper will create a process for the command. and wait till the process is clossed to release the lock.
When everything works as expected, an option -n can be used to run without a console window. Only do that once it is wori=king, because you don't see any messages from PowerRequestWrapper anymore

##example 1, interactive in a console window:
```
C:\source\repos\PowerRequestWrapper\Release>PowerRequestWrapper
PowerRequestWrapper started: "Unlock by pressing OK in pop-up dialog".
```
A pop-up dialog will be launched
If you now can check the power locks (run in a console as administrator):
```
C:\WINDOWS\system32>powercfg -requests
DISPLAY:
None.

SYSTEM:
[PROCESS] \Device\HarddiskVolume5\source\repos\PowerRequestWrapper\Release\PowerRequestWrapper.exe
Unlock by pressing OK in pop-up dialog

AWAYMODE:
None.

EXECUTION:
None.

PERFBOOST:
None.

ACTIVELOCKSCREEN:
None.
```
After closing the pop-up dialog:
```
PowerRequestWrapper stopped: Power Lock released.

C:\source\repos\PowerRequestWrapper\Release>
```
If you now check the power locks again, to lock is released:
```
C:\WINDOWS\system32>powercfg -requests
DISPLAY:
None.

SYSTEM:
None.

AWAYMODE:
None.

EXECUTION:
None.

PERFBOOST:
None.

ACTIVELOCKSCREEN:
None.```
##example 2, wrapper mode, around newsleecher (from a shortcut):
```
"C:\source\repos\PowerRequestWrapper\Release>PowerRequestWrapper.exe" -c "C:\Program Files (x86)\NewsLeecher\NewsLeecher.exe" -m "Wrapping NewsLeecher to prevent PC going into Sleep mode" -n
```
The -m option specifies a dedicated message, -n surpresses the console window
If you now check the power locks:
```
C:\WINDOWS\system32>powercfg -requests
DISPLAY:
None.

SYSTEM:
[PROCESS] \Device\HarddiskVolume5\source\repos\PowerRequestWrapper\Release>PowerRequestWrapper.exe
Wrapping NewsLeecher to prevent PC going into Sleep mode

AWAYMODE:
None.

EXECUTION:
None.

PERFBOOST:
None.

ACTIVELOCKSCREEN:
None.
```
##example 3, wrapper mode, around newsleecher (from a batch file):
```
@echo off
start /MIN "NewsLeecher" "C:\source\repos\PowerRequestWrapper\Release>PowerRequestWrapper.exe" -n -c "C:\Program Files (x86)\NewsLeecher\NewsLeecher.exe" -m "Wrapping NewsLeecher to prevent PC going into Sleep mode"
```
Success.