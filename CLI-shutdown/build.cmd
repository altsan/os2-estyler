@echo off >shutdown.def
cmd /c ..\shared\util\BldLevelInf.cmd -D"Styler enhanced shutdown command" -V"1.1.202" -N"Alessandro Cantatore" shutdown.def
icc /Ss /B"/MAP" shutdown.c shutdown.def
