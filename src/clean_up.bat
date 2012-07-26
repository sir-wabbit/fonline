cls
@echo ### Cleaning up the source dirs... Close project VISUAL STUDIO!!!! ###

del *.opt *.plg *.ncb *.log *.sbr *.obj

cd Debug
del *.res *.log *.sbr *.obj *.idb *.pch *.bsc *.pdb

cd..

cd Release
del *.res *.log *.sbr *.obj *.idb *.pch *.bsc *.pdb

cd..

@echo.
@echo ### Cleaning finished! ###
@echo.
@echo ### Press any key! ###
@pause > nul