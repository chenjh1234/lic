include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = jsstr

INCLUDEPATH += . $${INCLUDE_BUS} 

LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} 


 

 
SOURCES += \
	   jason.cpp 
SOURCES +=  $${SOURCE_APP}

# to execute mycmd after build:
#MY_CMD =  echo 000000000000
MY_CMD = cd ../licserver; make install ;cd ../jason
# do the make install

mytarget.target = mycmd
mytarget.commands =  $${MY_CMD}
#if 1st command failed , it will ignore the later;
QMAKE_EXTRA_TARGETS += mytarget  

POST_TARGETDEPS += mycmd

#for install:
SH.path +=/$${LIC_ROOT_PATH}/bin
SH.files = ../sh/* 
INSTALLS +=SH
 
