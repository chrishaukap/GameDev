!define APP_NAME "Tetrix"
!define COMP_NAME "mordekai"
!define WEB_SITE "http://www.evilmordekai.com"
!define VERSION "1.00.00.00"
!define COPYRIGHT "Chris Haukap  © 2008"
!define DESCRIPTION "Installer for Tetrix"
!define INSTALLER_NAME "TetrixSetup.exe"
!define MAIN_APP_EXE "tetrix.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!ifndef CONFIGURATION
   !error "ERROR - CONFIGURATION not set!"
!endif
!ifndef TARGET
   !error "ERROR - TARGET not set!"
!endif

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "..\dist\${TARGET}\${CONFIGURATION}\${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\Tetrix"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
   !insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!insertmacro MUI_PAGE_DIRECTORY

!ifdef REG_START_MENU
   !define MUI_STARTMENUPAGE_NODISABLE
   !define MUI_STARTMENUPAGE_DEFAULTFOLDER "Tetrix"
   !define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
   !define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
   !define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
   !insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH
!insertmacro MUI_LANGUAGE "English"

######################################################################
Section -MainProgram
   ${INSTALL_TYPE}
   SetOverwrite ifnewer
   
   SetOutPath "$INSTDIR"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\cfg.txt"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\tetrix.exe"
   
   SetOutPath "$INSTDIR\sounds"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\sounds\GAMEOVER.WAV"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\sounds\HWPNSEL.WAV"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\sounds\LEVCOMP.WAV"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\sounds\menu1.wav"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\sounds\MENUSEL.WAV"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\sounds\sp066.wav"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\sounds\techno.wav"
   
   SetOutPath "$INSTDIR\img"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\blah.bmp"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\block_24.bmp"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\menu_background_24.bmp"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\pointer_24.bmp"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\popup_24.bmp"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\start_button_out_24.bmp"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\start_button_over_24.bmp"
   File "$%PROJ_DIR%\bin\${TARGET}\${CONFIGURATION}\img\tetrisback_24.bmp"
SectionEnd

######################################################################

Section -Icons_Reg
   SetOutPath "$INSTDIR"
   WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
   !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
   CreateDirectory "$SMPROGRAMS\$SM_Folder"
   CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
   CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
   CreateShortCut "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

   !ifdef WEB_SITE
      WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
      CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
   !endif
      !insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
   CreateDirectory "$SMPROGRAMS\Tetrix"
   CreateShortCut "$SMPROGRAMS\Tetrix\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
   CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
   CreateShortCut "$SMPROGRAMS\Tetrix\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

   !ifdef WEB_SITE
      WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
      CreateShortCut "$SMPROGRAMS\Tetrix\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
   !endif
!endif

   WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
   WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
   WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
   WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
   WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
   WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
   WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif
SectionEnd

######################################################################

Section Uninstall
   ${INSTALL_TYPE}
   Delete "$INSTDIR\cfg.txt"
   Delete "$INSTDIR\tetrix.exe"
   Delete "$INSTDIR\sounds\GAMEOVER.WAV"
   Delete "$INSTDIR\sounds\HWPNSEL.WAV"
   Delete "$INSTDIR\sounds\LEVCOMP.WAV"
   Delete "$INSTDIR\sounds\menu1.wav"
   Delete "$INSTDIR\sounds\MENUSEL.WAV"
   Delete "$INSTDIR\sounds\sp066.wav"
   Delete "$INSTDIR\sounds\techno.wav"
   Delete "$INSTDIR\img\blah.bmp"
   Delete "$INSTDIR\img\block_24.bmp"
   Delete "$INSTDIR\img\menu_background_24.bmp"
   Delete "$INSTDIR\img\pointer_24.bmp"
   Delete "$INSTDIR\img\popup_24.bmp"
   Delete "$INSTDIR\img\start_button_out_24.bmp"
   Delete "$INSTDIR\img\start_button_over_24.bmp"
   Delete "$INSTDIR\img\tetrisback_24.bmp"
    
   RmDir "$INSTDIR\img"
   RmDir "$INSTDIR\sounds"
    
   Delete "$INSTDIR\uninstall.exe"
   !ifdef WEB_SITE
      Delete "$INSTDIR\${APP_NAME} website.url"
   !endif

   RmDir "$INSTDIR"

   !ifdef REG_START_MENU
      !insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
      Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
      Delete "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk"
      !ifdef WEB_SITE
         Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
      !endif
      Delete "$DESKTOP\${APP_NAME}.lnk"

      RmDir "$SMPROGRAMS\$SM_Folder"
   !endif

   !ifndef REG_START_MENU
      Delete "$SMPROGRAMS\Tetrix\${APP_NAME}.lnk"
      Delete "$SMPROGRAMS\Tetrix\Uninstall ${APP_NAME}.lnk"
      !ifdef WEB_SITE
         Delete "$SMPROGRAMS\Tetrix\${APP_NAME} Website.lnk"
      !endif
      Delete "$DESKTOP\${APP_NAME}.lnk"

      RmDir "$SMPROGRAMS\Tetrix"
   !endif

   DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
   DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

