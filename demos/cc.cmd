@echo off
rem bcc tvdemo1.cpp tvdemo2.cpp tvdemo3.cpp ascii.cpp calc.cpp calendar.cpp fileview.cpp gadgets.cpp mousedlg.cpp puzzle.cpp tv.lib
bcc -I\BCOS2\INCLUDE;..\tvision;..\ TVEDIT1.CPP TVEDIT2.CPP TVEDIT3.CPP tv.lib
