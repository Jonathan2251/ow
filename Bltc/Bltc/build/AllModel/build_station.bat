
SET TvMonReleaseDir=TVMon

REM --------RUN Gamma Version TSM---------------------------
..\createstation.exe stationscr.txt > createstation.output
move CreateStation.cpp ..\..\UserInterface\CreateStation.cpp
REM --------------------------------------------------------

del createstation.output tmp.txt
