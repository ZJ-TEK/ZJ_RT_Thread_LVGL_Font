@echo off
::ping 127.0.0.1 -n 3 >nul


:�˵�
echo ============================================
echo          Py_FontMaker��֤���ع���
echo ============================================
echo    1.Jlink����Font_LVGL
echo    2.Jlink����Font
echo    3.Jlink����nrf52840
echo    4.Jlink����Font_lib��norflash
echo ============================================

set /p ѡ��=���������
if %ѡ��%==1 (goto 1Jlink����Font_LVGL)
if %ѡ��%==2 (goto 2Jlink����Font)
if %ѡ��%==3 (goto 3Jlink����nrf52840)
if %ѡ��%==4 (goto 4Jlink����Font_lib��norflash) else (goto ��Ч����)   


:1Jlink����Font_LVGL
.\JLink\jlink.exe .\Script\Jlink_download_nrf52840_lvgl.jlink
echo �������...


:pause>nul
goto �˵�

:2Jlink����Font
.\JLink\jlink.exe .\Script\Jlink_download_nrf52840.jlink
echo �������...


:pause>nul
goto �˵�


:3Jlink����nrf52840
.\JLink\jlink.exe .\Script\Jlink_erase_nrf52840.jlink
echo �̼��������


:pause>nul
goto �˵�



:4Jlink����Font_lib��norflash
.\JLink\JFlashSPI_CL.exe -speed 50000  -connect -open .\hex\Font_Lib.bin 0 -auto
echo �̼��������


:pause>nul
goto �˵�

:��Ч����
echo ��Ч��ָ�� ָ�Χ��1-3������ѡ��
goto �˵�