@echo off
REM This batch file written by Darren Freestone for Lock and Code Pty Ltd 
REM Email: darren@lockandcode.com

REM ---------------------------------------------------------------------------------------
REM Change the following section of SET values below to suit your circumstances
REM and requirements.  Do not change other parts of this batch file unless you are 
REM sure of the consequences.
REM ---------------------------------------------------------------------------------------

set case_number="Case Number"
set evidence_number="Evidence Number"
set unique_description="Unique Description"
set examiner="Darren Freestone"
set notes_cdrive="Acquisition of C Drive on Microsoft Surface RT"
set output_prefix=WindowsDriveC
set device=\\.\\C:


REM ---------------------------------------------------------------------------------------
REM variables used elsewhere in the program
REM ---------------------------------------------------------------------------------------

set script_folder=%~dp0
set ipath=%script_folder%..\
set find_cmd=%SystemRoot%\system32\find.exe



REM ---------------------------------------------------------------------------------------
REM test to determine the architecture, so the correct binaries can be used
REM ---------------------------------------------------------------------------------------

FOR /F "tokens=*" %%A IN ('wmic computersystem get SystemType /Value ^| %find_cmd% "="') DO (SET arch=%%A)

if /I "%arch%"=="SystemType=x64-based PC" GOTO Set64
if /I "%arch%"=="SystemType=x86-based PC" GOTO Set86
if /I "%arch%"=="SystemType=ARM-based PC" GOTO SetARM
GO0TO Error2

:Set64
set drivenum=%script_folder%tools_x64\drivenum.exe
set dd=%script_folder%tools_x64\nano_dd.exe
set e01maker=%script_folder%tools_x64\e01maker.exe
set e01verify=%script_folder%tools_x64\e01verify.exe
GOTO Main

:Set86
set drivenum=%script_folder%tools_x86\drivenum.exe
set dd=%script_folder%tools_x86\nano_dd.exe
set e01maker=%script_folder%tools_x86\e01maker.exe
set e01verify=%script_folder%tools_x86\e01verify.exe
GOTO Main

:SetARM
set drivenum=%script_folder%tools_ARM\drivenum.exe
set dd=%script_folder%tools_ARM\nano_dd.exe
set e01maker=%script_folder%tools_ARM\e01maker.exe
set e01verify=%script_folder%tools_ARM\e01verify.exe
GOTO Main

:Main

REM ---------------------------------------------------------------------------------------
REM main part of the script, if nothing has failed above
REM ---------------------------------------------------------------------------------------


%dd% if=%device% | %e01maker% name=%ipath%%output_prefix% type=volume --fast case_number=%case_number% evidence_number=%evidence_number% unique_description=%unique_description% examiner=%examiner% notes=%notes_cdrive%

%e01verify% --target=%ipath%%output_prefix%.E01 --info

%e01verify% --target=%ipath%%output_prefix%.E01 --progress


pause
GOTO :EOF
