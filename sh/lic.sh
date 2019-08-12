#!/bin/tcsh -f
#setenv QTDIR  /home/cjh/qt561
#setenv QT5DIR  $QTDIR
setenv QTDIR  $QT5DIR
setenv QTINC  $QTDIR/include
setenv QTLIB  $QTDIR/lib

set path = ($QTDIR/bin $path)
set path = ($HOME/slickedit/bin $path)
setenv QT_QPA_PLATFORM_PLUGIN_PATH $QTDIR/plugins
#
if( $?LD_LIBRARY_PATH) then 
	setenv LD_LIBRARY_PATH $QTDIR/lib:$LD_LIBRARY_PATH
else
	setenv LD_LIBRARY_PATH $QTDIR/lib
endif
#
if( $?TETRIS_ROOT) then 
	#setenv TETRIS_ROOT  $NGP/configs/tetris
else
        setenv TETRIS_ROOT  $NGP/configs/tetris
endif 

setenv LIC_ROOT_PATH  $NGP/licmng
 
setenv LD_LIBRARY_PATH $NGP/support/json/lib:$LD_LIBRARY_PATH
setenv LD_LIBRARY_PATH $NGP/support/openssl/lib:$LD_LIBRARY_PATH
 
setenv LD_LIBRARY_PATH $LIC_ROOT_PATH/lib:$LD_LIBRARY_PATH
set path = ($LIC_ROOT_PATH/bin $path)
set path = (. $path)
