link.exe /ERRORREPORT:PROMPT /OUT:"%TARGET_PATH%.dll" /INCREMENTAL:NO /NOLOGO /LIBPATH:"%CA2_ROOT%time-windows/x64/basis" /LIBPATH:"C:/__archive/storage-windows/third/library/x64/basis" /LIBPATH:"C:/__archive/storage-windows/library/x64/basis" kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib "aura.lib" "sqlite.lib" "bzip2.lib" "libcrypto.lib" "libmysql.lib" "libssl.lib" "zlib.lib" "aura.lib" "data.lib" "pcre.lib" "netnode.lib" "time.lib" /MANIFEST:NO /DEBUG /PDB:"%HMH_LCTVWILD_PDB_PATH%.pdb" /SUBSYSTEM:WINDOWS /TLBID:1 /DYNAMICBASE /FIXED:NO /NXCOMPAT /MACHINE:X64 /DLL "%OBJFILE%" "C:/netnode/time-windows/intermediate/x64/basis/app-core/netnode_dynamic_source_script/netnode_dynamic_source_script_framework.obj"