include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = appcall

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_SERVER}
INCLUDEPATH += ../http

LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP} /lib64/libdl.so.2
LIBS +=  -llichttpcall -lcurl
 
SOURCES += \
	   appcall.cpp 
