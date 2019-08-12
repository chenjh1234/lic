
include (../licInclude.config)
TEMPLATE = lib
INCLUDEPATH +=  $${INCLUDE_BUS}
INCLUDEPATH +=  ./\
	../licbase ../liccommon ../licserver
INCLUDEPATH += $${INCLUDE_HTTP_CALL} 
#LIBS +=  $${LIBS_BUS}
DESTDIR =$${LIC_LIB}

TARGET = licclient

DEFINES += CLIENT_LIB

HEADERS +=  

SOURCES += \
	  applic.cpp \
	  noServer.cpp \   
	  startClient.cpp
 
