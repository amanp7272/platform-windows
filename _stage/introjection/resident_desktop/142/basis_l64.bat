link.exe /ERRORREPORT:PROMPT /OUT:"%TARGET_PATH%.dll" /EXPORT:resident_bot_get_new_library /MANIFEST:NO /NXCOMPAT /DYNAMICBASE "ws2_32.lib" "winmm.lib" "version.lib" "vfw32.lib" "opengl32.lib" "glu32.lib" "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" "veriwell_multimedia.lib" "ace.lib" "aura.lib" "axis.lib" "base.lib" "aura.lib" "programming.lib" "announcecom.lib" "multimedia.lib" "resident.lib" "app_core_resident_client.lib" "app_core_resident_desktop.lib" "_ace.lib" "C:\xcore\time\intermediate\x64\basis\app-core\app_core_resident_desktop\xcore\app-core\resident_desktop\framework.obj" "%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/app-core/app_core_resident_desktop/%ITEM_NAME%/%ITEM_TITLE%.obj" /FIXED:NO /IMPLIB:"%CA2_ROOT%time/intermediate/%PLATFORM%/%CONFIGURATION%/app-core/app_core_resident_desktop/%ITEM_NAME%/%ITEM_TITLE%.lib" /DEBUG:FULL /DLL /MACHINE:X64 /INCREMENTAL /SUBSYSTEM:WINDOWS /PDB:"%HMH_LCTVWILD_PDB_PATH%.pdb" /ERRORREPORT:PROMPT /NOLOGO /LIBPATH:"%CA2_ROOT%time\x64\basis\" /LIBPATH:"C:/archive/platform-windows/x64/basis/lib" /LIBPATH:"C:/archive/platform-windows/thirdparty/library/x64/basis" /TLBID:1 