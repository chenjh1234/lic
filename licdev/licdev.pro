
include (../licInclude.config)
TEMPLATE = app
  
DESTDIR =$${LIC_BIN}
TARGET = licdev

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_SERVER}
#INCLUDEPATH += $${HTTP_CALL}

LIBS +=  $${LIBS_BUS}
 
LIBS += \
	-L $${LIC_ROOT_PATH}/lib\
	-llicbase  -llicconfig -lliccommon  -llicserver  -llicconfig -llicserverdev  /lib64/libdl.so.2   

HEADERS +=  
 
SOURCES +=  \
	#../server/licServer.cpp\
	    startS.cpp\
	    licdev.cpp 	 
	    
