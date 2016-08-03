
SET DIR=AllModel
SET TvMonDir=TVMon

cd ..
call clean.bat

cd %DIR%
call build_station.bat

call setup_config.bat

cd ..\..\..\%TvMonDir%\Manufacturing\TVMon\TvMon\project
TVMonBuild_QipP2.bat ..\..\..\..\..\Bltc\Build\%DIR%\
cd ..\..\..\..\..\Bltc\Build\%DIR%\
pause
REM call ..\..\..\TVMonBuild_QipP2.bat ..\..\..\Bltc\Build\%DIR%\
