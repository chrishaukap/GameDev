!define APP_NAME "ScribbleWars"
!define COMP_NAME "EvilMordekai"
!define WEB_SITE "http://evilmordekai.com/blog/?page_id=37"
!define VERSION "1.00.00.00"
!define COPYRIGHT "Chris Haukap  © 2008"
!define DESCRIPTION "Installer for ${APP_NAME}"
!define INSTALLER_NAME "${APP_NAME}_Setup.exe"
!define MAIN_APP_EXE "${APP_NAME}.exe"
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
InstallDir "$PROGRAMFILES\${APP_NAME}"

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
   !define MUI_STARTMENUPAGE_DEFAULTFOLDER "${APP_NAME}"
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
   File "$%PROJ_DIR%\bin\${CONFIGURATION}\${APP_NAME}.exe"
   File "$%PROJ_DIR%\inst\glut32.dll"
   File "$%PROJ_DIR%\..\lib_openAL\redist\OpenAL32.dll"
   
   SetOutPath "$INSTDIR\sound"
   File "$%PROJ_DIR%\etc\sound\1.ogg"
   File "$%PROJ_DIR%\etc\sound\2.ogg"
   File "$%PROJ_DIR%\etc\sound\HeroDeath.ogg"
   File "$%PROJ_DIR%\etc\sound\HeroHit.ogg"
   File "$%PROJ_DIR%\etc\sound\HeroHurt1.ogg"
   File "$%PROJ_DIR%\etc\sound\HeroHurt2.ogg"
   File "$%PROJ_DIR%\etc\sound\HeroJump.ogg"
   File "$%PROJ_DIR%\etc\sound\HeroPickup.ogg"
   File "$%PROJ_DIR%\etc\sound\SGDieOGG.ogg"
   File "$%PROJ_DIR%\etc\sound\SGHitOgg.ogg"
   File "$%PROJ_DIR%\etc\sound\SGSpawnOGG.ogg"
   
   SetOutPath "$INSTDIR\images"
   File "$%PROJ_DIR%\etc\images\backdrop.bmp"
   File "$%PROJ_DIR%\etc\images\bullet.bmp"
   File "$%PROJ_DIR%\etc\images\bullet_mask.bmp"
   File "$%PROJ_DIR%\etc\images\boss1.bmp"
   File "$%PROJ_DIR%\etc\images\boss1_mask.bmp"
   File "$%PROJ_DIR%\etc\images\boss2.bmp"
   File "$%PROJ_DIR%\etc\images\boss2_mask.bmp"
   File "$%PROJ_DIR%\etc\images\platformblock.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f1.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f1_mask.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f2.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f2_mask.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f3.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f3_mask.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f4.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f4_mask.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f5.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f5_mask.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f6.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f6_mask.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f7.bmp"
   File "$%PROJ_DIR%\etc\images\scribbleman_walk_f7_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame1.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame1_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame2.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame2_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame3.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame3_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame4.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame4_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame5.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame5_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame6.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame6_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame7.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame7_mask.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame8.bmp"
   File "$%PROJ_DIR%\etc\images\walkcycle_right_frame8_mask.bmp"
   
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
   CreateDirectory "$SMPROGRAMS\${APP_NAME}"
   CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
   CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
   CreateShortCut "$SMPROGRAMS\${APP_NAME}\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

   !ifdef WEB_SITE
      WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
      CreateShortCut "$SMPROGRAMS\${APP_NAME}\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
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
   Delete "$INSTDIR\${APP_NAME}.exe"     
      
   Delete "$INSTDIR\images\backdrop.bmp"
   Delete "$INSTDIR\images\bullet.bmp"
   Delete "$INSTDIR\images\bullet_mask.bmp"
   Delete "$INSTDIR\images\boss1.bmp"
   Delete "$INSTDIR\images\boss1_mask.bmp"
   Delete "$INSTDIR\images\boss2.bmp"
   Delete "$INSTDIR\images\boss2_mask.bmp"
   Delete "$INSTDIR\images\platformblock.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f1.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f1_mask.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f2.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f2_mask.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f3.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f3_mask.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f4.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f4_mask.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f5.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f5_mask.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f6.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f6_mask.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f7.bmp"
   Delete "$INSTDIR\images\scribbleman_walk_f7_mask.bmp"   
   
   Delete "$INSTDIR\images\walkcycle_right_frame1.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame1_mask.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame2.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame2_mask.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame3.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame3_mask.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame4.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame4_mask.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame5.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame5_mask.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame6.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame6_mask.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame7.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame7_mask.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame8.bmp"
   Delete "$INSTDIR\images\walkcycle_right_frame8_mask.bmp"
      
   Delete "$INSTDIR\images"
   
   Delete "$INSTDIR\sound\1.ogg"
   Delete "$INSTDIR\sound\2.ogg"
   Delete "$INSTDIR\sound\HeroDeath.ogg"
   Delete "$INSTDIR\sound\HeroHit.ogg"
   Delete "$INSTDIR\sound\HeroHurt1.ogg"
   Delete "$INSTDIR\sound\HeroHurt2.ogg"
   Delete "$INSTDIR\sound\HeroJump.ogg"
   Delete "$INSTDIR\sound\HeroPickup.ogg"
   Delete "$INSTDIR\sound\SGDieOGG.ogg"
   Delete "$INSTDIR\sound\SGHitOgg.ogg"
   Delete "$INSTDIR\sound\SGSpawnOGG.ogg"
   Delete "$INSTDIR\sound"
   
   Delete "$INSTDIR\glut32.dll"
   Delete "$INSTDIR\OpenAL32.dll"
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
      Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
      Delete "$SMPROGRAMS\${APP_NAME}\Uninstall ${APP_NAME}.lnk"
      !ifdef WEB_SITE
         Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME} Website.lnk"
      !endif
      Delete "$DESKTOP\${APP_NAME}.lnk"

      RmDir "$SMPROGRAMS\${APP_NAME}"
   !endif

   DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
   DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"
SectionEnd

######################################################################

