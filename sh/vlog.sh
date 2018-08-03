#!/bin/tcsh
set dt =  `date +%y%m`
set filea = `echo ALOG20$dt`
set filep = `echo PLOG20$dt`
set filee = `echo ELOG20$dt`
echo $1
set afile = $LIC_ROOT_PATH/log/$filea.log
set pfile = $LIC_ROOT_PATH/log/$filep.log
set efile = $LIC_ROOT_PATH/log/$filee.log

if ( $1 == "e" )  then
    cat $efile
else if ( $1 == "p" ) then
    cat $pfile
    else
    cat $afile
endif

