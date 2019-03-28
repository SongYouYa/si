@echo off

choice /C 12 /m "1:∆’Õ®∞Ê£¨2:CD∞Ê"
if %errorlevel%==1 goto version2_nor
if %errorlevel%==2 goto version2_cd

:version2_nor
copy /Y include\version\version2_normal.h include\version\version2.h
goto version_logo

:version2_cd
copy /Y include\version\version2_cd.h include\version\version2.h
goto version_logo

:version_logo
choice /C 12 /m "1:∆’Õ®∞Ê£¨2:ŒﬁLOGO∞Ê"
if %errorlevel%==1 goto version_withlogo
if %errorlevel%==2 goto version_nologo

:version_withlogo
copy /Y include\version\version_withlogo.h include\version\version_logo.h
goto end

:version_nologo
copy /Y include\version\version_nologo.h include\version\version_logo.h
goto end

:end

pause