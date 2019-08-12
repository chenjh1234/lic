include (../licInclude.config)

TEMPLATE = app
DESTDIR =$${LIC_BIN}
TARGET = portal

INCLUDEPATH += $${INCLUDE_BUS}
INCLUDEPATH += $${INCLUDE_SERVER}
INCLUDEPATH += $${INCLUDE_CROW}

LIBS +=  $${LIBS_BUS}
LIBS +=  $${LIBS_APP}  -std=c++11 
 
SOURCES += \
	   portal.cpp 
