
setlocal
pushd

SET TvMonReleaseDir=TVMon

echo.
echo ******************************************************************************
echo ***************  Clean Bltc\UserInterface\CreateStation.cpp  *****************
echo ******************************************************************************
REM del ..\UserInterface\CreateStation.cpp

echo.
echo ******************************************************************************
echo ***************  Clean TvMon\Libs\Broadcom_Hal\src\TvMonIO\  *****************
echo ******************************************************************************
REM del ..\..\TvMon\Libs\Broadcom_Hal\src\TvMonIO\ModelConfig.h ..\..\TvMon\Libs\Broadcom_Hal\src\TvMonIO\ComCfg.h

cd ..\..\
echo.
echo ******************************************************************************
echo ***************  Clean *.list, *.bak in root dir *****************
echo ******************************************************************************
del *.list *.bak *.1 *.2 *.keep *.contrib *.old *.updt *.pgp *.jpg /S /q /f

cd Bltc\Build
echo.
echo ******************************************************************************
echo ***************  Clean dir Bltc\Build\  *****************
echo ******************************************************************************
del *.s *.bin /q /f

echo.
echo ******************************************************************************
echo ***************  Clean dir Bltc\Build\AllMOdel\  *****************
echo ******************************************************************************
del AllModel\*.s AllModel\*.bin AllModel\error_list.txt AllModel\station_list.txt /q /f

cd ..\..\%TvMonReleaseDir%\DctBsp\Deployment\Libraries
echo.
echo ******************************************************************************
echo ***************  Clean dir DctBsp\Deployment\Libraries\  *****************
echo ******************************************************************************
REM del *.lib  /q /f


cd ..\..\..\Manufacturing\TVMon\DctBspUser\Project
echo.
echo ******************************************************************************
echo ***************  Clean dir DctBspUser\project\  *****************
echo ******************************************************************************
rmdir/S/q BSPuser_Data
del *.lib  /q /f

cd ..\..\TvMon\Libs\Broadcom_3250\project
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\Broadcom_3250\project\  ****************
echo ******************************************************************************
rmdir/S/q bcmhal3250_Data
del bcmhal3250_Debug.lib  /q /f

cd ..\BcmBoard
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\Broadcom_3250\BcmBoard\  ***************
echo ******************************************************************************
rmdir/S/q bcmBoardLib_Data
del BcmBoard.lib  /q /f

cd ..\QamLib
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\Broadcom_3250\QamLib\  *****************
echo ******************************************************************************
rmdir/S/q QamLib_Data
del Qam.lib  /q /f

cd ..\TunerLib
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\Broadcom_3250\TunerLib\  *****************
echo ******************************************************************************
rmdir/S/q TunerLib_Data
del Tuner.lib /q /f

cd ..\..\Broadcom_Bsp\project
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\Broadcom_Bsp\project\  *****************
echo ******************************************************************************
rmdir/S/q Broadcom_Bsp_Data
del Broadcom_Bsp.lib  /q /f

cd ..\..\Broadcom_Hal\project
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\Broadcom_Hal\project\  *****************
echo ******************************************************************************
rmdir/S/q bcmhal3255_Data bcmhal7400_Data
del bcmhal3255_debug.lib bcmhal7400_debug.lib  /q /f

cd ..\..\Broadcom_TvMon\project
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\Broadcom_TvMon\project\  *****************
echo ******************************************************************************
rmdir/S/q Broadcom_TvMon_Data
del Broadcom_TvMon.lib  /q /f

cd ..\..\TokenDatabaseReader\TokenMgr\Project
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Libs\TokenDatabaseReader\TokenMgr\Project\  *****************
echo ******************************************************************************
rmdir/S/q BigInteger_Data TokenMgr_Data SystemSecurity_Data
del BigInteger.lib TokenMgr.lib SystemSecurity.lib /q /f

cd ..\..\..\..\Project
echo.
echo ******************************************************************************
echo ***************  Clean dir TvMon\Project\  *****************
echo ******************************************************************************
rmdir/S/q MotorolaTvMonDcx_Data MotorolaTvMonQiph_Data MotorolaTvMonQip7kP2_Data MotorolaTvMonQIP71XX_Data
del *.s *.bin *.obj *.xMap *.elf *.idb *.1 *.contrib /q /f
del MotorolaTvMonQiph.mcp.keep Compressed.lcf Compressed_boot.lcf Compressed_dl.lcf CompressedDebug.lcf CompressedDebug_dl.lcf   /q /f

popd
