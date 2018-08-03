#!/bin/tcsh -f
setenv QTDIR  /home/cjh/qt561
setenv QT5DIR  $QTDIR
setenv QTINC  $QTDIR/include
setenv QTLIB  $QTDIR/lib

set path = ($QTDIR/bin $path)
set path = ($HOME/slickedit/bin $path)
setenv QT_QPA_PLATFORM_PLUGIN_PATH $QTDIR/plugins
if( $?LD_LIBRARY_PATH) then 
	setenv LD_LIBRARY_PATH $QTDIR/lib:$LD_LIBRARY_PATH
else
	setenv LD_LIBRARY_PATH $QTDIR/lib
endif

setenv LD_LIBRARY_PATH /mnt/hgfs/win/intelso/lib64:$LD_LIBRARY_PATH
setenv LD_LIBRARY_PATH /mnt/hgfs/win/boost/lib:$LD_LIBRARY_PATH
setenv LD_LIBRARY_PATH /home/cjh/tetris/src/third/json/lib:$LD_LIBRARY_PATH
#setenv LIC_SRC_PATH  /home/cjh/licsrc/licsrc
setenv TETRIS_ROOT  /mnt/hgfs/win/licsrc
setenv LIC_ROOT_PATH  /home/cjh/licsrc/install
setenv LD_LIBRARY_PATH $LIC_ROOT_PATH/lib:$LD_LIBRARY_PATH
set path = ($LIC_ROOT_PATH/bin $path)
set path = (. $path)
