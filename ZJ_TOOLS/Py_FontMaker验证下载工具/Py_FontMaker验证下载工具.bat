@echo off
::ping 127.0.0.1 -n 3 >nul


:菜单
echo ============================================
echo          Py_FontMaker验证下载工具
echo ============================================
echo    1.Jlink下载Font_LVGL
echo    2.Jlink下载Font
echo    3.Jlink擦除nrf52840
echo    4.Jlink下载Font_lib到norflash
echo ============================================

set /p 选择=请输入命令：
if %选择%==1 (goto 1Jlink下载Font_LVGL)
if %选择%==2 (goto 2Jlink下载Font)
if %选择%==3 (goto 3Jlink擦除nrf52840)
if %选择%==4 (goto 4Jlink下载Font_lib到norflash) else (goto 无效命令)   


:1Jlink下载Font_LVGL
.\JLink\jlink.exe .\Script\Jlink_download_nrf52840_lvgl.jlink
echo 下载完毕...


:pause>nul
goto 菜单

:2Jlink下载Font
.\JLink\jlink.exe .\Script\Jlink_download_nrf52840.jlink
echo 下载完毕...


:pause>nul
goto 菜单


:3Jlink擦除nrf52840
.\JLink\jlink.exe .\Script\Jlink_erase_nrf52840.jlink
echo 固件擦除完毕


:pause>nul
goto 菜单



:4Jlink下载Font_lib到norflash
.\JLink\JFlashSPI_CL.exe -speed 50000  -connect -open .\hex\Font_Lib.bin 0 -auto
echo Font_lib下载到norflash完毕


:pause>nul
goto 菜单

:无效命令
echo 无效的指令 指令范围（1-3）重新选择
goto 菜单