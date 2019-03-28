@echo off
REM This batch file written by Darren Freestone for Lock and Code Pty Ltd 
REM Email: darren@lockandcode.com

REM ---------------------------------------------------------------------------------------
REM variables used throughout script
REM ---------------------------------------------------------------------------------------
set script_folder=%~dp0
set running_volume=%~d0
set running_volume2=//.//%running_volume%
set log_file=%script_folder%..\WindowsSystemInformation.txt
set ascii_log1=%script_folder%ascii_log1.txt
set uni_header=%script_folder%\shared\unicode_header.txt
set find_cmd=%SystemRoot%\system32\find.exe

REM ---------------------------------------------------------------------------------------
REM test to determine the architecture, so the correct binaries can be used
REM ---------------------------------------------------------------------------------------

FOR /F "tokens=*" %%A IN ('wmic computersystem get SystemType /Value ^| %find_cmd% "="') DO (SET arch=%%A)

if /I "%arch%"=="SystemType=x64-based PC" GOTO Set64
if /I "%arch%"=="SystemType=x86-based PC" GOTO Set86
if /I "%arch%"=="SystemType=ARM-based PC" GOTO SetARM
GOTO Error2

:Set64
set drivenum=%script_folder%tools_x64\drivenum.exe
GOTO Main

:Set86
set drivenum=%script_folder%tools_x86\drivenum.exe
GOTO Main

:SetARM
set drivenum=%script_folder%tools_ARM\drivenum.exe
GOTO Main

:Main

REM ---------------------------------------------------------------------------------------
REM Main Program
REM ---------------------------------------------------------------------------------------

REM remove the log if it already exists, then start a new one using the unicode header
del %log_file% >NUL
COPY /Y %uni_header% %log_file% >NUL

REM determine which physical disk we want to excluding for reporing
FOR /F "delims=" %%i IN ('%drivenum% volume^=%running_volume2%') DO set num=%%i

IF NOT ERRORLEVEL 0 GOTO Error


echo Gathering System Information... >CON
systeminfo /s \\localhost > %ascii_log1%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      System Information                                                       >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C type %ascii_log1%                                                                  >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering Driver Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      Driver Information                                                       >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic sysdriver list brief                                                          >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering Disk Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      Disk Drive Information (excluding disk %num%)                            >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic diskdrive where Index!=%num% list full                                        >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering Partition Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      Partition Information (excluding disk %num%)                             >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic partition where DiskIndex!=%num% list brief                                   >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering Volume Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      Volume Information (excluding volume %running_volume%)                   >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic volume where DriveLetter!="%running_volume%" list brief                       >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering BIOS Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      BIOS Information                                                         >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic bios list brief                                                               >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering User Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      User Information                                                         >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic useraccount list brief                                                        >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering Service Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      Service Information                                                      >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic service list brief                                                            >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering Process Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      Process Information                                                      >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic process list brief                                                            >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering Timezone Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      Timezone Information                                                     >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic timezone list full                                                            >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Gathering NIC Information... >CON
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C echo      NIC Information                                                          >> %log_file%
CMD /U /C echo ----------------------------------------------------------------------------- >> %log_file%
CMD /U /C wmic nic list brief                                                                >> %log_file%
CMD /U /C echo.                                                                              >> %log_file%

echo Done.
del %ascii_log1%

notepad %log_file%

GOTO :EOF

REM ---------------------------------------------------------------------------------------
REM Error handling
REM ---------------------------------------------------------------------------------------


:Error
echo Problem running "%drivenum% volume=%running_volume2%", aborting....
pause
GOTO :EOF

:Error2
echo Problem identifying the architecture %arch%, aborting....
pause
GOTO :EOF
