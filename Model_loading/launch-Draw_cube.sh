#!/bin/sh
bindir=$(pwd)
cd /home/fares/Downloads/Untitled Folder 3/Graphics Project/Lab2/Draw_cube/
export LD_LIBRARY_PATH=:$LD_LIBRARY_PATH

if test "x$1" = "x--debugger"; then
	shift
	if test "x" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		 -batch -command=$bindir/gdbscript  /home/fares/Downloads/Untitled\ Folder\ 3/Graphics\ Project/new/Draw_cube 
	else
		"/home/fares/Downloads/Untitled\ Folder\ 3/Graphics\ Project/new/Draw_cube"  
	fi
else
	"/home/fares/Downloads/Untitled\ Folder\ 3/Graphics\ Project/new/Draw_cube"  
fi
