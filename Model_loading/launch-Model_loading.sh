#!/bin/sh
bindir=$(pwd)
cd /home/fares/Downloads/Untitled Folder 3/Graphics Project/Lab2/Model_loading/
export LD_LIBRARY_PATH=:$LD_LIBRARY_PATH

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		 -batch -command=$bindir/gdbscript  /home/fares/Downloads/Untitled\ Folder\ 3/Graphics\ Project/new/Model_loading 
	else
		"/home/fares/Downloads/Untitled\ Folder\ 3/Graphics\ Project/new/Model_loading"  
	fi
else
	"/home/fares/Downloads/Untitled\ Folder\ 3/Graphics\ Project/new/Model_loading"  
fi
